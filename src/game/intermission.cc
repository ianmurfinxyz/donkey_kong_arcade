#include <array>
#include "types.hh"
#include "constants.hh"
#include "../assets/sounds.hh"
#include "../assets/sprites.hh"
#include "../assets/fonts.hh"

namespace intermission {
  static float clock_s;

  void on_enter(game_data& gd)
  {
    clock_s = 0.f;
    play_sound(SND_HOW_HIGH);
  }

  void update(game_data& gd, float dt_s)
  {
    clock_s += dt_s;
  }

  bool is_done(game_data& gd)
  {
    return clock_s > con::intermission_duration_s;
  }

  static constexpr std::array<const char*, 6> meter_text {
    " 25 m",
    " 50 m",
    " 75 m",
    "100 m",
    "125 m",
    "150 m",
  };

  void draw(game_data& gd)
  {
    assert(1 <= gd.stage_number && gd.stage_number <= 6);
    for(auto stage = 0; stage < gd.stage_number; ++stage){
      vector2f orangutan_pos{};
      orangutan_pos.x = 89;
      orangutan_pos.y = 200 - (stage * con::orangutan_height_px);
      draw_sprite(SID_ORANGUTAN, orangutan_pos);

      vector2i meter_pos{};
      meter_pos.x = 32;
      meter_pos.y = 225 - (stage * con::orangutan_height_px);
      draw_text(meter_pos.x, meter_pos.y, meter_text[stage], TEXT_WHITE);

      draw_text(25, 240, "HOW HIGH CAN YOU GET ?", TEXT_WHITE);
    }
  }
}
