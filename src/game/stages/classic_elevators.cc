#include "../world.hh"
#include "../constants.hh"

namespace stage::classic_elevators
{
  void setup(game_data& gd)
  {
    // flat ground girder.
    spawn_girder(gd, 0, 248, 28, girder_color::red, FLAG_BOTTOM_GIRDER);
    // flat top girder.
    spawn_girder(gd, 0, 88, 21, girder_color::red);
    // flat lady girder.
    spawn_girder(gd, 88, 56, 6, girder_color::red);

    // middle girders.
    spawn_girder(gd, 0, 232, 3, girder_color::red);
    spawn_girder(gd, 0, 192, 3, girder_color::red);
    spawn_girder(gd, 0, 136, 3, girder_color::red);
    spawn_girder(gd, 64, 208, 3, girder_color::red);
    spawn_girder(gd, 56, 136, 4, girder_color::red);
    spawn_girder(gd, 120, 232, 3, girder_color::red);
    spawn_girder(gd, 128, 128, 3, girder_color::red);
    spawn_girder(gd, 136, 160, 2, girder_color::red);
    spawn_girder(gd, 152, 224, 2, girder_color::red);
    spawn_girder(gd, 176, 216, 2, girder_color::red);
    spawn_girder(gd, 160, 168, 2, girder_color::red);
    spawn_girder(gd, 184, 176, 2, girder_color::red);
    spawn_girder(gd, 200, 208, 3, girder_color::red);
    spawn_girder(gd, 208, 184, 2, girder_color::red);
    spawn_girder(gd, 184, 144, 5, girder_color::red);
    spawn_girder(gd, 160, 120, 2, girder_color::red);
    spawn_girder(gd, 184, 112, 2, girder_color::red);
    spawn_girder(gd, 208, 104, 2, girder_color::red);

    spawn_ladder(gd, 8, 192, 5, ladder_color::cyan);
    spawn_ladder(gd, 16, 136, 7, ladder_color::cyan);
    spawn_ladder(gd, 64, 136, 9, ladder_color::cyan);
    spawn_ladder(gd, 80, 136, 9, ladder_color::cyan);
    spawn_ladder(gd, 136, 128, 4, ladder_color::cyan);
    spawn_ladder(gd, 184, 144, 4, ladder_color::cyan);
    spawn_ladder(gd, 160, 88, 4, ladder_color::cyan);
    spawn_ladder(gd, 160, 88, 4, ladder_color::cyan);
    spawn_ladder(gd, 128, 56, 4, ladder_color::cyan);
    spawn_ladder(gd, 208, 184, 3, ladder_color::cyan);
    spawn_ladder(gd, 208, 104, 5, ladder_color::cyan);

    // left-most upward elevators
    spawn_elevator(gd, 32, 248, 2, -152, 0.f, girder_color::red, elevator_dir::vertical, elevator_mode::repeat);
    spawn_elevator(gd, 32, 248, 2, -152, 0.33f, girder_color::red, elevator_dir::vertical, elevator_mode::repeat);
    spawn_elevator(gd, 32, 248, 2, -152, 0.66f, girder_color::red, elevator_dir::vertical, elevator_mode::repeat);

    // right-most downward elevators
    spawn_elevator(gd, 96, 96, 2, 152, 0.f, girder_color::red, elevator_dir::vertical, elevator_mode::repeat);
    spawn_elevator(gd, 96, 96, 2, 152, 0.33f, girder_color::red, elevator_dir::vertical, elevator_mode::repeat);
    spawn_elevator(gd, 96, 96, 2, 152, 0.66f, girder_color::red, elevator_dir::vertical, elevator_mode::repeat);

    // exit ladders are props so nothing can climb them.
    for(auto i = 0; i < 7; ++i){
      spawn_prop(gd, SID_LADDER_CYAN, 64, 32 + (i * 8));
      spawn_prop(gd, SID_LADDER_CYAN, 80, 32 + (i * 8));
    }

    spawn_prop(gd, SID_RED_POLE, 39, 96);
    spawn_prop(gd, SID_RED_POLE, 39, 128);
    spawn_prop(gd, SID_RED_POLE, 39, 160);
    spawn_prop(gd, SID_RED_POLE, 39, 192);
    spawn_prop(gd, SID_RED_POLE, 39, 224);

    spawn_prop(gd, SID_RED_POLE, 103, 96);
    spawn_prop(gd, SID_RED_POLE, 103, 128);
    spawn_prop(gd, SID_RED_POLE, 103, 160);
    spawn_prop(gd, SID_RED_POLE, 103, 192);
    spawn_prop(gd, SID_RED_POLE, 103, 224);

    spawn_prop(gd, SID_ELEVATOR_BOX, 32, 88, false);
    spawn_prop(gd, SID_ELEVATOR_BOX, 96, 88, false);
    spawn_prop(gd, SID_ELEVATOR_BOX, 32, 239, false, PROP_FLIP_Y);
    spawn_prop(gd, SID_ELEVATOR_BOX, 96, 239, false, PROP_FLIP_Y);

    spawn_pauline(gd, {96, 45}, help_color::cyan);
    spawn_kong(gd, {44, 72}, false);
    spawn_collectable(gd, vector2f{4, 120}, COLLECTABLE_PARASOL);
    spawn_collectable(gd, vector2f{69, 199}, COLLECTABLE_HAT);
    spawn_collectable(gd, vector2f{207, 94}, COLLECTABLE_BAG);
    spawn_mob(gd, MOB_FIREBALL, vector2f{211, 87}, look_direction::right);
    spawn_mob(gd, MOB_FIREBALL, vector2f{69, 120}, look_direction::right);
    spawn_jump_man(gd, {2, 215});
    spawn_bonus_box(gd, {171, 40}, bonus_box_color::pink);
  }

  void reset(game_data& gd)
  {
    spawn_collectable(gd, vector2f{4, 120}, COLLECTABLE_PARASOL);
    spawn_collectable(gd, vector2f{69, 199}, COLLECTABLE_HAT);
    spawn_collectable(gd, vector2f{207, 94}, COLLECTABLE_BAG);
    spawn_mob(gd, MOB_FIREBALL, vector2f{211, 87}, look_direction::right);
    spawn_mob(gd, MOB_FIREBALL, vector2f{69, 120}, look_direction::right);
    spawn_jump_man(gd, {2, 215});
  }

  void update(game_data& gd, float dt_s)
  {
    static const path_data spring_path {
      {{-24, 83}, 0},
      {{-20, 74}, 0},
      {{-14, 67}, FLAG_OPEN_SPRING},
      {{1, 62}, 0},
      {{16, 67}, FLAG_CLOSE_SPRING},
      {{22, 74}, 0},
      {{26, 83}, 0},
      {{30, 74}, 0},
      {{36, 67}, FLAG_OPEN_SPRING},
      {{51, 62}, 0},
      {{66, 67}, FLAG_CLOSE_SPRING},
      {{72, 74}, 0},
      {{76, 83}, 0},
      {{81, 74}, 0},
      {{87, 67}, FLAG_OPEN_SPRING},
      {{103, 62}, 0},
      {{118, 67}, FLAG_CLOSE_SPRING},
      {{124, 74}, 0},
      {{128, 83}, 0},
      {{132, 74}, 0},
      {{138, 67}, FLAG_OPEN_SPRING},
      {{154, 62}, 0},
      {{169, 67}, FLAG_CLOSE_SPRING},
      {{175, 74}, 0},
      {{179, 83}, FLAG_OPEN_SPRING | FLAG_FALL_SPRING},
      {{179, 261}, 0},
    };
    static float spawn_clock = 10.f;
    if(gd.springs.size() < 3){
      spawn_clock += dt_s;
      if(spawn_clock > 1.5f){
        spawn_spring(gd, &spring_path);
        spawn_clock = 0.f;
      }
    }
  }

  bool is_won(game_data& gd)
  {
    static constexpr aabb win_box { 121, 143, 48, 30};
    return is_collision(gd.jump_man.body_box, win_box);
  }
}