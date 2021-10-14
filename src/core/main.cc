#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <exception>
#include <string>
#include <cstring>
#include <sstream>
#include <ctime>
#include <random>
#include <memory>
#include "log.hh"
#include "input.hh"
#include "../assets/sprites.hh"
#include "../assets/fonts.hh"
#include "../assets/sounds.hh"
#include "../game/constants.hh"
#include "../hud/hud.hh"
#include "../title/title.hh"
#include "../menu/menu.hh"
#include "../hiscores/hiscores.hh"
#include "../game/game.hh"
#include "rand.hh"
#include "math.hh"

namespace global
{
  int window_width_px = 600;
  int window_height_px = 600;
  bool is_debug_draw = false;
  bool is_debug_enabled = false;
  bool is_jm_invulnerable = false;
  bool is_classic_mode = true;
}

/** new width and height must be greater than world width and height. */
irect calculate_world_bitmap_blit_rect(int new_width_px, int new_height_px)
{
  global::window_width_px = new_width_px;
  global::window_height_px = new_height_px;

  assert(global::window_width_px >= con::world_width_px);
  assert(global::window_height_px >= con::world_height_px);

  auto scale_x = 1;
  while((con::world_width_px * (scale_x + 1)) < global::window_width_px){
    ++scale_x;
  }
  auto scale_y = 1;
  while((con::world_height_px * (scale_y + 1)) < global::window_height_px){
    ++scale_y;
  }
  auto world_bitmap_draw_scale = std::min(scale_x, scale_y);

  auto drawn_world_width_px = con::world_width_px * world_bitmap_draw_scale;
  auto drawn_world_height_px = con::world_height_px * world_bitmap_draw_scale;

  return {
    (global::window_width_px - drawn_world_width_px) / 2,
    (global::window_height_px - drawn_world_height_px) / 2,
    drawn_world_width_px,
    drawn_world_height_px,
  };
}

int main()
{
  float target_fps_hz = 60.f;
  ALLEGRO_DISPLAY* display;
  ALLEGRO_EVENT_QUEUE* event_queue;
  ALLEGRO_TIMER* game_ticker;
  double real_time_s = 0.0;
  double game_time_s = 0.0;
  float tick_delta_s;
  bool is_paused = false;
  bool is_stat_draw = false;
  bool is_fullscreen = false;
  int fps_tick_counter = 0;
  int ticks_done_this_frame = 0;
  int ticks_accumulated = 0;
  bool need_redraw = false;
  bool is_running = true;
  double fps_timer_s = 0.0;
  double fps = 0.0;
  int top_hiscore = 0;
  ALLEGRO_FONT* debug_font;
  ALLEGRO_BITMAP* world_bitmap;  // world drawn to this 224x256px bitmap, then scale drawn to window.
  irect world_bitmap_blit_rect;   // area of window to draw world_bitmap to.
  std::unique_ptr<game_data> game;

  hiscores::reg::initialise();

  enum class app_state { title, menu, game, hi_score };
  app_state app_state_;
  const auto change_app_state = [&game, &app_state_](app_state new_state, int new_score = 0)
  {
    switch(new_state){
      case app_state::title: {title::on_enter(); break;}
      case app_state::menu: {menu::on_enter(); break;}
      case app_state::hi_score: {hiscores::reg::on_enter(new_score); break;}
      case app_state::game: {game::on_enter(*game); break;}
      default: assert(0);
    }
    app_state_ = new_state;
  };
  change_app_state(app_state::title, 1000);

  // load config file.
  {
    log(log_lvl::info, "loading config");

    ALLEGRO_CONFIG* dk_config = al_load_config_file("assets/dk_config.ini");
    if(dk_config != nullptr){
      const auto log_set_value = [](const char* value_name, const char* value){
        std::stringstream ss{};
        ss << "set '" << fmt_bold << value_name << fmt_clear
        << "' to '" << fmt_bold << value << fmt_clear << "'.";
        log(log_lvl::info, ss.str());
      };

      auto log_invalid_value = [](const char* value_name, const char* value, const char* reason){
        std::stringstream ss{};
        ss << "invalid config value '" << fmt_bold << value << fmt_clear
        << "' for config property '" << fmt_bold << value_name << fmt_clear
        << "'; " << reason;
        log(log_lvl::warning, ss.str());
      };

      const char* value;
      value = al_get_config_value(dk_config, "core", "target_fps_hz");
      if(value != nullptr){
        try {
          target_fps_hz = std::stof(value);
          log_set_value("target_fps_hz", value);
        }
        catch(const std::exception& e){
          log_invalid_value("target_fps_hz", value, "expected double.");
        }
      }

      auto is_valid_window_size = true;
      value = al_get_config_value(dk_config, "core", "window_width_px");
      if(value != nullptr){
        try {
          global::window_width_px = std::stoi(value);
          log_set_value("window_width_px", value);
        }
        catch(const std::exception& e){
          log_invalid_value("window_width_px", value, "expected integer.");
          is_valid_window_size = false;
        }
      }
      value = al_get_config_value(dk_config, "core", "window_height_px");
      if(value != nullptr){
        try {
          global::window_height_px = std::stoi(value);
          log_set_value("window_height_px", value);
        }
        catch(const std::exception& e){
          log_invalid_value("window_height_px", value, "expected integer.");
          is_valid_window_size = false;
        }
      }
      if(global::window_width_px < con::world_width_px){
        log(log_lvl::info, "config value for window width too small! ignoring...");
        is_valid_window_size = false;
      }
      if(global::window_height_px < con::world_height_px){
        log(log_lvl::info, "config value for window height too small! ignoring...");
        is_valid_window_size = false;
      }
      if(!is_valid_window_size){
        global::window_width_px = con::world_width_px;
        global::window_height_px = con::world_height_px;
        log_set_value("window_width_px", std::to_string(global::window_width_px).c_str());
        log_set_value("window_height_px", std::to_string(global::window_height_px).c_str());
      }

      value = al_get_config_value(dk_config, "core", "control_scheme");
      if(value != nullptr){
        if(std::strcmp("wasd", value) == 0){
          input::set_control_scheme(input::wasd_control_scheme);
          log_set_value("control_scheme", value);
        }
        else if(std::strcmp("arrows", value) == 0){
          input::set_control_scheme(input::arrows_control_scheme);
          log_set_value("control_scheme", value);
        }
        else {
          log_invalid_value("control_scheme", value, "unknown option");
        }
      }

      value = al_get_config_value(dk_config, "core", "debug_mode");
      if(value != nullptr){
        try {
          auto debug_enabled = std::stoi(value);
          debug_enabled = debug_enabled > 0 ? 1 : 0;
          log_set_value("debug_mode", debug_enabled ? "true" : "false");
          global::is_debug_enabled = static_cast<bool>(debug_enabled);
        }
        catch(const std::exception& e){
          log_invalid_value("debug_mode", value, "expected integer (boolean).");
          global::is_debug_enabled = false;
        }
      }
      value = al_get_config_value(dk_config, "core", "invulnerable_mode");
      if(value != nullptr){
        try {
          auto invulnerable_enabled = std::stoi(value);
          invulnerable_enabled = invulnerable_enabled > 0 ? 1 : 0;
          log_set_value("invulnerable_mode", invulnerable_enabled ? "true" : "false");
          global::is_jm_invulnerable = static_cast<bool>(invulnerable_enabled);
        }
        catch(const std::exception& e){
          log_invalid_value("invulnerable_mode", value, "expected integer (boolean).");
          global::is_jm_invulnerable = false;
        }
      }
      value = al_get_config_value(dk_config, "core", "classic_mode");
      if(value != nullptr){
        try {
          auto classic_enabled = std::stoi(value);
          classic_enabled = classic_enabled > 0 ? 1 : 0;
          log_set_value("classic mode", classic_enabled ? "true" : "false");
          global::is_classic_mode = static_cast<bool>(classic_enabled);
        }
        catch(const std::exception& e){
          log_invalid_value("invulnerable_mode", value, "expected integer (boolean).");
          global::is_classic_mode = true;
        }
      }
      value = al_get_config_value(dk_config, "core", "fullscreen");
      if(value != nullptr){
        try {
          is_fullscreen = std::stoi(value) > 0;
          log_set_value("fullscreen mode", is_fullscreen ? "true" : "false");
        }
        catch(const std::exception& e){
          log_invalid_value("fullscreen", value, "expected integer (boolean).");
          is_fullscreen = false;
        }
      }
    }
    else log(log_lvl::warning, "failed to load config file");
  }

  // load hiscores
  {
    auto table = hiscores::load_hiscores();
    top_hiscore = hiscores::get_top_hiscore();
    hiscores::log_hiscores();
  }

  // initialise core
  {
    const auto require_init = [](bool init_result, const char* system_name) -> void {
      if(!init_result){
        std::string msg{"failed to initialise "};
        msg += system_name;
        log(log_lvl::fatal, msg);
        exit(EXIT_FAILURE);
      }
    };

    log(log_lvl::info, "initialising core.");

    require_init(al_init(), "allegro");
    require_init(al_init_image_addon(), "allegro image addon");
    require_init(al_init_font_addon(), "allegro font addon");
    require_init(al_init_ttf_addon(), "allegro ttf addon");
    require_init(al_init_primitives_addon(), "allegro primitives addon");
    require_init(al_install_keyboard(), "allegro keyboard driver");
    require_init(al_install_audio(), "allegro audio addon");
    require_init(al_init_acodec_addon(), "allegro audio codecs addon");
    require_init(al_reserve_samples(16), "audio sample reservation");

    if(is_fullscreen) al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    else al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);
    display = al_create_display(global::window_width_px, global::window_height_px);
    require_init(display != nullptr, "allegro display");
    global::window_width_px = al_get_display_width(display);
    global::window_height_px = al_get_display_height(display);

    auto old_bitmap_flags = al_get_new_bitmap_flags();
    al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
    world_bitmap = al_create_bitmap(con::world_width_px, con::world_height_px);
    require_init(world_bitmap != nullptr, "world bitmap");
    al_set_new_bitmap_flags(old_bitmap_flags);
    world_bitmap_blit_rect = calculate_world_bitmap_blit_rect(global::window_width_px, global::window_height_px);

    event_queue = al_create_event_queue();
    require_init(event_queue != nullptr, "allegro event queue");

    tick_delta_s = 1.f / target_fps_hz;
    game_ticker = al_create_timer(tick_delta_s);
    require_init(game_ticker != nullptr, "allegro timer for game ticker");

    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(game_ticker));
    al_register_event_source(event_queue, al_get_display_event_source(display));

    debug_font = al_create_builtin_font();
    require_init(debug_font, "debug font");

    // seed random number generator.
    auto seq = std::seed_seq{std::time(nullptr)};
    auto seed_state = rnd::xorwow::state_type{};
    seq.generate(seed_state.begin(), seed_state.end());
    rnd::generator.seed(seed_state);
  }

  // initialise game.
  {
    log(log_lvl::info, "initialising game.");
    if(!load_sprite_sheets()){
      exit(EXIT_FAILURE);
    }
    if(!load_fonts()){
      exit(EXIT_FAILURE);
    }
    if(!load_sounds()){
      exit(EXIT_FAILURE);
    }
    game = std::move(game::create());
  }

  play_sound(SND_BOOT);

  // runloop
  {
    al_start_timer(game_ticker);
    while(is_running)
    {
      do {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);
        switch(event.type){
          case ALLEGRO_EVENT_TIMER:
            real_time_s += tick_delta_s;
            fps_timer_s += tick_delta_s;
            ++ticks_accumulated;
            break;
          case ALLEGRO_EVENT_KEY_DOWN:
            if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
              is_running = false;
            }
            else if(event.keyboard.keycode == ALLEGRO_KEY_TAB){
              global::is_debug_draw = !global::is_debug_draw;
              auto debug_state_string = global::is_debug_draw ? "on" : "off";
              log(log_lvl::info, std::string{"debug mode "} + debug_state_string);
            }
            else if(event.keyboard.keycode == ALLEGRO_KEY_O){
              is_paused = !is_paused;
              auto pause_string = is_paused ? "paused game" : "unpaused game";
              log(log_lvl::info, pause_string);
            }
            else if(event.keyboard.keycode == ALLEGRO_KEY_P){
              is_stat_draw = !is_stat_draw;
              auto stats_string = is_stat_draw ? "enabling" : "disabling";
              log(log_lvl::info, stats_string + std::string{" runtime stats output"});
            }
            input::record_key_pressed(event.keyboard.keycode);
            break;
          case ALLEGRO_EVENT_KEY_UP:
            input::record_key_released(event.keyboard.keycode);
            break;
          case ALLEGRO_EVENT_DISPLAY_RESIZE:
            al_acknowledge_resize(display);
            // stop resizing once window gets too small and allow the game to overflow the window bounds.
            if(event.display.width >= con::world_width_px && event.display.height >= con::world_height_px){
              world_bitmap_blit_rect = calculate_world_bitmap_blit_rect(event.display.width, event.display.height);
            }
            break;
          case ALLEGRO_EVENT_DISPLAY_CLOSE:
            is_running = false;
            break;
        }
      }
      while(!al_event_queue_is_empty(event_queue));

      // limiting ticks per frame prevents a 'spiral of death'.
      constexpr auto max_ticks_per_frame = 5;
      ticks_done_this_frame = 0;
      while(ticks_accumulated > 0 && ticks_done_this_frame < max_ticks_per_frame){
        if(!is_paused){
          switch(app_state_){
            case app_state::title:
              title::update(tick_delta_s);
              if(title::is_done()) change_app_state(app_state::menu); // TODO: switch to menu not game.
              break;
            case app_state::menu:
              menu::update(tick_delta_s);
              if(menu::is_playtime()) change_app_state(app_state::game);
              else if(menu::is_title_time()) change_app_state(app_state::title);
              break;
            case app_state::game:
              game::update(*game, tick_delta_s);
              if(game::is_done(*game)){
                auto final_score = game::get_game_stats(*game).score;
                if(hiscores::is_hiscore(final_score)) change_app_state(app_state::hi_score, final_score);
                else change_app_state(app_state::title);
              }
              break;
            case app_state::hi_score:
              hiscores::reg::update(tick_delta_s);
              if(hiscores::reg::is_done()) {
                top_hiscore = hiscores::get_top_hiscore();
                change_app_state(app_state::title);
              }
              break;
          }
          game_time_s += tick_delta_s;
        }
        hud::update(tick_delta_s);
        input::update_key_states();
        need_redraw = true;
        --ticks_accumulated;
        ++ticks_done_this_frame;
        ++fps_tick_counter;
      }
      if(fps_timer_s > 1.0){
        fps = fps_tick_counter / fps_timer_s;
        fps_timer_s = 0.0;
        fps_tick_counter = 0;
      }
      if(need_redraw){
        al_set_target_bitmap(world_bitmap);
        al_clear_to_color(al_map_rgb(0, 0, 0));
        switch(app_state_){
            case app_state::title: {title::draw(); break;}
            case app_state::menu: {menu::draw(); break;}
            case app_state::game: {game::draw(*game); break;}
            case app_state::hi_score: {hiscores::reg::draw(); break;}
            default: assert(0);
        }
        auto stats = app_state_ == app_state::game ? game::get_game_stats(*game) : game_stats{0, 0, 0};
        hud::draw({top_hiscore, stats.score, stats.level_number, stats.life_count});
        al_set_target_backbuffer(display);
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_scaled_bitmap(
          world_bitmap,
          0,
          0,
          con::world_width_px,
          con::world_height_px,
          world_bitmap_blit_rect.x,
          world_bitmap_blit_rect.y,
          world_bitmap_blit_rect.w,
          world_bitmap_blit_rect.h,
          0
        );
        if(is_stat_draw){
          // draw the runtime statistics window in bottom-left.
          constexpr auto box_w_px = 200;
          constexpr auto box_h_px = 70;
          al_draw_filled_rectangle(
            0,
            static_cast<float>(global::window_height_px - box_h_px),
            box_w_px,
            static_cast<float>(global::window_height_px),
            al_map_rgb(0, 0, 255)
          );
          std::stringstream ss{};
          ss.precision(3);
          ss << "real_time(secs)=" << real_time_s << " game_time(secs)=" << game_time_s << "\n"
             << "fps=" << fps << " ticks_per_frame=" << ticks_done_this_frame;
          constexpr auto box_padding_px = 10;
          al_draw_multiline_text(
            debug_font,
            al_map_rgb(255, 255, 255),
            box_padding_px,
            static_cast<float>(global::window_height_px - box_h_px + box_padding_px),
            box_w_px - (box_padding_px * 2),
            14,
            ALLEGRO_ALIGN_LEFT,
            ss.str().c_str()
            );
        }
        if(is_paused){
          // draw the paused icon.
          static auto bg_color = al_map_rgb(255, 0, 0);
          static auto fg_color = al_map_rgb(255, 0, 0);
          constexpr auto radius_px = 30;
          constexpr auto diameter_px = radius_px * 2;
          auto center_x_px = 20 + radius_px;
          auto center_y_px = center_x_px;
          constexpr auto border_thickness_px = 4.f;
          al_draw_circle(
            static_cast<float>(center_x_px),
            static_cast<float>(center_y_px),
            radius_px,
            fg_color,
            border_thickness_px
          );
          constexpr int stripe_half_gap_px = diameter_px * 0.06f;
          constexpr int stripe_w_px = diameter_px * 0.16f;
          constexpr int stripe_h_px = diameter_px * 0.4f;
          constexpr int stripe_half_h_px = stripe_h_px / 2.f;
          constexpr int strip_border_radius_px = 2;
          al_draw_filled_rounded_rectangle(
            center_x_px - stripe_half_gap_px - stripe_w_px,
            center_y_px - stripe_half_h_px,
            center_x_px - stripe_half_gap_px,
            center_y_px + stripe_half_h_px,
            strip_border_radius_px,
            strip_border_radius_px,
            fg_color
          );
          al_draw_filled_rounded_rectangle(
            center_x_px + stripe_half_gap_px,
            center_y_px - stripe_half_h_px,
            center_x_px + stripe_half_gap_px + stripe_w_px,
            center_y_px + stripe_half_h_px,
            strip_border_radius_px,
            strip_border_radius_px,
            fg_color
            );
        }
        al_flip_display();
      }
    }
  }

  // shutdown game
  {
    log(log_lvl::info, "shutting down game.");
    unload_sprite_sheets();
    unload_fonts();
    unload_sounds();
  }

  // shutdown core
  {
    log(log_lvl::info, "shutting down core.");
    al_destroy_font(debug_font);
    al_destroy_bitmap(world_bitmap);
    al_destroy_timer(game_ticker);
    if(event_queue != nullptr){
      al_destroy_event_queue(event_queue);
    }
    if(display != nullptr){
      al_destroy_display(display);
    }
    al_uninstall_keyboard();
    al_shutdown_ttf_addon();
    al_shutdown_font_addon();
    al_shutdown_image_addon();
    al_shutdown_primitives_addon();
  }
}
