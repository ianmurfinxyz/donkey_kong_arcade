#include <allegro5/allegro_primitives.h>
#include <sstream>
#include <iomanip>
#include "hud.hh"
#include "../game/constants.hh"
#include "../assets/fonts.hh"
#include "../assets/sprites.hh"

namespace
{
  constexpr float flash_period_s = 0.5f;
  float flash_clock_s = 0.f;
  bool show_player_up = true;
}

namespace hud {
  void update(float dt_s)
  {
    flash_clock_s += dt_s;
    if(flash_clock_s > flash_period_s){
      flash_clock_s = 0.f;
      show_player_up = !show_player_up;
    }
  }

  void draw(hud_stats stats)
  {
    if(show_player_up) draw_text(16, 0, "1UP", TEXT_RED);
    draw_text(73, 0, "HIGH SCORE", TEXT_RED);
    draw_text(170, 24, "L=", TEXT_BLUE);

    std::stringstream ss{};
    ss << std::fixed << std::setprecision(0) << std::setfill('0') << std::setw(7) << stats.current_score;
    draw_text(9, 8, ss.str(), TEXT_WHITE);

    std::stringstream{}.swap(ss);
    ss << std::fixed << std::setprecision(0) << std::setfill('0') << std::setw(7) << stats.high_score;
    draw_text(89, 8, ss.str(), TEXT_WHITE);

    std::stringstream{}.swap(ss);
    ss << std::fixed << std::setprecision(0) << std::setfill('0') << std::setw(2) << stats.level_number;
    draw_text(186, 24, ss.str(), TEXT_BLUE);

    for(auto i = 0; i < stats.life_count; ++i){
      draw_sprite(SID_MARIO_LIFE, vector2i{9 + (8 * i), 24});
    }
  }
}
