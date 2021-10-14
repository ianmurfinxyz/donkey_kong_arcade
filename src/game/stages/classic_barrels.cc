#include "../world.hh"
#include "../constants.hh"

namespace stage::classic_barrels
{
  void setup(game_data& gd)
  {
    // flat ground girder.
    spawn_girder(gd, 0, 248, 14, girder_color::red, FLAG_BOTTOM_GIRDER);
    spawn_girder(gd, 112, 247, 2, girder_color::red, FLAG_BOTTOM_GIRDER);
    spawn_girder(gd, 128, 246, 2, girder_color::red, FLAG_BOTTOM_GIRDER);
    spawn_girder(gd, 144, 245, 2, girder_color::red, FLAG_BOTTOM_GIRDER);
    spawn_girder(gd, 160, 244, 2, girder_color::red, FLAG_BOTTOM_GIRDER);
    spawn_girder(gd, 176, 243, 2, girder_color::red, FLAG_BOTTOM_GIRDER);
    spawn_girder(gd, 192, 242, 2, girder_color::red, FLAG_BOTTOM_GIRDER);
    spawn_girder(gd, 208, 241, 2, girder_color::red, FLAG_BOTTOM_GIRDER);

    // flat top girder.
    spawn_girder(gd, 0, 84, 18, girder_color::red);

    // flat lady girder.
    spawn_girder(gd, 88, 56, 6, girder_color::red);

    // add the sloping platforms.
    struct platform {
      int left_px;
      int top_px;
      int width_girder_units;
      int unit_y_offset;
    };
    constexpr std::array<platform, 5> platforms {
      platform{0, 208, 13, 1},
      platform{16, 187, 13, -1},
      platform{0, 142, 13, 1},
      platform{16, 121, 13, -1},
      platform{144, 85, 4, 1}
    };
    for(auto& p : platforms){
      for(auto i = 0; i < p.width_girder_units; ++i){
        int left_px = p.left_px + (con::girder_unit_width_px * i);
        int top_px = p.top_px + (p.unit_y_offset * i);
        spawn_girder(gd, left_px, top_px, 2, girder_color::red);
      }
    }

    // static ladders.
    spawn_ladder(gd, 184, 219, 3, ladder_color::cyan);
    spawn_ladder(gd, 32, 186, 3, ladder_color::cyan);
    spawn_ladder(gd, 96, 182, 4, ladder_color::cyan);
    spawn_ladder(gd, 112, 149, 4, ladder_color::cyan);
    spawn_ladder(gd, 72, 118, 4, ladder_color::cyan);
    spawn_ladder(gd, 184, 153, 3, ladder_color::cyan);
    spawn_ladder(gd, 32, 120, 3, ladder_color::cyan);
    spawn_ladder(gd, 184, 87, 3, ladder_color::cyan);
    spawn_ladder(gd, 128, 56, 4, ladder_color::cyan);

    // topless ladders.
    spawn_ladder(gd, 80, 220, 1, ladder_color::cyan, FLAG_CLIMBABLE_NO_TOP);
    spawn_ladder(gd, 80, 240, 1, ladder_color::cyan, FLAG_CLIMBABLE_NO_TOP);
    spawn_ladder(gd, 64, 176, 1, ladder_color::cyan, FLAG_CLIMBABLE_NO_TOP);
    spawn_ladder(gd, 64, 152, 1, ladder_color::cyan, FLAG_CLIMBABLE_NO_TOP);
    spawn_ladder(gd, 168, 144, 1, ladder_color::cyan, FLAG_CLIMBABLE_NO_TOP);
    spawn_ladder(gd, 168, 112, 2, ladder_color::cyan, FLAG_CLIMBABLE_NO_TOP);
    spawn_ladder(gd, 88, 104, 2, ladder_color::cyan, FLAG_CLIMBABLE_NO_TOP);
    spawn_ladder(gd, 88, 88, 1, ladder_color::cyan, FLAG_CLIMBABLE_NO_TOP);

    // exit ladders are props so nothing can climb them.
    for(auto i = 0; i < 7; ++i){
      spawn_prop(gd, SID_LADDER_CYAN, 64, 32 + (i * 8));
      spawn_prop(gd, SID_LADDER_CYAN, 80, 32 + (i * 8));
    }

    spawn_pauline(gd, {96, 45}, help_color::cyan);
    spawn_prop(gd, SID_BARREL_STACK, 0, 51, false);
    spawn_oil_can(gd, 16, 232);
    spawn_jump_man(gd, {20, 240});
    spawn_barrel_kong(gd, {43, 68}, look_direction::right, true, true, true);
    spawn_hammer(gd, vector2f{17, 98}, hammer_number::hammer_0);
    spawn_hammer(gd, vector2f{165, 195}, hammer_number::hammer_1);
    spawn_bonus_box(gd, {171, 40}, bonus_box_color::pink);

    gd.mob_spawn_clock_s = 0.f;
  }

  void reset(game_data& gd)
  {
    spawn_hammer(gd, vector2f{17, 98}, hammer_number::hammer_0);
    spawn_hammer(gd, vector2f{165, 195}, hammer_number::hammer_1);
    spawn_jump_man(gd, {20, 240});
    spawn_barrel_kong(gd, {47, 68}, look_direction::right, true, true, true);
    gd.oil_can_.state = oil_can_state::not_lit;
    gd.mob_spawn_clock_s = 0.f;
  }

  void update(game_data& gd, float dt_s)
  {
    // note: this logic is ultimately flawed since it doesn't take into account how many blue barrels are already
    // spawned, thus if you spawn barrels at too high a rate then you can end up with more mobs than the limit. But
    // it doesn't seem to be an issue in my play testing with the current spawn intervals.
    if(gd.mobs.size() < con::barrel_level_mob_count[gd.difficulty]){
      gd.mob_spawn_clock_s += dt_s;
      if(gd.mob_spawn_clock_s > con::barrel_level_blue_spawn_interval_s[gd.difficulty]){
        gd.barrel_kong.flags |= FLAG_BARREL_KONG_FORCE_BLUE_BARREL;
        gd.mob_spawn_clock_s = 0.f;
      }
    }
  }

  bool is_won(game_data& gd)
  {
    static constexpr aabb win_box { 121, 143, 48, 30};
    return is_collision(gd.jump_man.body_box, win_box);
  }
}