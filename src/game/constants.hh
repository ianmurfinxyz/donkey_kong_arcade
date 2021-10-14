#ifndef DONKEY_KONG_1981_CONSTANTS_HH
#define DONKEY_KONG_1981_CONSTANTS_HH

#include <cmath>
#include "types.hh"

namespace con
{
  /** width and height of (square) world blocks. */
  constexpr int block_size_px = 8;
  constexpr int block_half_size_px = block_size_px / 2;

  /** The min width of a girder in pixels (see the girder sprite). */
  constexpr int girder_unit_width_px = 16;

  constexpr int world_width_px = 224;
  constexpr int world_height_px = 256;
  constexpr int world_half_width_px = world_width_px / 2;

  constexpr int world_width_blocks = 224 / block_size_px;
  constexpr int world_height_blocks = 256 / block_size_px;

  constexpr float death_freeze_duration_s = 1.f;

  /** The thickness of debug drawn rects (borders) and lines etc. */
  constexpr int debug_line_thickness = 1;

  constexpr int starting_life_count = 3;
  constexpr int score_for_extra_life = 10000;

  /** offset from jump man's center to spawn score popups. */
  constexpr vector2f popup_offset_px = {0.f, 10.f};
  constexpr float popup_duration_s = 1.f;

  constexpr int min_difficulty = 1;
  constexpr int max_difficulty = 5;
  constexpr int difficulty_delta_s = 33.f;

  /** Time between 100 point reduction in stage bonus. */
  constexpr float bonus_drop_period_s = 1.75;
  constexpr int bonus_drop = 100;
  constexpr vector2f bonus_box_text_offset {5, 8};

  constexpr float game_over_duration_s = 4.f;
  constexpr float intermission_duration_s = 2.6f;

  /** some of the sound assets are a little out of balance :( */
  constexpr float death_sound_gain = 0.6f;
  constexpr float smash_sound_gain = 0.6f;
  constexpr float spring_fall_gain = 0.1f;
  constexpr float spring_bounce_gain = 0.3f;

  constexpr int barrel_level_mob_count[max_difficulty] {3, 3, 4, 5, 5};
  constexpr float barrel_level_blue_spawn_interval_s[max_difficulty] {
    20.f, 20.f, 20.f, 15.f, 12.f
  };

  constexpr int factory_level_mob_count[max_difficulty] {3, 3, 4, 5, 5};
  constexpr float factory_level_top_conveyors_tub_count = 3;
  constexpr float factory_level_bottom_conveyor_tub_count = 6;
  constexpr float factory_level_tub_count =
    (factory_level_top_conveyors_tub_count * 2) + factory_level_bottom_conveyor_tub_count;

  constexpr int rivets_level_mob_count = 5;
  constexpr float rivets_level_mob_spawn_interval_s[max_difficulty] {
    2.0f, 1.6f, 1.2f, 1.0f, 0.6f
  };

  constexpr float jump_man_gravity_px_per_s2 = 300.f;
  constexpr float jump_man_run_speed_px_per_s = 50.f;
  constexpr float jump_man_jump_speed_px_per_s = 96.f;
  constexpr float jump_man_jump_x_speed_scale = 0.7f;
  constexpr float jump_man_climb_speed_px_per_s = 25.f;
  constexpr float jump_man_splat_height_px = 20.f;
  constexpr float jump_man_width_px = 12.f;
  constexpr float jump_man_half_width_px = jump_man_width_px / 2.f;
  constexpr float jump_man_height_px = 16.f;
  constexpr float jump_man_half_height_px = jump_man_height_px / 2.f;
  constexpr float jump_man_feet_box_width_px = 6.f;
  constexpr float jump_man_feet_box_half_width_px = jump_man_feet_box_width_px / 2.f;
  constexpr float jump_man_feet_box_height_px = 8;
  constexpr float jump_man_feet_box_half_height_px = jump_man_feet_box_height_px / 2.f;
  constexpr float jump_man_leap_box_width_px = 10;
  constexpr float jump_man_leap_box_half_width_px = jump_man_leap_box_width_px / 2.f;
  constexpr float jump_man_leap_wide_box_width_px = 38;
  constexpr float jump_man_leap_wide_box_half_width_px = jump_man_leap_wide_box_width_px / 2;
  constexpr float jump_man_leap_box_height_px = 10;
  constexpr float jump_man_body_box_margin_px = 3;
  constexpr float jump_man_run_frame_delta_s = 0.07f;
  constexpr float jump_man_run_hammering_frame_delta_s = 0.04f;
  constexpr float jump_man_hammer_swing_delta_s = jump_man_run_hammering_frame_delta_s * 4;
  constexpr float jump_man_hammer_total_duration_s = 12.f;
  constexpr float jump_man_hammer_expiring_duration_s = 6.f;
  constexpr float jump_man_hammer_up_box_offset_px = 14.f;
  constexpr float jump_man_hammer_down_box_offset_px = 17.f;
  constexpr float jump_man_hammer_head_width_px = 8.f;
  constexpr float jump_man_hammer_head_height_px = 6.f;
  constexpr float jump_man_hammer_head_half_width_px = jump_man_hammer_head_width_px / 2;
  constexpr float jump_man_hammer_head_half_height_px = jump_man_hammer_head_height_px / 2;

  constexpr float kong_body_width_px = 28;
  constexpr float kong_body_height_px = 36;
  constexpr float kong_body_half_width_px = kong_body_width_px / 2;
  constexpr float kong_body_half_height_px = kong_body_height_px / 2;
  constexpr float kong_beat_chest_interval_s = 3.f;
  constexpr float kong_screen_margin_px = 40.f;

  constexpr int orangutan_height_px = 32;

  // The frequency of barrel rolling for each difficulty.
  constexpr float barrel_kong_state_duration_s[max_difficulty] = {
    0.6f, 0.6f, 0.5f, 0.5f, 0.4f
  };
  // the chance kong will choose to roll a drop wild rather than another wild.
  constexpr int barrel_kong_drop_wild_chance = 10;
  // the wild kong chooses based on the current difficulty.
  constexpr wild_type barrel_kong_wild_choice[max_difficulty] = {
    wild_type::WILD_CALM,
    wild_type::WILD_CALM,
    wild_type::WILD_CRAZY,
    wild_type::WILD_CRAZY,
    wild_type::WILD_HOMING,
  };
  constexpr vector2f barrel_kong_rolling_barrel_offset = {-34, 5};
  constexpr vector2f barrel_kong_wild_barrel_offset = {-8, 3};
  constexpr int barrel_kong_wild_roll_chance = 16; // 1 in 16 chance.
  // chance kong fist pumps after rolling a barrel.
  constexpr int barrel_kong_fist_pump_chance = 3;

  constexpr int rivet_width_px = 8;
  constexpr float rivet_hit_box_width_px = 3;
  constexpr float rivet_hit_box_height_px = 4;
  constexpr float rivet_hit_box_margin_px = 3;
  constexpr float rivet_hit_jump_offset_y_px = -15;

  constexpr int conveyor_reverse_chance = 20;
  constexpr float conveyor_tick_interval_s = 0.5f;
  constexpr float conveyor_belt_speed_px_per_s = 20.f;

  constexpr float moving_ladder_speed_px_per_s = 10.f;
  constexpr float wide_ladder_block_width_px = 10.f;
  constexpr float moving_ladder_rest_duration_s = 1.f;

  constexpr float elevator_speed_px_per_s = 30.f;
  constexpr float elevator_rest_duration_s = 1.f;

  constexpr float spring_speed_px_per_s = 100.f;
  constexpr float spring_body_box_width_px = 6;
  constexpr float spring_body_box_height_px = 8;
  constexpr float spring_body_box_half_width_px = spring_body_box_width_px / 2;
  constexpr float spring_body_box_half_height_px = spring_body_box_height_px / 2;
  constexpr float spring_max_path_shift_x_px = 5.f;

  constexpr int barrel_bounce_count = 3;
  constexpr float barrel_gravity_px_per_s2 = 600.f;
  constexpr float barrel_speed_px_per_s = 70.f;
  constexpr float barrel_bounce_speed_px_per_s = 60.f;
  constexpr float barrel_on_fall_speed_reduction = 0.5f;
  constexpr float barrel_ladder_speed_px_per_s = 70.f;
  constexpr float barrel_width_px = 12;
  constexpr float barrel_height_px = 10;
  constexpr float barrel_half_width_px = barrel_width_px / 2;
  constexpr float barrel_half_height_px = barrel_height_px / 2;
  constexpr float barrel_wild_width_px = 16;
  constexpr float barrel_wild_height_px = 10;
  constexpr float barrel_wild_half_width_px = barrel_wild_width_px / 2;
  constexpr float barrel_wild_half_height_px = barrel_wild_height_px / 2;
  constexpr float barrel_box_width_px = 8;
  constexpr float barrel_box_height_px = 8;
  constexpr float barrel_ladder_box_width_px = 4;
  constexpr float barrel_ladder_box_height_px = 10;
  constexpr float barrel_min_fall_height_to_reduce_speed = 3.f;
  constexpr float barrel_min_fall_height_to_reverse = 10.f;
  constexpr float barrel_min_ladder_fall_px = 20.f;

  constexpr float wild_rebound_duration_s = 0.01f;
  constexpr float wild_slow_speed_px_per_s = 80.f;
  constexpr float wild_fast_speed_px_per_s = 120.f;
  constexpr float wild_acceleration_px_per_s2 = 130.f;
  constexpr float wild_speed_start_percentage = 0.2f;
  constexpr float wild_speed_bounce_reduction = 0.2f;
  constexpr int wild_drop_hit_chance = 10;
  constexpr float wild_body_margin_px = 3;

  constexpr float wild_speeds[WILD_TYPE_COUNT] = {
    wild_fast_speed_px_per_s, // drop
    wild_slow_speed_px_per_s, // calm
    wild_fast_speed_px_per_s, // crazy
    wild_slow_speed_px_per_s  // homing
  };

  // The margins are regions on the left and right side of the screen that are
  // used in wild steering. If a wild is in the left margin it cannot choose a
  // rebound direction that will take it further left, and visa versa. This is
  // to prevent the wild bounding off-screen.
  constexpr int wild_margin_width_px = 60;
  constexpr int wild_left_margin_x = wild_margin_width_px;
  constexpr int wild_right_margin_x = world_width_px - wild_margin_width_px;

  // used when selecting which direction a homing wild will rebound. The method
  // of selection is as follows. The space around the barrel center is divided
  // into regions like:
  //  o---> x
  //  | | R0 | R1  | R2  | R3  |  R4 |  R5 |    B = barrel position
  //  v |    |     |     B     |     |     |    J = jump man position
  //  y |    |     |     |     |     |     |    RN = region number
  //    |    |     |     |  J  |     |     |
  // The rebound direction is then selected based on what region jump man is
  // in. The above arrangement, with jump man in R3, will select first right-ward
  // rebound direction. If jump man was in R4 it would select the 2nd right-ward
  // direction etc. This method allows the wild to home in on jump man by selecting
  // the direction which will move the wild toward jump man's current position.
  constexpr int wild_homing_region_width_px = 30;

  // note: rebound directions are ordered, with ascending indices pointing
  // increasingly rightward (+x-axis). All vectors are normalised. The set
  // is also mirrored with the upper half being equal to the lower half
  // mirrored about the line x=0. The set is intentionally of odd size,
  // with the median pointing directly down (+y-axis).
  constexpr int wide_rebound_dirs_count = 9;
  constexpr int wide_rebound_down = static_cast<int>(wide_rebound_dirs_count / 2);
  constexpr float pi = 3.141592653589793238462643383279502884;
  constexpr float wdT = (0.8f * pi) / wide_rebound_dirs_count;
  extern const vector2f wide_rebound_dirs[wide_rebound_dirs_count];
  constexpr int narrow_rebound_dirs_count = 5;
  constexpr int narrow_rebound_down = static_cast<int>(narrow_rebound_dirs_count / 2);
  constexpr float ndT = (0.4f * pi) / narrow_rebound_dirs_count;
  extern const vector2f narrow_rebound_dirs[narrow_rebound_dirs_count];

  /** The vector offset from pauline's center to the top-left of the help sprite. */
  constexpr vector2f pauline_help_offset = {8, -13};
  constexpr float pauline_help_interval_s = 8.f;
  constexpr float pauline_help_sign_width_px = 24;

  /** Chance pauline will reverse direction and look other way. */
  constexpr int pauline_flip_chance = 200;

  constexpr int oil_can_width_px = 16;
  constexpr int oil_can_half_width_px = oil_can_width_px / 2;
  constexpr int oil_can_height_px = 16;
  constexpr int oil_can_margin_px = 4;
  constexpr int oil_can_flames_width_px = 15;
  constexpr int oil_can_flames_height_px = 16;
  constexpr float oil_can_mob_forge_duration_s = 2.f;

  constexpr float tub_gravity_px_per_s2 = 200.f;

  constexpr int tub_width_px[TUB_COUNT] = {
    16, // cement
  };
  constexpr int tub_height_px[TUB_COUNT] = {
    8, // cement
  };
  constexpr int tub_margin_px[TUB_COUNT] = {
    2, // cement
  };

  constexpr float mob_ai_tick_interval_s = 1.f / 20.f;
  constexpr float mob_climb_speed_px_per_s = 30.f;
  constexpr float mob_roam_speed_px_per_s[max_difficulty]{
    20.f, 30.f, 30.f, 40.f, 40.f
  };
  constexpr float mob_body_margin_px = 3.f;
  constexpr float mob_ladder_box_width_px = 4.f;
  constexpr float mob_ladder_box_half_width_px = mob_ladder_box_width_px / 2;
  constexpr float mob_ladder_box_height_px = 6.f;
  constexpr float mob_feeler_box_width_px = 1.f;
  constexpr float mob_feeler_extension_px = 6.f;
  constexpr float mob_feeler_range_px = -2.f;
  constexpr float mob_gravity_px_per_s2 = 200.f;
  constexpr float mob_bounce_lo_px = 0.f;
  constexpr float mob_bounce_hi_px = 3.f;
  constexpr float mob_bounce_speed_px_per_s = 0.2f;

  /** Used by levels when spawning mobs. */
  constexpr float mob_min_spawn_interval_s = 1.5f;
  constexpr float mob_launch_speed_x_px_per_s = mob_roam_speed_px_per_s[1];
  constexpr float mob_launch_speed_y_px_per_s = -60.f;

  // chances are 1 in 'chance', so if chance==10, then 1 in 10 chance.
  constexpr int mob_climb_chance = 50;
  constexpr int mob_reverse_chance = 40;

  constexpr float mob_width_px[MOB_COUNT] = {
    14, // fireball
    16, // firefox
  };
  constexpr float mob_half_width_px[MOB_COUNT] = {
    mob_width_px[0] / 2,
    mob_width_px[1] / 2,
  };
  constexpr float mob_height_px[MOB_COUNT] = {
    14, // fireball
    14, // firefox
  };
  constexpr float mob_half_height_px[MOB_COUNT] = {
    mob_height_px[0] / 2,
    mob_height_px[1] / 2,
  };

  constexpr int pickup_collect_margin_px = 2.f;
  constexpr int hammer_pickup_width_px = 9;
  constexpr int hammer_pickup_height_px = 10;
  constexpr int collectable_width_px[COLLECTABLE_COUNT] = {
    16, // parasol
    15,  // hat
    9,  // bag
  };
  constexpr int collectable_height_px[COLLECTABLE_COUNT] = {
    15, // parasol
    8,  // hat
    9,  // bag
  };
}

#endif //DONKEY_KONG_1981_CONSTANTS_HH
