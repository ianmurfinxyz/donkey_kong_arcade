#include "../world.hh"
#include "../constants.hh"
#include "../../core/rand.hh"

namespace stage::classic_rivets
{
  static void spawn_rivets(game_data& gd)
  {
    spawn_rivet(gd, 56, 87);
    spawn_rivet(gd, 160, 87);
    spawn_rivet(gd, 56, 127);
    spawn_rivet(gd, 160, 127);
    spawn_rivet(gd, 56, 167);
    spawn_rivet(gd, 160, 167);
    spawn_rivet(gd, 56, 207);
    spawn_rivet(gd, 160, 207);
  }

  static void spawn_collectables(game_data& gd)
  {
    spawn_collectable(gd, vector2f{28, 72}, COLLECTABLE_PARASOL);
    spawn_collectable(gd, vector2f{197, 199}, COLLECTABLE_HAT);
    spawn_collectable(gd, vector2f{127, 238}, COLLECTABLE_BAG);
    spawn_hammer(gd, vector2f{105, 98}, hammer_number::hammer_0);
    spawn_hammer(gd, vector2f{8, 138}, hammer_number::hammer_1);
  }

  void setup(game_data& gd)
  {
    // flat ground girder.
    spawn_girder(gd, 0, 248, 28, girder_color::blue, FLAG_BOTTOM_GIRDER);
    // lady girder.
    spawn_girder(gd, 56, 48, 14, girder_color::blue);

    spawn_girder(gd, 8, 208, 6, girder_color::blue);
    spawn_girder(gd, 64, 208, 12, girder_color::blue);
    spawn_girder(gd, 168, 208, 6, girder_color::blue);

    spawn_girder(gd, 16, 168, 5, girder_color::blue);
    spawn_girder(gd, 64, 168, 12, girder_color::blue);
    spawn_girder(gd, 168, 168, 5, girder_color::blue);

    spawn_girder(gd, 24, 128, 4, girder_color::blue);
    spawn_girder(gd, 64, 128, 12, girder_color::blue);
    spawn_girder(gd, 168, 128, 4, girder_color::blue);

    spawn_girder(gd, 32, 88, 3, girder_color::blue);
    spawn_girder(gd, 64, 88, 12, girder_color::blue);
    spawn_girder(gd, 168, 88, 3, girder_color::blue);

    spawn_ladder(gd, 32, 88, 5, ladder_color::yellow);
    spawn_ladder(gd, 64, 88, 5, ladder_color::yellow);
    spawn_ladder(gd, 152, 88, 5, ladder_color::yellow);
    spawn_ladder(gd, 184, 88, 5, ladder_color::yellow);

    spawn_ladder(gd, 24, 128, 5, ladder_color::yellow);
    spawn_ladder(gd, 104, 128, 5, ladder_color::yellow);
    spawn_ladder(gd, 192, 128, 5, ladder_color::yellow);

    spawn_ladder(gd, 16, 168, 5, ladder_color::yellow);
    spawn_ladder(gd, 72, 168, 5, ladder_color::yellow);
    spawn_ladder(gd, 144, 168, 5, ladder_color::yellow);
    spawn_ladder(gd, 200, 168, 5, ladder_color::yellow);

    spawn_ladder(gd, 8, 208, 5, ladder_color::yellow);
    spawn_ladder(gd, 104, 208, 5, ladder_color::yellow);
    spawn_ladder(gd, 208, 208, 5, ladder_color::yellow);

    spawn_rivets(gd);

    spawn_kong(gd, {112, 72}, false);
    spawn_prop(gd, SID_CYAN_POLE, 71, 56);
    spawn_prop(gd, SID_CYAN_POLE, 151, 56);

    spawn_pauline(gd, {112, 37}, help_color::blue, true);
    spawn_jump_man(gd, {42, 232});

    spawn_collectables(gd);

    spawn_bonus_box(gd, {171, 40}, bonus_box_color::blue);

    gd.mob_spawn_clock_s = 0.f;
  }

  void reset(game_data& gd)
  {
    spawn_rivets(gd);
    spawn_collectables(gd);
    spawn_jump_man(gd, {42, 232});
    gd.mob_spawn_clock_s = 0.f;
  }

  void update(game_data& gd, float dt_s)
  {
    constexpr vector2f left_spawn_pos[4] {
      {8, 240},
      {14, 200},
      {23, 160},
      {31, 120},
    };
    constexpr vector2f right_spawn_pos[4] {
      {216, 240},
      {205, 200},
      {197, 160},
      {189, 120},
    };
    if(gd.mobs.size() < con::rivets_level_mob_count){
      gd.mob_spawn_clock_s += dt_s;
      if(gd.mob_spawn_clock_s > con::rivets_level_mob_spawn_interval_s[gd.difficulty]){
        const auto spawn_choice = rnd::uniformSignedInt(0, 3);
        if(gd.jump_man.center_px.x <= con::world_half_width_px)
          spawn_mob(gd, MOB_FIREFOX, right_spawn_pos[spawn_choice], look_direction::left);
        else
          spawn_mob(gd, MOB_FIREFOX, left_spawn_pos[spawn_choice], look_direction::right);
        gd.mob_spawn_clock_s = 0.f;
      }
    }
  }

  bool is_won(game_data& gd)
  {
    return gd.rivets.empty() && gd.jump_man.state_ != jump_man_state::dying;
  }

  void on_won(game_data& gd)
  {
    clear_static_world(gd);
    spawn_girder(gd, 0, 248, 28, girder_color::blue, FLAG_BOTTOM_GIRDER);

    spawn_girder(gd, 8, 208, 6, girder_color::blue);
    spawn_girder(gd, 168, 208, 6, girder_color::blue);

    spawn_girder(gd, 16, 168, 5, girder_color::blue);
    spawn_girder(gd, 168, 168, 5, girder_color::blue);

    spawn_girder(gd, 24, 128, 4, girder_color::blue);
    spawn_girder(gd, 168, 128, 4, girder_color::blue);

    spawn_girder(gd, 32, 88, 3, girder_color::blue);
    spawn_girder(gd, 168, 88, 3, girder_color::blue);

    spawn_ladder(gd, 32, 88, 5, ladder_color::yellow);
    spawn_ladder(gd, 184, 88, 5, ladder_color::yellow);

    spawn_ladder(gd, 24, 128, 5, ladder_color::yellow);
    spawn_ladder(gd, 192, 128, 5, ladder_color::yellow);

    spawn_ladder(gd, 16, 168, 5, ladder_color::yellow);
    spawn_ladder(gd, 200, 168, 5, ladder_color::yellow);

    spawn_ladder(gd, 8, 208, 5, ladder_color::yellow);
    spawn_ladder(gd, 208, 208, 5, ladder_color::yellow);
  }
}