#include "../world.hh"
#include "../constants.hh"
#include "../../core/rand.hh"

namespace stage::classic_factory
{
  void setup(game_data& gd)
  {
    // flat ground girder.
    spawn_girder(gd, 0, 248, 28, girder_color::orange, FLAG_BOTTOM_GIRDER);
    // flat lady girder.
    spawn_girder(gd, 88, 56, 6, girder_color::orange);
    // middle girders.
    spawn_girder(gd, 8, 168, 5, girder_color::orange);
    spawn_girder(gd, 64, 168, 11, girder_color::orange);
    spawn_girder(gd, 168, 168, 5, girder_color::orange);

    spawn_ladder(gd, 24, 208, 5, ladder_color::white);
    spawn_ladder(gd, 80, 208, 5, ladder_color::white);
    spawn_ladder(gd, 136, 208, 5, ladder_color::white);
    spawn_ladder(gd, 192, 208, 5, ladder_color::white);
    spawn_ladder(gd, 64, 168, 5, ladder_color::white);
    spawn_ladder(gd, 144, 168, 5, ladder_color::white);
    spawn_ladder(gd, 24, 128, 5, ladder_color::white);
    spawn_ladder(gd, 80, 128, 5, ladder_color::white);
    spawn_ladder(gd, 136, 128, 5, ladder_color::white);
    spawn_ladder(gd, 192, 128, 5, ladder_color::white);
    spawn_ladder(gd, 128, 56, 4, ladder_color::white);

    spawn_moving_ladder(gd, 200, 88, 2, 3, ladder_color::white);
    spawn_moving_ladder(gd, 16, 88, 2, 3, ladder_color::white);

    // full conveyors
    spawn_conveyor(gd, -10, con::world_width_px + 10, 88, 2, 211, 8, 26, conveyor_color::orange);

    // bottom conveyor with tubs that go fully left to right. This conveyor has enough extra length off-screen in both
    // directions to lay out 3 tubs with 47px between them (141 = 3*tub_width + 2*47).
    spawn_conveyor(gd, -141, con::world_width_px + 141, 208, 2, 211, 8, 26, conveyor_color::orange);

    // half conveyors
    spawn_conveyor(gd, -141, 105, 128, -100, 95, 0, 12, conveyor_color::orange);
    spawn_conveyor(gd, 119, con::world_width_px + 141, 128, 119, -100, 128, 12, conveyor_color::orange);

    // exit ladders are props so nothing can climb them.
    for(auto i = 0; i < 7; ++i){
      spawn_prop(gd, SID_LADDER_WHITE, 64, 32 + (i * 8));
      spawn_prop(gd, SID_LADDER_WHITE, 80, 32 + (i * 8));
    }

    spawn_prop(gd, SID_WIRE_FENCE, 96, 144);
    spawn_oil_can(gd, 104, 128);
    light_oil_can(gd.oil_can_);
    spawn_kong(gd, {50, 72}, true);
    spawn_pauline(gd, {96, 45}, help_color::white);
    spawn_jump_man(gd, {20, 240});
    spawn_collectable(gd, vector2f{196, 152}, COLLECTABLE_PARASOL);
    spawn_collectable(gd, vector2f{61, 159}, COLLECTABLE_HAT);
    spawn_collectable(gd, vector2f{119, 238}, COLLECTABLE_BAG);
    spawn_hammer(gd, vector2f{104, 178}, hammer_number::hammer_0);
    spawn_hammer(gd, vector2f{16, 139}, hammer_number::hammer_1);
    spawn_bonus_box(gd, {171, 40}, bonus_box_color::orange);
  }

  void reset(game_data& gd)
  {
    spawn_collectable(gd, vector2f{196, 152}, COLLECTABLE_PARASOL);
    spawn_collectable(gd, vector2f{61, 159}, COLLECTABLE_HAT);
    spawn_collectable(gd, vector2f{119, 238}, COLLECTABLE_BAG);
    spawn_hammer(gd, vector2f{104, 178}, hammer_number::hammer_0);
    spawn_hammer(gd, vector2f{16, 139}, hammer_number::hammer_1);
    spawn_jump_man(gd, {20, 240});
    spawn_tub(gd, 45, 112, TUB_CEMENT);
  }

  void update(game_data& gd, float dt_s)
  {
    if(gd.mobs.size() < con::factory_level_mob_count[gd.difficulty - 1]){
      gd.mob_spawn_clock_s += dt_s;
      if(gd.mob_spawn_clock_s > con::mob_min_spawn_interval_s){
        auto launch_dir = rnd::uniformSignedInt(0, 1) == 0 ? -1 : 1;
        spawn_mob(gd, MOB_FIREBALL, vector2f{112, 122}, look_direction::left, vector2f{
          launch_dir * con::mob_launch_speed_x_px_per_s,
          con::mob_launch_speed_y_px_per_s
        });
        gd.mob_spawn_clock_s = 0.f;
        light_oil_can(gd.oil_can_);
        if(gd.jump_man.flags & FLAG_JM_HAMMERING){
          set_mob_vulnerability(gd, true);
        }
      }
    }

    if(gd.tubs.size() < con::factory_level_tub_count){
      // need to count the tubs on each conveyor so we know where to spawn the new ones.
      // This is a rather inelegant way to do this since I didn't have the foresight to
      // make this data easily availible :(
      constexpr auto tub_seperation_px = 62;
      auto top_left_tubs_count = 0;
      auto top_right_tubs_count = 0;
      auto bottom_tubs_count = 0;
      auto bottom_tub_min_x = 9999.f;
      auto bottom_tub_max_x = -9999.f;
      for(const auto& tub : gd.tubs){
        if(tub.position_px.y < 136){
          if(tub.position_px.x < 112){
            ++top_left_tubs_count;
          }
          else{
            ++top_right_tubs_count;
          }
        }
        else{
          ++bottom_tubs_count;
          bottom_tub_min_x = std::min(bottom_tub_min_x, tub.position_px.x);
          bottom_tub_max_x = std::max(bottom_tub_max_x, tub.position_px.x);
        }
      }
      auto spawn_count = 0;
      if(top_left_tubs_count == 0){
        while(top_left_tubs_count < con::factory_level_top_conveyors_tub_count){
          spawn_tub(gd, -16 + (spawn_count * -tub_seperation_px), 120, TUB_CEMENT);
          ++top_left_tubs_count;
          ++spawn_count;
        }
      }
      spawn_count = 0;
      if(top_right_tubs_count == 0){
        while(top_right_tubs_count < con::factory_level_top_conveyors_tub_count){
          spawn_tub(gd, con::world_width_px + (spawn_count * tub_seperation_px), 120, TUB_CEMENT);
          ++top_right_tubs_count;
          ++spawn_count;
        }
      }
      if(bottom_tubs_count < con::factory_level_bottom_conveyor_tub_count){
        const auto bottom_conveyor = std::find_if(gd.conveyors.begin(), gd.conveyors.end(), [](const auto& conveyor){
          return conveyor.surface_px == 208;
        });
        assert(bottom_conveyor != gd.conveyors.end());
        if(bottom_conveyor->belt_dir == look_direction::left && (bottom_tub_max_x < con::world_width_px - tub_seperation_px)){
          spawn_count = 0;
          while(spawn_count < 3 && bottom_tubs_count < con::factory_level_bottom_conveyor_tub_count){
            spawn_tub(gd, con::world_width_px + (spawn_count * tub_seperation_px), 200, TUB_CEMENT);
            ++spawn_count;
            ++bottom_tubs_count;
          }
        }
        if(bottom_conveyor->belt_dir == look_direction::right && (bottom_tub_min_x > tub_seperation_px - con::tub_width_px[TUB_CEMENT])){
          spawn_count = 0;
          while(spawn_count < 3 && bottom_tubs_count < con::factory_level_bottom_conveyor_tub_count){
            spawn_tub(gd, -con::tub_width_px[TUB_CEMENT] + (spawn_count * -tub_seperation_px), 200, TUB_CEMENT);
            ++spawn_count;
            ++bottom_tubs_count;
          }
        }
      }
    }
  }

  bool is_won(game_data& gd)
  {
    static constexpr aabb win_box { 121, 143, 48, 30};
    return is_collision(gd.jump_man.body_box, win_box);
  }
}