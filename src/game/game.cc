#include <allegro5/allegro_primitives.h>
#include "types.hh"
#include "game.hh"
#include "constants.hh"
#include "stages/stages.hh"
#include "../core/input.hh"
#include "../core/rand.hh"
#include "../assets/fonts.hh"
#include "../assets/sounds.hh"
#include "../core/global.hh"

namespace intermission {
  void on_enter(game_data& gd);
  void update(game_data& gd, float dt_s);
  bool is_done(game_data& gd);
  void draw(game_data& gd);
}

namespace play {
  void on_enter(game_data& gd);
  void on_exit(game_data& gd);
  void update(game_data& gd, float dt_s);
  void draw(game_data& gd);
}

namespace {
  snd_play_id pid_scene_sound;
}

namespace
{
  void change_state(game_data& gd, game_state next_state)
  {
    if(gd.state == game_state::play){
      play::on_exit(gd);
    }
    switch(next_state){
      case game_state::enter_cutscene:
        if(gd.current_stage->enter_cutscene){
          gd.cutscene_player.initialise(*gd.current_stage->enter_cutscene);
          if(gd.current_stage->enter_cutscene_sound != SND_COUNT)
            pid_scene_sound = play_sound(gd.current_stage->enter_cutscene_sound);
        }
        else gd.cutscene_player.initialise(cutscenes::delay);
        break;
      case game_state::intermission:
        intermission::on_enter(gd);
        break;
      case game_state::play: {
        clear_static_world(gd);
        clear_dynamic_world(gd);
        gd.jump_man.flags |= FLAG_JM_DEAD;
        gd.current_stage->setup(gd);
        gd.difficulty = std::min(gd.level_number, con::max_difficulty);
        gd.difficulty_clock_s = 0.f;
        gd.start_bonus = std::min(((gd.level_number * 10) + 40) * 100, 8000);
        gd.bonus_clock_s = 0.f;
        play::on_enter(gd);
        break;
      }
      case game_state::game_over:
        clear_dynamic_world(gd);
        break;
      case game_state::exit_cutscene:
        clear_dynamic_world(gd);
        clear_world_actors(gd);
        if(gd.current_stage->exit_cutscene){
          gd.cutscene_player.initialise(*gd.current_stage->exit_cutscene);
          const auto& exit_sounds = gd.current_stage->exit_cutscene_sounds;
          if(exit_sounds[0] != SND_COUNT){
            if(exit_sounds[1] != SND_COUNT) play_sound(exit_sounds[rnd::uniformSignedInt(0, 1)]);
            else play_sound(exit_sounds[0]);
          }
        }
        else gd.cutscene_player.initialise(cutscenes::delay);
        // run an initial update to add all the 0s start frames, this avoids a
        // flicker in which the first cutscene draw is all black because the 1st draw
        // is called before the 1st update.
        gd.cutscene_player.update(0.01);
        break;
    }
    gd.state = next_state;
  }

  void select_next_level(game_data& gd)
  {
    ++gd.stage_number;
    switch(gd.level_number){
      case 1: if(gd.stage_number > 2) {gd.stage_number = 1; break;}
      case 2: if(gd.stage_number > 3) {gd.stage_number = 1; break;}
      case 3: if(gd.stage_number > 4) {gd.stage_number = 1; break;}
      case 4: if(gd.stage_number > 5) {gd.stage_number = 1; break;}
      default: if(gd.stage_number > 6) {gd.stage_number = 1; break;}
    }
    if(gd.stage_number == 1) ++gd.level_number;

    static const auto select_barrel_stage = [](game_data& gd){
      gd.current_stage = global::is_classic_mode ?
        &stage::barrel_stages[0] :
        &stage::barrel_stages[rnd::uniformSignedInt(0, stage::barrel_stage_count - 1)];
    };
    static const auto select_challenge_1_stage = [](game_data& gd){
      gd.current_stage = global::is_classic_mode ?
        &stage::challenge_1_stages[0] :
        &stage::challenge_1_stages[rnd::uniformSignedInt(0, stage::challenge_1_stage_count - 1)];
    };
    static const auto select_challenge_2_stage = [](game_data& gd){
      gd.current_stage = global::is_classic_mode ?
        &stage::challenge_2_stages[0] :
        &stage::challenge_2_stages[rnd::uniformSignedInt(0, stage::challenge_2_stage_count - 1)];
    };
    static const auto select_kill_stage = [](game_data& gd){
      gd.current_stage = global::is_classic_mode ?
        &stage::kill_stages[0] :
        &stage::kill_stages[rnd::uniformSignedInt(0, stage::kill_stage_count - 1)];
    };
    assert(gd.level_number > 0);
    switch(gd.level_number){
      case 1:
        if(gd.stage_number == 1) select_barrel_stage(gd);
        else if(gd.stage_number == 2) select_kill_stage(gd);
        else assert(0);
        break;
      case 2:
        if(gd.stage_number == 1) select_barrel_stage(gd);
        else if(gd.stage_number == 2) select_challenge_2_stage(gd);
        else if(gd.stage_number == 3) select_kill_stage(gd);
        else assert(0);
        break;
      case 3:
        if(gd.stage_number == 1) select_barrel_stage(gd);
        else if(gd.stage_number == 2) select_challenge_1_stage(gd);
        else if(gd.stage_number == 3) select_challenge_2_stage(gd);
        else if(gd.stage_number == 4) select_kill_stage(gd);
        else assert(0);
        break;
      case 4:
        if(gd.stage_number == 1) select_barrel_stage(gd);
        else if(gd.stage_number == 2) select_challenge_1_stage(gd);
        else if(gd.stage_number == 3) select_barrel_stage(gd);
        else if(gd.stage_number == 4) select_challenge_2_stage(gd);
        else if(gd.stage_number == 5) select_kill_stage(gd);
        else assert(0);
        break;
      default:
        if(gd.stage_number == 1) select_barrel_stage(gd);
        else if(gd.stage_number == 2) select_challenge_1_stage(gd);
        else if(gd.stage_number == 3) select_barrel_stage(gd);
        else if(gd.stage_number == 4) select_challenge_2_stage(gd);
        else if(gd.stage_number == 5) select_barrel_stage(gd);
        else if(gd.stage_number == 6) select_kill_stage(gd);
        else assert(0);
        break;
    }
  }

  void draw_game_over_sign()
  {
    al_draw_filled_rectangle(56, 160, 167, 200, al_map_rgb(0, 0, 0));
    draw_text(72, 176, "GAME", TEXT_CYAN);
    draw_text(120, 176, "OVER", TEXT_CYAN);
  }
}

namespace game
{
  std::unique_ptr<game_data> create()
  {
    return std::make_unique<game_data>();
  }

  void on_enter(game_data& gd)
  {
    gd.level_number = 0;
    gd.stage_number = 0;
    gd.difficulty = con::min_difficulty;
    gd.mob_ai_tick_clock_s = 0.f;
    gd.death_freeze_clock_s = 0.f;
    gd.conveyor_tick_clock_s = 0.f;
    gd.mob_spawn_clock_s = 0.f;
    gd.kong_beat_chest_clock_s = 0.f;
    gd.jump_man_life_count = con::starting_life_count;
    gd.flags = 0;
    gd.score = 0;
    select_next_level(gd);
    change_state(gd, game_state::enter_cutscene);
  }

  void update(game_data& gd, float dt_s)
  {
    if(gd.flags & FLAG_GAME_OVER) return;

    switch(gd.state){
      case game_state::enter_cutscene:
        gd.cutscene_player.update(dt_s);
        if(gd.cutscene_player.is_done() || input::is_action_triggered(input::ACTION_SKIP_CUTSCENE)){
          change_state(gd, game_state::intermission);
          stop_sound(pid_scene_sound);
        }
        break;
      case game_state::intermission:
        intermission::update(gd, dt_s);
        if(intermission::is_done(gd)) {
          change_state(gd, game_state::play);
        }
        break;
      case game_state::play:
        if(global::is_debug_enabled && input::is_action_triggered(input::ACTION_FORCE_NEXT_LVL)){
          gd.flags = 0;
          select_next_level(gd);
          change_state(gd, game_state::enter_cutscene);
          break;
        }
        play::update(gd, dt_s);
        if(gd.current_stage->is_won(gd)){
          gd.score += gd.bonus;
          if(gd.current_stage->on_won) gd.current_stage->on_won(gd);
          change_state(gd, game_state::exit_cutscene);
        }
        else if(gd.jump_man_life_count < 0){
          change_state(gd, game_state::game_over);
        }
        break;
      case game_state::game_over:
        gd.game_over_clock_s += dt_s;
        if(gd.game_over_clock_s > con::game_over_duration_s){
          gd.flags |= FLAG_GAME_OVER;
        }
        break;
      case game_state::exit_cutscene:
        gd.cutscene_player.update(dt_s);
        if(gd.cutscene_player.is_done()){
          select_next_level(gd);
          change_state(gd, game_state::enter_cutscene);
        }
        break;
    }
  }

  bool is_done(game_data& gd)
  {
    return gd.flags & FLAG_GAME_OVER;
  }

  void draw(game_data& gd)
  {
    if(gd.state == game_state::enter_cutscene){
      gd.cutscene_player.draw();
    }
    else if(gd.state == game_state::intermission){
      intermission::draw(gd);
    }
    else{
      play::draw(gd);
    }

    if(gd.state == game_state::exit_cutscene){
      gd.cutscene_player.draw();
    }
    else if(gd.state == game_state::game_over){
      draw_game_over_sign();
    }
  }

  game_stats get_game_stats(game_data& gd)
  {
    return {gd.level_number, gd.jump_man_life_count, gd.score};
  }
}