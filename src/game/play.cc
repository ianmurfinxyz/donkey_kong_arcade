#include <allegro5/allegro_primitives.h>
#include <tuple>
#include <sstream>
#include <iomanip>
#include "types.hh"
#include "constants.hh"
#include "world.hh"
#include "../core/input.hh"
#include "../core/global.hh"
#include "../core/rand.hh"
#include "../assets/animations.hh"
#include "../assets/sounds.hh"
#include "../assets/fonts.hh"

namespace {
  snd_play_id pid_music;
  snd_play_id pid_run;
  snd_play_id pid_countdown;
}

void stop_music(){stop_sound(pid_music);}
void play_music(music_track track)
{
  stop_music();
  sound_id snd{};
  switch(track){
    case music_track::one: {snd = SND_MUSIC_1; break;}
    case music_track::two: {snd = SND_MUSIC_2; break;}
    case music_track::hammer: {snd = SND_MUSIC_HAMMER; break;}
    default: assert(0);
  }
  pid_music = play_sound(snd, 1.f, true);
}

void stop_run_sound(){stop_sound(pid_run);}
void start_run_sound(){stop_run_sound(); pid_run = play_sound(SND_RUN, 1.f, true);}
void stop_climb_sound(){stop_sound(pid_run);}
void start_climb_sound(){stop_climb_sound(); pid_run = play_sound(SND_RUN, 0.8f, true);}
void stop_countdown_sound(){stop_sound(pid_countdown);}
void start_countdown_sound(){stop_run_sound(); pid_countdown = play_sound(SND_COUNTDOWN, 1.f, true);}

void update_jump_man_feet_box(jump_man_data& jm)
{
  jm.feet_box.left = jm.center_px.x - con::jump_man_feet_box_half_width_px;
  jm.feet_box.right = jm.center_px.x + con::jump_man_feet_box_half_width_px;
  jm.feet_box.bottom = (jm.center_px.y + con::jump_man_half_height_px) - con::jump_man_feet_box_half_height_px;
  jm.feet_box.top = (jm.center_px.y + con::jump_man_half_height_px) + con::jump_man_feet_box_half_height_px;
}

void update_jump_man_body_box(jump_man_data& jm)
{
  jm.body_box.left = (jm.center_px.x - con::jump_man_half_width_px) + con::jump_man_body_box_margin_px;
  jm.body_box.right = (jm.center_px.x + con::jump_man_half_width_px) - con::jump_man_body_box_margin_px;
  jm.body_box.bottom = (jm.center_px.y - con::jump_man_half_height_px) + con::jump_man_body_box_margin_px;
  jm.body_box.top = (jm.center_px.y + con::jump_man_half_height_px) - con::jump_man_body_box_margin_px;
}

void update_jump_man_leap_box(jump_man_data& jm)
{
  auto box_half_width =
    input::is_action_triggered(input::ACTION_MOVE_LEFT) ||
    input::is_action_triggered(input::ACTION_MOVE_RIGHT) ?
    con::jump_man_leap_wide_box_half_width_px : con::jump_man_leap_box_half_width_px;

  jm.leap_box.left = jm.center_px.x - box_half_width;
  jm.leap_box.right = jm.center_px.x + box_half_width;
  jm.leap_box.bottom = (jm.center_px.y + con::jump_man_half_height_px);
  jm.leap_box.top = (jm.center_px.y + con::jump_man_half_height_px) + con::jump_man_leap_box_height_px;
}

void update_jump_man_hammer_box(jump_man_data& jm)
{
  if(!(jm.flags & FLAG_JM_HAMMERING))
    return;

  if(jm.animation_.get_current_frame().flags & ANIM_FRAME_HAMMER_DOWN){
    auto offset = jm.look_dir == look_direction::left ?
      -con::jump_man_hammer_down_box_offset_px : con::jump_man_hammer_down_box_offset_px;

    auto hammer_center_x_px = jm.center_px.x + offset;

    jm.hammer_box.left = hammer_center_x_px - con::jump_man_hammer_head_half_height_px;
    jm.hammer_box.right = hammer_center_x_px + con::jump_man_hammer_head_half_height_px;
    jm.hammer_box.bottom = jm.center_px.y - con::jump_man_hammer_head_half_width_px;
    jm.hammer_box.top = jm.center_px.y + con::jump_man_hammer_head_half_width_px;
  }
  else{
    auto hammer_center_y_px = jm.center_px.y - con::jump_man_hammer_up_box_offset_px;

    jm.hammer_box.left = jm.center_px.x - con::jump_man_hammer_head_half_width_px;
    jm.hammer_box.right = jm.center_px.x + con::jump_man_hammer_head_half_width_px;
    jm.hammer_box.bottom = hammer_center_y_px - con::jump_man_hammer_head_half_height_px;
    jm.hammer_box.top = hammer_center_y_px + con::jump_man_hammer_head_half_height_px;
  }
}

void update_barrel_body_box(barrel_data& bd)
{
  constexpr auto offset_x = (con::barrel_width_px - con::barrel_box_width_px) / 2;
  constexpr auto offset_y = con::barrel_height_px - con::barrel_box_height_px;
  bd.body_box.left = bd.position_px.x + offset_x;
  bd.body_box.bottom = bd.position_px.y + offset_y;
  bd.body_box.right = bd.body_box.left + con::barrel_box_width_px;
  bd.body_box.top = bd.body_box.bottom + con::barrel_box_height_px;
}

void update_barrel_ladder_box(barrel_data& bd)
{
  constexpr auto offset_x = (con::barrel_width_px - con::barrel_ladder_box_width_px) / 2;
  constexpr auto offset_y = con::barrel_height_px / 2;
  bd.ladder_box.left = bd.position_px.x + offset_x;
  bd.ladder_box.bottom = bd.position_px.y + offset_y;
  bd.ladder_box.right = bd.ladder_box.left + con::barrel_ladder_box_width_px;
  bd.ladder_box.top = bd.ladder_box.bottom + con::barrel_ladder_box_height_px;
}

void update_wild_body_box(wild_data& wd)
{
  wd.body_box.left = wd.position_px.x + con::wild_body_margin_px;
  wd.body_box.right = (wd.position_px.x + con::barrel_wild_width_px) - con::wild_body_margin_px;
  wd.body_box.bottom = wd.position_px.y + con::wild_body_margin_px;
  wd.body_box.top = wd.position_px.y + con::barrel_wild_height_px;
}

void update_mob_aabbs(mob_data& mob)
{
  mob.body_box.left = (mob.center_px.x - con::mob_half_width_px[mob.type]) + con::mob_body_margin_px;
  mob.body_box.right = (mob.center_px.x + con::mob_half_width_px[mob.type]) - con::mob_body_margin_px;
  mob.body_box.bottom = (mob.center_px.y - con::mob_half_height_px[mob.type]) + con::mob_body_margin_px;
  mob.body_box.top = (mob.center_px.y + con::mob_half_height_px[mob.type]);

  mob.ladder_box.left = mob.center_px.x - con::mob_ladder_box_half_width_px;
  mob.ladder_box.right = mob.center_px.x + con::mob_ladder_box_half_width_px;
  mob.ladder_box.bottom = mob.center_px.y + con::mob_half_height_px[mob.type] - 2.f;
  mob.ladder_box.top = mob.ladder_box.bottom + con::mob_ladder_box_height_px;

  mob.left_feeler.left = mob.center_px.x - con::mob_half_width_px[mob.type] - con::mob_feeler_range_px - con::mob_feeler_box_width_px;
  mob.left_feeler.right = mob.left_feeler.left + con::mob_feeler_box_width_px;
  mob.left_feeler.bottom = mob.center_px.y;
  mob.left_feeler.top = mob.center_px.y + con::mob_half_height_px[mob.type] + con::mob_feeler_extension_px;

  mob.right_feeler.left = mob.center_px.x + con::mob_half_width_px[mob.type] + con::mob_feeler_range_px;
  mob.right_feeler.right = mob.right_feeler.left + con::mob_feeler_box_width_px;
  mob.right_feeler.bottom = mob.left_feeler.bottom;
  mob.right_feeler.top = mob.left_feeler.top;
}

void update_spring_body_box(spring_data& sd)
{
  sd.body_box.left = sd.shift_x_px + (sd.center_px.x - con::spring_body_box_half_width_px);
  sd.body_box.right = sd.shift_x_px + (sd.center_px.x + con::spring_body_box_half_width_px);
  sd.body_box.bottom = sd.center_px.y - con::spring_body_box_half_height_px;
  sd.body_box.top = sd.center_px.y + con::spring_body_box_half_height_px;
}

void update_tub_aabb(tub_data& tub)
{
  auto w = con::tub_width_px[tub.type];
  auto h = con::tub_height_px[tub.type];
  auto m = con::tub_margin_px[tub.type];
  tub.body_box.left = tub.position_px.x + m;
  tub.body_box.bottom = tub.position_px.y + m;
  tub.body_box.right = (tub.position_px.x + w) - m;
  tub.body_box.top = tub.position_px.y + h;
}

void update_kong_aabb(kong_data& kd)
{
  kd.body_box.left = kd.center_px.x - con::kong_body_half_width_px;
  kd.body_box.right = kd.center_px.x + con::kong_body_half_width_px;
  kd.body_box.bottom = kd.center_px.y - con::kong_body_half_height_px;
  kd.body_box.top = kd.center_px.y + con::kong_body_half_height_px;
}

void update_barrel_kong_aabb(barrel_kong_data& kd)
{
  kd.body_box.left = kd.center_px.x - con::kong_body_half_width_px;
  kd.body_box.right = kd.center_px.x + con::kong_body_half_width_px;
  kd.body_box.bottom = kd.center_px.y - con::kong_body_half_height_px;
  kd.body_box.top = kd.center_px.y + con::kong_body_half_height_px;
}

vector2f select_wild_rebound_dir(const wild_data& wd, const jump_man_data& jm)
{
  auto is_left = (wd.position_px.x + con::barrel_wild_half_width_px) < con::wild_left_margin_x;
  auto is_right = (wd.position_px.x + con::barrel_wild_half_width_px) > con::wild_right_margin_x;
  switch(wd.wild_type_){
    case WILD_CALM: {
      auto lo = is_left ? con::narrow_rebound_down : 0;
      auto hi = is_right ? con::narrow_rebound_down : con::narrow_rebound_dirs_count - 1;
      return con::narrow_rebound_dirs[rnd::uniformUnsignedInt(lo, hi)];
    }
    case WILD_DROP:
      if(wd.rebound_count == 0){
        return con::wide_rebound_dirs[con::wide_rebound_down];
      }
      // fallthrough.
    case WILD_CRAZY:{
      auto lo = is_left ? con::wide_rebound_down : 0;
      auto hi = is_right ? con::wide_rebound_down : con::wide_rebound_dirs_count - 1;
      return con::wide_rebound_dirs[rnd::uniformUnsignedInt(lo, hi)];
    }
    case WILD_HOMING: {
      auto wx = wd.position_px.x + con::barrel_wild_half_width_px;
      auto jx = jm.center_px.x;
      if((wx - 2.f) <= jx && jx <= (wx + 2.f)){ // if approx equal
        return con::wide_rebound_dirs[con::wide_rebound_down];
      }
      else if(jx < wx){
        auto dir = vector2f{};
        auto dir_index = con::wide_rebound_down - 1;
        auto homing_region_border = wx;
        while(jx < homing_region_border && dir_index >= 0){
          dir = con::wide_rebound_dirs[dir_index--];
          homing_region_border -= con::wild_homing_region_width_px;
        }
        return dir;
      }
      else{ // jx > wx
        auto dir = vector2f{};
        auto dir_index = con::wide_rebound_down + 1;
        auto homing_region_border = wx;
        while(jx > homing_region_border && dir_index < con::wide_rebound_dirs_count){
          dir = con::wide_rebound_dirs[dir_index++];
          homing_region_border += con::wild_homing_region_width_px;
        }
        return dir;
      }
    }
    default:
      assert(0);
  }
}

namespace {
  enum class support_type {
    none,
    girder,
    conveyor
  };

  struct support {
    support_type type;
    int surface_px;
    vector2f surface_velocity; // ignore for girder supports.
    int flags = 0;
  };

  support find_support(const game_data& gd, aabb supported)
  {
    // note: we assume the girders are not setup in such a way that we could interact with
    // multiple at once; this should be ensured by the level design.
    // note: we give the conveyors priority over the girders since they have the additional
    // effect.
    for(const auto& conveyor : gd.conveyors){
      if(is_collision_hline(supported, conveyor.left_px, conveyor.right_px, conveyor.surface_px)){
        auto belt_speed_x = conveyor.belt_dir == look_direction::left ?
          -con::conveyor_belt_speed_px_per_s : con::conveyor_belt_speed_px_per_s;
        return {support_type::conveyor, conveyor.surface_px, {belt_speed_x, 0.f}};
      }
    }

    for(const auto& elevator : gd.elevators){
      if(is_collision_hline(supported, elevator.left_px, elevator.right_px, elevator.surface_px)){
        return {support_type::girder, elevator.surface_px, {}};
      }
    }

    for(const auto& rivet: gd.rivets){
      if(is_collision_hline(supported, rivet.left_px, rivet.left_px + con::rivet_width_px, rivet.surface_px)){
        return {support_type::girder, rivet.surface_px, {}};
      }
    }

    // note: we find the first 2 girders we are interacting with and select the highest;
    // jump man's feet are only wide enough to interact with at most 2 girders.
    const girder_data* active_girder = nullptr;
    for(const auto& girder : gd.girders){
      if(is_collision_hline(supported, girder.left_px, girder.right_px, girder.surface_px)){
        if(!active_girder){
          active_girder = &girder;
        }
        else if(girder.surface_px < active_girder->surface_px){
          active_girder = &girder;
          break;
        }
      }
    }
    if(active_girder){
      return {support_type::girder, active_girder->surface_px, {}, active_girder->flags};
    }

    return {support_type::none, 0, {}};
  }

  // note: there is both a find ladder and find climbable because the mobs and barrels use the former, and
  // jump man the latter. Why? Because mobs and barrels cannot use moving ladders only ladders.

  // The bool in the pair signals if the collision is at the bottom of the ladder (true), or the top (false).
  std::pair<const ladder_data*, bool> find_ladder(const auto& ladders, aabb climber)
  {
    const ladder_data* active_ladder = nullptr;
    for(const auto& ladder : ladders){
      if(is_collision_point(climber, vector2i{ladder.spine_px, ladder.top_px})){
        return {&ladder, true};
      }
      if(is_collision_point(climber, vector2i{ladder.spine_px, static_cast<int>(ladder.bottom_px)})){
        return {&ladder, false};
      }
    }
    return {nullptr, false};
  }

  // The bool in the pair signals if the collision is at the bottom of the ladder (true), or the top (false).
  std::pair<const climbable_data*, bool> find_climbable(game_data& gd, aabb climber)
  {
    const ladder_data* active_ladder = nullptr;
    bool at_bottom = false;
    for(const auto& ladder : gd.ladders){
      if(is_collision_point(climber, vector2i{ladder.spine_px, ladder.top_px})){
        active_ladder = &ladder;
        at_bottom = true;
        break;
      }
      if(is_collision_point(climber, vector2i{ladder.spine_px, static_cast<int>(ladder.bottom_px)})){
        active_ladder = &ladder;
        break;
      }
    }
    if(active_ladder){
      return std::make_pair(active_ladder, at_bottom);
    }

    const moving_ladder_data* active_moving_ladder = nullptr;
    for(const auto& ladder : gd.moving_ladders){
      if(is_collision_point(climber, vector2i{ladder.spine_px, ladder.top_px})){
        active_moving_ladder = &ladder;
        at_bottom = true;
        break;
      }
      if(is_collision_point(climber, vector2i{ladder.spine_px, static_cast<int>(ladder.bottom_px)})){
        active_moving_ladder = &ladder;
        break;
      }
    }
    if(active_moving_ladder){
      return std::make_pair(active_moving_ladder, at_bottom);
    }

    return std::make_pair(nullptr, false);
  }

  bool jm_support_required(const jump_man_data& jm)
  {
    return jm.state_ == jump_man_state::idle ||
           jm.state_ == jump_man_state::running ||
           jm.state_ == jump_man_state::jumping;
  }

  bool jm_ladder_required(const jump_man_data& jm)
  {
    return jm.state_ == jump_man_state::idle ||
    jm.state_ == jump_man_state::running;
  }

  void jm_start_idle(jump_man_data& jm)
  {
    jm.velocity.zero();
    const auto& frames = jm.flags & FLAG_JM_HAMMERING ? jm.flags & FLAG_JM_HAMMER_EXPIRING ?
      *jump_man_animations::jm_idle_expiring_hammering_animations[jm.character_] :
      *jump_man_animations::jm_idle_hammering_animations[jm.character_] :
      *jump_man_animations::jm_idle_animations[jm.character_];
    jm.animation_.initialise(frames);
    jm.state_ = jump_man_state::idle;
  }

  void jm_start_run_right(jump_man_data& jm)
  {
    jm.velocity = vector2f{con::jump_man_run_speed_px_per_s, 0.f};
    const auto& frames = jm.flags & FLAG_JM_HAMMERING ? jm.flags & FLAG_JM_HAMMER_EXPIRING ?
      *jump_man_animations::jm_run_expiring_hammering_animations[jm.character_] :
      *jump_man_animations::jm_run_hammering_animations[jm.character_] :
      *jump_man_animations::jm_run_animations[jm.character_];
    jm.animation_.initialise(frames);
    jm.look_dir = look_direction::right;
    jm.state_ = jump_man_state::running;
  }

  void jm_start_run_left(jump_man_data& jm)
  {
    jm.velocity = vector2f{-con::jump_man_run_speed_px_per_s, 0.f};
    const auto& frames = jm.flags & FLAG_JM_HAMMERING ? jm.flags & FLAG_JM_HAMMER_EXPIRING ?
      *jump_man_animations::jm_run_expiring_hammering_animations[jm.character_] :
      *jump_man_animations::jm_run_hammering_animations[jm.character_] :
      *jump_man_animations::jm_run_animations[jm.character_];
    jm.animation_.initialise(frames);
    jm.look_dir = look_direction::left;
    jm.state_ = jump_man_state::running;
  }

  void jm_start_dying(jump_man_data& jm)
  {
    jm.velocity.zero();
    jm.animation_.initialise(*jump_man_animations::jm_dying_animations[jm.character_], animation::mode::once);
    jm.state_ = jump_man_state::dying;
  }

  void jm_start_climbing_up(jump_man_data& jm, int ladder_left_px)
  {
    jm.center_px.x = ladder_left_px; // snap to ladder axis.
    jm.animation_.initialise(*jump_man_animations::jm_climbing_animations[jm.character_]);
    jm.velocity = vector2f{0.f, -con::jump_man_climb_speed_px_per_s};
    jm.state_ = jump_man_state::climbing;
  }

  void jm_start_climbing_down(jump_man_data& jm)
  {
    jm.animation_.initialise(*jump_man_animations::jm_climbing_animations[jm.character_]);
    jm.velocity = vector2f{0.f, con::jump_man_climb_speed_px_per_s};
    jm.state_ = jump_man_state::climbing;
  }

  void jm_start_climb_idle(jump_man_data& jm)
  {
    jm.animation_.initialise(*jump_man_animations::jm_climb_idle_animations[jm.character_]);
    jm.velocity.zero();
    jm.state_ = jump_man_state::climbing_idle;
  }

  void jm_stand_on_support(jump_man_data& jm, const support& s)
  {
    jm.center_px.y = s.surface_px - con::jump_man_half_height_px;
  }

  // height_offset is used to require the barrel to be an extra distance below the player.
  bool is_jm_above_barrel(const jump_man_data& jm, const barrel_data& bd, float height_offset_px)
  {
    return jm.center_px.y - con::jump_man_half_height_px < (bd.position_px.y + con::barrel_height_px) - height_offset_px;
  }

  // logic taken from original game, which was nicely broken down here:
  //   http://donhodges.com/Controlling_the_barrels_in_Donkey_Kong.htm
  // note: there appears to be an error in this article in which it states that the expression
  // r%3 gives a random number in the range [0,3] inclusive, which it does not; this range is
  // the result of r%4.
  bool use_ladder(const game_data& gd, const barrel_data& bd, const ladder_data& ld)
  {
    if(gd.oil_can_.state == oil_can_state::not_lit){
      return true;
    }
    if(is_jm_above_barrel(gd.jump_man, bd, 0)){
      return false;
    }
    auto r = rnd::uniformSignedInt(0, 255);
    if(r % 4 >= static_cast<int>(gd.difficulty / 2) + 1){
      return false;
    }
    auto is_move_left = input::is_action_triggered(input::ACTION_MOVE_LEFT);
    auto is_move_right = input::is_action_triggered(input::ACTION_MOVE_RIGHT);
    if(ld.spine_px == gd.jump_man.center_px.x ||
    (bd.position_px.x < gd.jump_man.center_px.x && is_move_left) ||
    (bd.position_px.x > gd.jump_man.center_px.x && is_move_right)){
      return true;
    }
    return (r & 0x18) == 0;
  }

  void reverse_barrel(barrel_data& barrel)
  {
    barrel.look_dir = barrel.look_dir == look_direction::left ?
      look_direction::right : look_direction::left;

    barrel.velocity.x = barrel.look_dir == look_direction::left ?
      -con::barrel_speed_px_per_s : con::barrel_speed_px_per_s;

    auto animation_mode = barrel.look_dir == look_direction::left ?
      animation::mode::reverse_loop : animation::mode::loop;

    auto& animation_frames = barrel.type == barrel_type::common ?
      common_barrel_rolling : blue_barrel_rolling;

    barrel.animation_.initialise(animation_frames, animation_mode);
  }

  void reverse_mob(mob_data& mob, float mob_roam_speed_px_per_s)
  {
    mob.look_dir = mob.look_dir == look_direction::left ?
      look_direction::right : look_direction::left;

    mob.velocity.x = mob.look_dir == look_direction::left ?
      -mob_roam_speed_px_per_s : mob_roam_speed_px_per_s;
  }

  void update_conveyors(game_data& gd, float dt_s)
  {
    if(!gd.conveyors.empty()){
      for(auto& conveyor : gd.conveyors){
        conveyor.left_cap_anim.update(dt_s);
        conveyor.right_cap_anim.update(dt_s);
      }
      gd.conveyor_tick_clock_s += dt_s;
      if(gd.conveyor_tick_clock_s > con::conveyor_tick_interval_s){
        gd.conveyor_tick_clock_s = 0.f;
        for(auto& conveyor : gd.conveyors){
          if(rnd::uniformSignedInt(1, con::conveyor_reverse_chance) == 1){
            conveyor.belt_dir = conveyor.belt_dir == look_direction::left ?
              look_direction::right : look_direction::left;
            auto anim_mode = conveyor.belt_dir == look_direction::left ?
              animation::mode::reverse_loop : animation::mode::loop;
            conveyor.left_cap_anim.initialise(conveyor_left_cap_animation, anim_mode);
            conveyor.right_cap_anim.initialise(conveyor_right_cap_animation, anim_mode);
          }
        }
      }
    }
  }

  void update_moving_ladders(game_data& gd, float dt_s)
  {
    for(auto& ladder : gd.moving_ladders){
      ladder.bottom_px += con::moving_ladder_speed_px_per_s * ladder.move_dir * dt_s;
      if(ladder.move_dir > 0) {
        if(ladder.bottom_px > ladder.bottom_contracted_px){ // contracting ladder.
          ladder.bottom_px = ladder.bottom_contracted_px;
          ladder.last_move_dir = ladder.move_dir;
          ladder.move_dir = 0;
          ladder.rest_clock_s = 0.f;
        }
      }
      else if(ladder.move_dir < 0){
        if(ladder.bottom_px < ladder.bottom_extended_px){ // extending ladder.
          ladder.bottom_px = ladder.bottom_extended_px;
          ladder.last_move_dir = ladder.move_dir;
          ladder.move_dir = 0;
          ladder.rest_clock_s = 0.f;
          ladder.flags = 0; // enable the ladder top.
        }
      }
      else{
        ladder.rest_clock_s += dt_s;
        if(ladder.rest_clock_s > con::moving_ladder_rest_duration_s){
          ladder.move_dir = ladder.last_move_dir * -1;
          ladder.flags |= FLAG_CLIMBABLE_NO_TOP;
        }
      }
    }
  }

  void update_elevators(game_data& gd, float dt_s)
  {
    for(auto& elevator : gd.elevators){
      elevator.move_pos_px += con::elevator_speed_px_per_s * elevator.move_dir * dt_s;
      if(elevator.move_dir > 0){
        if(elevator.move_pos_px > elevator.move_max_px){
          if(elevator.mode == elevator_mode::repeat){
            elevator.move_pos_px = elevator.move_min_px + (elevator.move_pos_px - elevator.move_max_px);
          }
          else{
            elevator.last_move_dir = elevator.move_dir;
            elevator.move_dir = 0;
            elevator.move_pos_px = elevator.move_max_px;
          }
        }
      }
      else if(elevator.move_dir < 0){
        if(elevator.move_pos_px < elevator.move_min_px){
          if(elevator.mode == elevator_mode::repeat){
            elevator.move_pos_px = elevator.move_max_px + (elevator.move_pos_px - elevator.move_min_px);
          }
          else{
            elevator.last_move_dir = elevator.move_dir;
            elevator.move_dir = 0;
            elevator.move_pos_px = elevator.move_min_px;
          }
        }
      }
      else{
        elevator.rest_clock_s += dt_s;
        if(elevator.rest_clock_s > con::elevator_rest_duration_s){
          elevator.move_dir = elevator.last_move_dir * -1;
          elevator.rest_clock_s = 0.f;
        }
      }
      auto& pos = elevator.dir == elevator_dir::vertical ? elevator.surface_px : elevator.left_px;
      pos = std::round(elevator.move_pos_px);
    }
  }

  void update_springs(game_data& gd, float dt_s)
  {
    for(auto& spring : gd.springs){
      auto distance = con::spring_speed_px_per_s * dt_s;
      if(distance >= spring.target_distance_px){
        auto overflow = distance - spring.target_distance_px;
        spring.center_px += spring.target_dir * spring.target_distance_px;
        spring.sid = (*spring.path)[spring.target_index].flags & FLAG_OPEN_SPRING ? SID_SPRING_OPEN :
          (*spring.path)[spring.target_index].flags & FLAG_CLOSE_SPRING ? SID_SPRING_CLOSED : spring.sid;
        if((*spring.path)[spring.target_index].flags & FLAG_CLOSE_SPRING){
          play_sound(SND_SPRING_BOUNCE, 1.f, false, con::spring_bounce_gain);
        }
        else if((*spring.path)[spring.target_index].flags & FLAG_FALL_SPRING){
          play_sound(SND_SPRING_FALL, 1.f, false, con::spring_fall_gain);
        }
        spring.target_index++;
        if(spring.target_index >= (*spring.path).size()){
          spring.target_index = 1;
          spring.center_px = (*spring.path)[0].position_px;
        }
        auto displacement = (*spring.path)[spring.target_index].position_px - spring.center_px;
        spring.target_dir = norm(displacement);
        spring.target_distance_px = displacement.length() - overflow;
        spring.center_px += spring.target_dir * overflow;
      }
      else{
        spring.center_px += spring.target_dir * distance;
        spring.target_distance_px -= distance;
      }
      update_spring_body_box(spring);
    }
  }

  void update_tubs(game_data& gd, float dt_s)
  {
    for(auto& tub : gd.tubs){
      auto support_ = find_support(gd, tub.body_box);
      if(support_.type != support_type::none){
        tub.state = tub_state::resting;
        tub.position_px.y = support_.surface_px - con::tub_height_px[tub.type];
        tub.velocity.y = 0.f;
      }
      else{
        tub.state = tub_state::falling;
      }
      if(tub.state == tub_state::falling){
        tub.velocity.y += con::tub_gravity_px_per_s2 * dt_s;
      }
      auto net_velocity = tub.velocity;
      if(support_.type == support_type::conveyor){
        net_velocity += support_.surface_velocity;
      }
      tub.position_px += net_velocity * dt_s;
      update_tub_aabb(tub);
    }

    // remove tubs if they have fallen of the end of the conveyors whilst off-screen.
    gd.tubs.erase(std::remove_if(gd.tubs.begin(), gd.tubs.end(), [](const tub_data& tub){
      return (tub.state == tub_state::falling) && (tub.position_px.x < 0.f || tub.position_px.x > con::world_width_px);
    }), gd.tubs.end());
  }

  void update_jump_man(game_data& gd, float dt_s)
  {
    auto& jm = gd.jump_man;
    auto last_state = jm.state_;

    support support_{};
    if(jm_support_required(jm)){
      support_ = find_support(gd, jm.feet_box);
    }

    auto is_at_climbable_bottom = false;
    if(jm_ladder_required(jm)){
      std::tie(gd.active_climbable, is_at_climbable_bottom) = find_climbable(gd, jm.feet_box);
    }

    if(jm.state_== jump_man_state::idle || jm.state_ == jump_man_state::running){
      auto is_state_change = false;

      if(support_.type != support_type::none){
        jm_stand_on_support(jm, support_);
      }
      else {
        // start falling because no support.
        jm.state_ = jump_man_state::jumping;
        jm.fall_start_y_px = jm.center_px.y;
        jm.velocity.x = 0.f;
        if(!(jm.flags & FLAG_JM_HAMMERING)){
          jm.animation_.initialise(*jump_man_animations::jm_jump_animations[jm.character_]);
        }
        jm.state_ = jump_man_state::jumping;
        is_state_change = true;
      }

      if(!is_state_change && !(jm.flags & FLAG_JM_HAMMERING) && input::is_action_triggered(input::ACTION_JUMP)){
        // start jumping.
        jm.velocity += vector2f{0.f, -1.f} * con::jump_man_jump_speed_px_per_s;
        jm.velocity.x *= con::jump_man_jump_x_speed_scale;
        jm.animation_.initialise(*jump_man_animations::jm_jump_animations[jm.character_]);
        jm.fall_start_y_px = jm.center_px.y;
        jm.state_ = jump_man_state::jumping;
        play_sound(SND_JUMP);
      }

      if(!is_state_change && !(jm.flags & FLAG_JM_HAMMERING) && gd.active_climbable){
        if(is_at_climbable_bottom && input::is_action_triggered(input::ACTION_CLIMB_UP)){
          jm_start_climbing_up(jm, gd.active_climbable->spine_px);
          is_state_change = true;
        }
        else if(!is_at_climbable_bottom && input::is_action_triggered(input::ACTION_CLIMB_DOWN)){
          // start climbing onto the top of the ladder.
          jm.velocity.zero();
          jm.center_px.x = gd.active_climbable->spine_px; // snap to ladder axis.
          jm.center_px.y = gd.active_climbable->bottom_px; // y-axis points down so bottom is top.
          jm.animation_.initialise(*jump_man_animations::jm_climb_off_animations[jm.character_], animation::mode::reverse_once);
          jm.state_ = jump_man_state::climbing_on_top;
          is_state_change = true;
        }
      }

      auto is_move_left = input::is_action_triggered(input::ACTION_MOVE_LEFT);
      auto is_move_right = input::is_action_triggered(input::ACTION_MOVE_RIGHT);
      if(jm.state_ == jump_man_state::idle){
        if(is_move_left && !is_move_right){
          jm_start_run_left(jm);
        }
        else if(!is_move_left && is_move_right){
          jm_start_run_right(jm);
        }
      }
      else if(jm.state_ == jump_man_state::running) {
        if(is_move_left == is_move_right){
          jm_start_idle(jm);
        }
        else if(is_move_left && jm.look_dir == look_direction::right){
          jm_start_run_left(jm);
        }
        else if(is_move_right && jm.look_dir == look_direction::left){
          jm_start_run_right(jm);
        }
      }
    }

    else if(jm.state_ == jump_man_state::jumping){
      if(support_.type != support_type::none && jm.velocity.y > 0.f){
        jm_stand_on_support(jm, support_);
        auto fall_y_delta_px = jm.center_px.y - jm.fall_start_y_px;
        if(global::is_jm_invulnerable){
          fall_y_delta_px = 0.f;
        }
        if(fall_y_delta_px > con::jump_man_splat_height_px){
          gd.flags |= FLAG_JUMP_MAN_HIT;
          gd.flags |= FLAG_GAME_FROZEN;
          jm.animation_.initialise(*jump_man_animations::jm_hit_animations[jm.character_]);
          stop_music();
          stop_run_sound();
          play_sound(SND_FALL);
          play_sound(SND_DEATH, 1.f, false, con::death_sound_gain);
        }
        else {
          jm_start_idle(jm);
        }
      }
    }

    // if jump man is standing on a ladder he has already climbed onto.
    else if(jm.state_ == jump_man_state::climbing_idle){
      auto is_climb_up = input::is_action_triggered(input::ACTION_CLIMB_UP);
      auto is_climb_down = input::is_action_triggered(input::ACTION_CLIMB_DOWN);
      // may be on a contracting moving ladder and resting above the current bottom (bottom is top w.r.t world
      // as y-axis down).
      if(jm.center_px.y <= gd.active_climbable->bottom_px){
        jm.center_px.y = gd.active_climbable->bottom_px;
      }
      if(is_climb_up && !is_climb_down){
        jm_start_climbing_up(jm, gd.active_climbable->spine_px);
      }
      else if(is_climb_down && !is_climb_up){
        jm_start_climbing_down(jm);
      }
    }

    else if(jm.state_ == jump_man_state::climbing){
      auto is_climb_up = input::is_action_triggered(input::ACTION_CLIMB_UP);
      auto is_climb_down = input::is_action_triggered(input::ACTION_CLIMB_DOWN);
      if(is_climb_up == is_climb_down){
        // stop climbing and idle on the ladder.
        jm_start_climb_idle(jm);
      }
      else if(is_climb_up){
        if(jm.center_px.y <= gd.active_climbable->bottom_px){ // remember y-axis down.
          if(gd.active_climbable->flags & FLAG_CLIMBABLE_NO_TOP){
            // cannot climb off a ladder with no top so clamp to highest point.
            jm.center_px.y = gd.active_climbable->bottom_px;
          }
          else {
            // start climbing off the top of the ladder.
            jm.velocity.zero();
            jm.animation_.initialise(*jump_man_animations::jm_climb_off_animations[jm.character_], animation::mode::once);
            jm.state_ = jump_man_state::climbing_off_top;
          }
        }
      }
      else { // climbing down
        if(jm.feet_box.top >= gd.active_climbable->top_px){ // top is larger y (which is lower on screen).
          // drop off ladder.
          jm_start_idle(jm);
          gd.active_climbable = nullptr;
        }
      }
    }

    else if(jm.state_ == jump_man_state::climbing_on_top){
      if(jm.animation_.is_done()){
        jm_start_climbing_down(jm);
      }
    }

    else if(jm.state_ == jump_man_state::climbing_off_top){
      if(jm.animation_.is_done()){
        jm.center_px.y = gd.active_climbable->bottom_px - con::jump_man_half_height_px;
        jm_start_idle(jm);
        gd.active_climbable = nullptr;
      }
    }

    // handle hammer countdown.
    if(jm.flags & FLAG_JM_HAMMERING){
      jm.hammer_timer_s -= dt_s;
      if(jm.hammer_timer_s < 0.f){
        jm.flags &= ~(FLAG_JM_HAMMERING | FLAG_JM_HAMMER_EXPIRING);
        stop_music();
        play_music(gd.current_stage->music_track_);
        set_mob_vulnerability(gd, false);
        const auto& frames = jm.state_ == jump_man_state::running ?
          *jump_man_animations::jm_run_animations[jm.character_] :
          *jump_man_animations::jm_idle_animations[jm.character_];
        jm.animation_.initialise(frames);
        if(jm.state_ == jump_man_state::running) start_run_sound();
      }
      else if(!(jm.flags & FLAG_JM_HAMMER_EXPIRING) && jm.hammer_timer_s < con::jump_man_hammer_expiring_duration_s){
        const auto& frames = (jm.state_ == jump_man_state::running || jm.state_ == jump_man_state::jumping) ?
          *jump_man_animations::jm_run_expiring_hammering_animations[jm.character_] :
          *jump_man_animations::jm_idle_expiring_hammering_animations[jm.character_];
        jm.animation_.initialise(frames);
        jm.flags |= FLAG_JM_HAMMER_EXPIRING;
      }
    }

    auto new_state = jm.state_;
    if(last_state != new_state){
      if(last_state == jump_man_state::running) stop_run_sound();
      if(new_state == jump_man_state::running) start_run_sound();
      if(last_state == jump_man_state::climbing) stop_climb_sound();
      if(new_state == jump_man_state::climbing) start_climb_sound();
    }

    jm.animation_.update(dt_s);

    // integrate jump man.
    if(jm.state_ == jump_man_state::jumping){
      jm.velocity.y += con::jump_man_gravity_px_per_s2 * dt_s;
      // bounce jump man off the edge of the world if jumping into it.
      if((jm.center_px.x - con::jump_man_half_width_px < 0.f && jm.velocity.x < 0.f) ||
      (jm.center_px.x + con::jump_man_half_width_px > con::world_width_px && jm.velocity.x > 0.f)){
        jm.velocity.x *= -1.f;
        jm.look_dir = jm.look_dir == look_direction::left ? look_direction::right : look_direction::left;
      }
    }
    auto net_velocity = jm.velocity;
    if(support_.type == support_type::conveyor){
      net_velocity += support_.surface_velocity;
    }
    // prevent jump man leaving the game.
    if((jm.center_px.x - con::jump_man_half_width_px < 0.f && net_velocity.x < 0.f) ||
    (jm.center_px.x + con::jump_man_half_width_px > con::world_width_px && net_velocity.x > 0.f)){
      net_velocity.x *= 0.f;
    }
    jm.center_px += net_velocity * dt_s;
    update_jump_man_feet_box(jm);
    update_jump_man_body_box(jm);
    update_jump_man_leap_box(jm);
    update_jump_man_hammer_box(jm);
  }

  void update_pauline(game_data& gd, float dt_s)
  {
    if(gd.pauline.flags & FLAG_PAULINE_VISIBLE){
      if(gd.pauline.state == pauline_state::idle){
        gd.pauline.help_clock_s += dt_s;
        if(gd.pauline.help_clock_s > con::pauline_help_interval_s){
          gd.pauline.state = pauline_state::calling_help;
          gd.pauline.help_clock_s = 0.f;
          gd.pauline.wiggle_anim.reset();
        }
      }
      else{
        gd.pauline.wiggle_anim.update(dt_s);
        if(gd.pauline.wiggle_anim.is_done()){
          gd.pauline.state = pauline_state::idle;
        }
      }
      if((gd.pauline.flags & FLAG_PAULINE_CAN_FLIP) && rnd::uniformSignedInt(1, con::pauline_flip_chance) == 1){
        gd.pauline.flags = gd.pauline.flags & FLAG_PAULINE_FLIPPED ?
          gd.pauline.flags & ~FLAG_PAULINE_FLIPPED : gd.pauline.flags | FLAG_PAULINE_FLIPPED;
      }
    }
  }

  void update_pickups(game_data& gd)
  {
    auto& jm = gd.jump_man;

    auto can_pickup =
      jm.state_ == jump_man_state::idle ||
      jm.state_ == jump_man_state::running ||
      jm.state_ == jump_man_state::jumping;

    if(!can_pickup)
      return;

    for(auto& pickup : gd.pickups){
      if(is_collision(pickup.collect_box, gd.jump_man.body_box)){
        if(pickup.pickup_type_ == pickup_type::hammer && !(gd.jump_man.flags & FLAG_JM_HAMMERING)){
          const auto& frames = (jm.state_ == jump_man_state::running || jm.state_ == jump_man_state::jumping) ?
            *jump_man_animations::jm_run_hammering_animations[jm.character_] :
            *jump_man_animations::jm_idle_hammering_animations[jm.character_];
          jm.animation_.initialise(frames);
          gd.jump_man.flags |= FLAG_JM_HAMMERING;
          gd.jump_man.hammer_timer_s = con::jump_man_hammer_total_duration_s;
          pickup.flags |= FLAG_PICKUP_COLLECTED;
          stop_music();
          play_music(music_track::hammer);
          set_mob_vulnerability(gd, true);
        }
        else{ // collectable
          pickup.flags |= FLAG_PICKUP_COLLECTED;
          auto popup_position = gd.jump_man.center_px + con::popup_offset_px;
          if(gd.level_number == 1){
            gd.score += 300;
            spawn_score_popup(gd, popup_position, score_value::score_300);
          }
          else if(gd.level_number == 2){
            gd.score += 500;
            spawn_score_popup(gd, popup_position, score_value::score_500);
          }
          else if(gd.level_number >= 3){
            gd.score += 800;
            spawn_score_popup(gd, popup_position, score_value::score_800);
          }
          play_sound(SND_SCORE);
        }
      }
    }

    auto new_end = std::remove_if(gd.pickups.begin(), gd.pickups.end(), [](const pickup_data& p){
      return p.flags & FLAG_PICKUP_COLLECTED;
    });
    gd.pickups.erase(new_end, gd.pickups.end());
  }

  void update_rivets(game_data& gd)
  {
    const auto& jm = gd.jump_man;
    for(auto& rivet : gd.rivets){
      auto body_hit = is_collision(rivet.hit_body_box, jm.feet_box);
      if(body_hit && !(rivet.flags & FLAG_RIVET_HIT)) rivet.flags |= FLAG_RIVET_HIT;
      else if(!body_hit && rivet.flags & FLAG_RIVET_HIT) rivet.flags |= FLAG_RIVET_COLLECTED;
      else if(is_collision(rivet.hit_jump_box, jm.feet_box) && jm.state_ == jump_man_state::jumping){
        rivet.flags |= FLAG_RIVET_COLLECTED;
      }
      if(rivet.flags & FLAG_RIVET_COLLECTED){
        gd.score += 200;
        auto popup_position = gd.jump_man.center_px + con::popup_offset_px;
        spawn_score_popup(gd, popup_position, score_value::score_100);
        play_sound(SND_SCORE);
      }
    }
    gd.rivets.erase(std::remove_if(gd.rivets.begin(), gd.rivets.end(), [](const auto& rivet){
      return rivet.flags & FLAG_RIVET_COLLECTED;
    }), gd.rivets.end());
  }

  void update_animated_props(game_data& gd, float dt_s)
  {
    for(auto& prop : gd.animated_props){
      prop.animation_.update(dt_s);
    }
  }

  void update_popups(game_data& gd, float dt_s)
  {
    auto new_end = std::remove_if(gd.popups.begin(), gd.popups.end(), [dt_s](popup_data& pd){
      pd.popup_clock_s += dt_s;
      return pd.popup_clock_s > con::popup_duration_s;
    });
    gd.popups.erase(new_end, gd.popups.end());
  }

  void update_oil_can(game_data& gd, float dt_s)
  {
    if(gd.oil_can_.state == oil_can_state::lit_hot){
      gd.oil_can_.hot_clock_s += dt_s;
      if(gd.oil_can_.hot_clock_s > con::oil_can_mob_forge_duration_s){
        if(gd.oil_can_.flags & FLAG_OIL_CAN_BIRTH_FIREBALL){
          auto launch_dir = rnd::uniformSignedInt(0, 1) == 0 ? -1 : 1;
          auto mob_spawn_px = vector2f{
            gd.oil_can_.position_px.x + con::oil_can_half_width_px,
            gd.oil_can_.position_px.y
          };
          spawn_mob(gd, mob_type::MOB_FIREBALL, mob_spawn_px, look_direction::left, {
            launch_dir * con::mob_launch_speed_x_px_per_s,
            con::mob_launch_speed_y_px_per_s
          });
          gd.oil_can_.flags &= ~FLAG_OIL_CAN_BIRTH_FIREBALL;
        }
        gd.oil_can_.state = oil_can_state::lit_cold;
        gd.oil_can_.flames_anim_.initialise(cold_flames_animation);
      }
    }
    gd.oil_can_.flames_anim_.update(dt_s);
  }

  void update_kong(game_data& gd, float dt_s)
  {
    if(gd.kong_.flags & FLAG_KONG_ALIVE){
      gd.kong_.animation_.update(dt_s);

      // update chest beating.
      if(gd.kong_.state == kong_state::frowny){
        gd.kong_beat_chest_clock_s += dt_s;
        if(gd.kong_beat_chest_clock_s > con::kong_beat_chest_interval_s){
          gd.kong_.animation_.initialise(kong_beat_chest, animation::mode::once);
          gd.kong_.state = kong_state::chest_beating;
          gd.kong_beat_chest_clock_s = 0.f;
        }
      }
      else if(gd.kong_.animation_.is_done()){
        gd.kong_.animation_.initialise(kong_frowny);
        gd.kong_.state = kong_state::frowny;
      }

      // apply conveyor effect.
      if(gd.kong_.flags & FLAG_KONG_ON_CONVEYOR){
        auto support_ = find_support(gd, gd.kong_.body_box);
        if(support_.type == support_type::conveyor){
          gd.kong_.center_px += support_.surface_velocity * dt_s;
          update_kong_aabb(gd.kong_);
        }
        // prevent the conveyor dragging kong off screen.
        if(gd.kong_.center_px.x - con::kong_screen_margin_px < 0.f){
          gd.kong_.center_px.x = con::kong_screen_margin_px;
        }
        else if(gd.kong_.center_px.x + con::kong_screen_margin_px > con::world_width_px){
          gd.kong_.center_px.x = con::world_width_px - con::kong_screen_margin_px;
        }
      }
    }
  }

  void update_barrel_kong(game_data& gd, float dt_s)
  {
    auto& bk = gd.barrel_kong;

    if(!(bk.flags & FLAG_BARREL_KONG_ALIVE))
      return;

    bk.state_clock_s += dt_s;
    if(bk.state_clock_s < con::barrel_kong_state_duration_s[gd.difficulty - 1])
      return;

    bk.state_clock_s = 0.f;

    switch(bk.state){
      // transition rest -> grab.
      case barrel_kong_state::rest:
        bk.kong_sid = SID_KONG_ROLLING;
        bk.barrel_type_ = bk.flags & FLAG_BARREL_KONG_FORCE_BLUE_BARREL ? barrel_type::blue : barrel_type::common;
        bk.flags &= ~FLAG_BARREL_KONG_FORCE_BLUE_BARREL;
        bk.barrel_sid = bk.barrel_type_ == barrel_type::common ? SID_COMMON_BARREL_0 : SID_BLUE_BARREL_0;
        bk.barrel_position_px = con::barrel_kong_rolling_barrel_offset;
        if(bk.roll_dir == look_direction::right){
          bk.flags &= ~FLAG_BARREL_KONG_FLIP_X; // disable bit.
        }
        else{
          bk.flags |= FLAG_BARREL_KONG_FLIP_X; // enable bit.
          bk.barrel_position_px.x *= -1;
          bk.barrel_position_px.x -= con::barrel_width_px + 1;
        }
        bk.barrel_position_px += bk.center_px;
        bk.state = barrel_kong_state::grab;
        break;
        // transition grab -> wild roll.
      case barrel_kong_state::grab:
        bk.kong_sid = SID_KONG_WITH_BARREL;
        bk.barrel_sid = bk.barrel_type_ == barrel_type::common ? SID_COMMON_BARREL_WILD_0 : SID_BLUE_BARREL_WILD_0;
        bk.barrel_position_px = bk.center_px + con::barrel_kong_wild_barrel_offset;
        bk.flags &= ~FLAG_BARREL_KONG_FLIP_X; // disable bit.
        bk.state = barrel_kong_state::wild;
        break;
        // transition wild -> barrel roll | rest (if rolled wild).
      case barrel_kong_state::wild:
        if(bk.flags & FLAG_BARREL_KONG_FORCE_WILD_ROLL || rnd::uniformSignedInt(1, con::barrel_kong_wild_roll_chance) == 1){
          bk.kong_sid = SID_KONG_FROWNY;
          auto wild_choice =
            bk.flags & FLAG_BARREL_KONG_FORCE_DROP_WILD ? wild_type::WILD_DROP :
            rnd::uniformSignedInt(1, con::barrel_kong_drop_wild_chance) == 1 ?
            wild_type::WILD_DROP : con::barrel_kong_wild_choice[gd.difficulty - 1];
          spawn_wild(gd, bk.barrel_position_px.x, bk.barrel_position_px.y - 1, bk.barrel_type_, wild_choice);
          bk.flags &= ~FLAG_BARREL_KONG_FORCE_DROP_WILD;
          bk.flags &= ~FLAG_BARREL_KONG_FORCE_WILD_ROLL;
          bk.barrel_position_px.x = -100;
          bk.state = barrel_kong_state::rest;
        }
        else {
          bk.kong_sid = SID_KONG_ROLLING;
          bk.barrel_sid = bk.barrel_type_ == barrel_type::common ? SID_COMMON_BARREL_0 : SID_BLUE_BARREL_0;
          bk.barrel_position_px = con::barrel_kong_rolling_barrel_offset;
          if(bk.roll_dir == look_direction::right){
            bk.flags |= FLAG_BARREL_KONG_FLIP_X; // enable bit.
            bk.barrel_position_px.x *= -1;
            bk.barrel_position_px.x -= con::barrel_width_px + 1;
          }
          else{
            bk.flags &= ~FLAG_BARREL_KONG_FLIP_X; // disable bit.
          }
          bk.barrel_position_px += bk.center_px;
          bk.state = barrel_kong_state::roll;
        }
        break;
        // transition barrel roll -> rest
      case barrel_kong_state::roll:
        auto dice_roll = rnd::uniformSignedInt(1, con::barrel_kong_fist_pump_chance);
        auto pump_fist = dice_roll == 1;
        bk.kong_sid = pump_fist ? SID_KONG_BEAT_CHEST : SID_KONG_FROWNY;
        bk.flags &= ~FLAG_BARREL_KONG_FLIP_X; // disable bit.
        if(pump_fist && dice_roll % 2 == 0){
          bk.flags |= FLAG_BARREL_KONG_FLIP_X; // random fist pump left or right.
        }
        spawn_barrel(gd, bk.barrel_position_px, bk.roll_dir, bk.barrel_type_);
        bk.barrel_position_px.x = -100;
        bk.state = barrel_kong_state::rest;
        break;
    }
  }

  void update_barrels(game_data& gd, float dt_s)
  {
    for(auto& barrel : gd.barrels){
      auto support_ = find_support(gd, barrel.body_box);
      auto [active_ladder, is_at_bottom] = find_ladder(gd.ladders, barrel.ladder_box);
      switch(barrel.state){
        case barrel_state::rolling:
          if(support_.type != support_type::none){
            barrel.position_px.y = support_.surface_px - con::barrel_height_px;
          }
          else {
            barrel.bounces_remaining = con::barrel_bounce_count;
            barrel.fall_start_y = barrel.position_px.y;
            barrel.speed_reduced = 0;
            barrel.state = barrel_state::falling;
            break;
          }
          if(active_ladder && !is_at_bottom && use_ladder(gd, barrel, *active_ladder)){
            barrel.position_px.x = active_ladder->spine_px - con::barrel_wild_half_width_px;
            barrel.velocity.x = 0.f;
            barrel.velocity.y = con::barrel_ladder_speed_px_per_s;
            barrel.start_ladder_y_px = barrel.position_px.y;
            const auto& frames = barrel.type == barrel_type::common ? common_wild_rolling : blue_wild_rolling;
            barrel.animation_.initialise(frames);
            barrel.state = barrel_state::on_ladder;
          }
          break;
          case barrel_state::falling:
            // reduce barrel speed when first fall off a girder (so don't fly off screen due to momentum).
            if((barrel.position_px.y - barrel.fall_start_y > con::barrel_min_fall_height_to_reduce_speed) && !barrel.speed_reduced){
              barrel.velocity.x *= con::barrel_on_fall_speed_reduction;
              barrel.speed_reduced = 1;
            }
            if(support_.type != support_type::none && barrel.velocity.y > 0.f){
              barrel.position_px.y = support_.surface_px - con::barrel_height_px;
              auto fall_height_px = barrel.position_px.y - barrel.fall_start_y;
              if(fall_height_px > con::barrel_min_fall_height_to_reverse){
                if(barrel.bounces_remaining == con::barrel_bounce_count){ // if first bounce.
                  if(!is_jm_above_barrel(gd.jump_man, barrel, 40.f)){ // roll off screen if below jump man.
                    reverse_barrel(barrel);
                  }
                  barrel.velocity.y = 0.f;
                }
                if(barrel.bounces_remaining > 0){
                  barrel.velocity.y -= con::barrel_bounce_speed_px_per_s;
                  barrel.state = barrel_state::falling;
                  --barrel.bounces_remaining;
                }
                else {
                  barrel.velocity.y = 0.f;
                  barrel.state = barrel_state::rolling;
                }
              }
              else{
                barrel.velocity.y = 0.f;
                barrel.state = barrel_state::rolling;
              }
            }
            break;
            case barrel_state::on_ladder:
              if(support_.type != support_type::none &&
              ((barrel.position_px.y - barrel.start_ladder_y_px) > con::barrel_min_ladder_fall_px)){
                barrel.velocity.zero();
                barrel.state = barrel_state::rolling;
                reverse_barrel(barrel);
              }
              break;
      }
      barrel.animation_.update(dt_s);
      if(barrel.state == barrel_state::falling){
        barrel.velocity.y += con::barrel_gravity_px_per_s2 * dt_s;
      }
      barrel.position_px += barrel.velocity * dt_s;
      update_barrel_body_box(barrel);
      update_barrel_ladder_box(barrel);
    }

    // remove barrels which have wandered off screen.
    static const auto is_barrel_off_sreen = [](const barrel_data& barrel) -> bool {
      constexpr auto margin_px = 10.f;
      return (barrel.position_px.x < 0.f - con::barrel_width_px - margin_px) ||
      (barrel.position_px.x > con::world_width_px + margin_px) ||
      (barrel.position_px.y < 0.f - con::barrel_height_px - margin_px) ||
      (barrel.position_px.y > con::world_height_px + margin_px);
    };
    auto new_end = std::remove_if(gd.barrels.begin(), gd.barrels.end(), is_barrel_off_sreen);
    gd.barrels.erase(new_end, gd.barrels.end());
  }

  void update_wilds(game_data& gd, float dt_s)
  {
    for(auto& wild : gd.wilds){
      wild.animation_.update(dt_s);
      if(wild.state == wild_state::falling){
        auto can_rebound =
          wild.position_px.y > (wild.last_rebound_y + con::barrel_wild_height_px + 4.f) &&
          ((wild.wild_type_ == WILD_DROP && wild.rebound_count < 1 && rnd::uniformSignedInt(1, con::wild_drop_hit_chance) == 1) ||
          wild.wild_type_ != WILD_DROP ||
          wild.position_px.y > con::world_height_px - 30 || // to ensure we always test for hitting bottom girder.
          wild.position_px.x > con::wild_right_margin_x && wild.speed_dir.x > 0.f ||
          wild.position_px.x < con::wild_left_margin_x && wild.speed_dir.x < 0.f);
        if(can_rebound){
          auto support_ = find_support(gd, wild.body_box);
          if(support_.type != support_type::none){
            if(support_.type == support_type::girder && support_.flags & FLAG_BOTTOM_GIRDER){
              // hit bottom girder so needs to bounce and roll off screen; do this by replacing the wild
              // with a normal rolling barrel. Note: need to set the barrel up in a state such that it
              // will start bouncing.
              auto barrel = barrel_data{};
              barrel.position_px.x = wild.position_px.x;
              barrel.position_px.y = support_.surface_px - con::barrel_wild_height_px;
              barrel.velocity.x = -1.f * con::barrel_speed_px_per_s;
              barrel.velocity.y = 0.f;
              barrel.speed_reduced = 1;
              barrel.type = wild.barrel_type_;
              auto& animation_frames = wild.barrel_type_ == barrel_type::common ?
                common_barrel_rolling : blue_barrel_rolling;
              barrel.animation_.initialise(animation_frames, animation::mode::reverse_loop);
              barrel.look_dir = look_direction::left;
              barrel.state = barrel_state::falling;
              barrel.fall_start_y = 0.f;
              barrel.bounces_remaining = con::barrel_bounce_count - 1; // -1 to avoid not dir reverse if above jm.
              update_barrel_body_box(barrel);
              update_barrel_ladder_box(barrel);
              gd.barrels.push_back(barrel);
              // set a 'dirty' flag instead of immedietly erasing so as to not invalidate the
              // iterators in this loop.
              wild.flags |= FLAG_WILD_DEAD;
            }
            else{
              wild.position_px.y = support_.surface_px - con::barrel_wild_height_px;
              wild.last_rebound_y = wild.position_px.y;
              ++wild.rebound_count;
              wild.speed_dir = select_wild_rebound_dir(wild, gd.jump_man);
              wild.speed *= con::wild_speed_bounce_reduction;
              wild.rebound_clock_s = 0.f;
              wild.state = wild_state::rebounding;
            }
            continue;
          }
        }
        auto target_speed = con::wild_speeds[wild.wild_type_];
        if(wild.speed < target_speed){
          wild.speed = std::min(wild.speed + (con::wild_acceleration_px_per_s2 * dt_s), target_speed);
        }
        wild.position_px += wild.speed_dir * (wild.speed * dt_s);
        update_wild_body_box(wild);
      }
      else{ // rebounding.
        wild.rebound_clock_s += dt_s;
        if(wild.rebound_clock_s > con::wild_rebound_duration_s){
          wild.state = wild_state::falling;
        }
      }
    }

    gd.wilds.erase(std::remove_if(gd.wilds.begin(), gd.wilds.end(), [](const wild_data& wild){
      return wild.flags & FLAG_WILD_DEAD || wild.position_px.y > con::world_height_px;
    }), gd.wilds.end());
  }

  void update_mobs(game_data& gd, float dt_s)
  {
    gd.mob_ai_tick_clock_s += dt_s;
    if(gd.mob_ai_tick_clock_s > con::mob_ai_tick_interval_s){
      for(auto& mob : gd.mobs){
        if(mob.state == mob_state::roaming){
          // force moving back onto screen if wander off.
          if(mob.center_px.x < 0.f){
            mob.look_dir = look_direction::right;
            mob.velocity.x = con::mob_roam_speed_px_per_s[gd.difficulty - 1];
            continue;
          }
          if(mob.center_px.x > con::world_width_px){
            mob.look_dir = look_direction::left;
            mob.velocity.x = -con::mob_roam_speed_px_per_s[gd.difficulty - 1];
            continue;
          }
          // make a decision to climb a ladder.
          if(rnd::uniformSignedInt(1, con::mob_climb_chance) == 1){
            mob.flags |= FLAG_MOB_CLIMB_NEXT_LADDER;
          }
          // change direction if run out of girder.
          const auto& feeler = mob.look_dir == look_direction::left ? mob.left_feeler : mob.right_feeler;
          const auto felt_support = find_support(gd, feeler);
          if(felt_support.type == support_type::none){
            reverse_mob(mob, con::mob_roam_speed_px_per_s[gd.difficulty - 1]);
            continue;
          }
          // change direction randomly.
          if(rnd::uniformSignedInt(1, con::mob_reverse_chance) == 1){
            reverse_mob(mob, con::mob_roam_speed_px_per_s[gd.difficulty - 1]);
          }
        }
      }
      gd.mob_ai_tick_clock_s = 0.f;
    }

    for(auto& mob : gd.mobs){
      auto support_ = find_support(gd, mob.body_box);
      switch(mob.state){
        case mob_state::roaming: {
          // ensure standing on highest girder.
          if(support_.type != support_type::none){
            mob.center_px.y = support_.surface_px - con::mob_half_height_px[mob.type];
          }
          else{
            mob.state = mob_state::falling;
            break;
          }
          // climb ladder if found one and ready to climb.
          if(mob.flags & FLAG_MOB_CLIMB_NEXT_LADDER){
            auto [active_ladder, is_at_bottom] = find_ladder(gd.ladders, mob.ladder_box);
            if((active_ladder && is_at_bottom) ||
               (active_ladder && !is_at_bottom && mob.center_px.y < gd.jump_man.center_px.y)){
              mob.center_px.x = active_ladder->spine_px;
              mob.velocity.x = 0;
              mob.velocity.y = is_at_bottom ? -con::mob_climb_speed_px_per_s : con::mob_climb_speed_px_per_s;
              mob.climb_start_y_px = mob.center_px.y;
              mob.state = mob_state::climbing;
              mob.flags &= ~FLAG_MOB_CLIMB_NEXT_LADDER;
            }
          }
          break;
        }
        case mob_state::climbing:
          // detect and handle end of climb.
          if(std::abs(mob.center_px.y - mob.climb_start_y_px) > con::mob_height_px[mob.type] + 2.f){
            support_ = mob.velocity.y > 0.f ? find_support(gd, mob.body_box) : find_support(gd, mob.ladder_box);
            if(support_.type != support_type::none){
              mob.center_px.y = support_.surface_px - con::mob_half_height_px[mob.type];
              mob.velocity.x = mob.look_dir == look_direction::left ?
                               -con::mob_roam_speed_px_per_s[gd.difficulty - 1] :
                               con::mob_roam_speed_px_per_s[gd.difficulty - 1];
              mob.velocity.y = 0.f;
              mob.state = mob_state::roaming;
            }
          }
          break;
        case mob_state::falling:
          if(support_.type != support_type::none){
            mob.center_px.y = support_.surface_px - con::mob_half_height_px[mob.type];
            mob.velocity.y = 0.f;
            mob.state = mob_state::roaming;
          }
          break;
      }

      mob.animation_.update(dt_s);

      if(mob.state != mob_state::falling){
        mob.bounce_y_px += con::mob_bounce_speed_px_per_s * mob.bounce_dir;
        if(mob.bounce_y_px < con::mob_bounce_lo_px){
          mob.bounce_dir = 1;
        }
        if(mob.bounce_y_px > con::mob_bounce_hi_px){
          mob.bounce_dir = -1;
        }
      }

      if(mob.state == mob_state::falling){
        mob.velocity.y += con::mob_gravity_px_per_s2 * dt_s;
      }
      auto net_velocity = mob.velocity;
      if(mob.state != mob_state::climbing && support_.type == support_type::conveyor){
        net_velocity += support_.surface_velocity;
      }
      mob.center_px += net_velocity * dt_s;
      update_mob_aabbs(mob);
    }
  }

  void handle_collisions(game_data& gd)
  {
    auto jm = gd.jump_man;
    auto gd_flags = 0;

    static const auto rand_score = []() -> std::pair<int, score_value> {
      switch(rnd::uniformSignedInt(1, 3)){
        case 1: return {300, score_value::score_300};
        case 2: return {500, score_value::score_500};
        case 3: return {800, score_value::score_800};
        default: assert(0);
      }
    };

    if(jm.flags & FLAG_JM_HAMMERING){
      for(auto iter = gd.barrels.begin(); iter != gd.barrels.end(); ++iter){
        if(is_collision(iter->body_box, jm.hammer_box)){
          gd_flags |= FLAG_GAME_FROZEN;
          gd_flags |= FLAG_ENEMY_HIT;
          auto score = iter->type == barrel_type::common ?
            std::make_pair<int, score_value>(300, score_value::score_300) : rand_score();
          gd.score += score.first;
          spawn_score_popup(gd, gd.jump_man.center_px + con::popup_offset_px, score.second);
          spawn_hit_anim(gd, {
            iter->position_px.x + con::barrel_half_width_px,
            iter->position_px.y + con::barrel_half_height_px
          });
          gd.barrels.erase(iter);
          break;
        }
      }
      if(!gd_flags){
        for(auto iter = gd.wilds.begin(); iter != gd.wilds.end(); ++iter){
          if(is_collision(iter->body_box, jm.hammer_box)){
            gd_flags |= FLAG_GAME_FROZEN;
            gd_flags |= FLAG_ENEMY_HIT;
            auto score = iter->barrel_type_ == barrel_type::common ?
              std::make_pair<int, score_value>(300, score_value::score_300) : rand_score();
            gd.score += score.first;
            spawn_score_popup(gd, gd.jump_man.center_px + con::popup_offset_px, score.second);
            spawn_hit_anim(gd, {
              iter->position_px.x + con::barrel_half_width_px,
              iter->position_px.y + con::barrel_half_height_px
            });
            gd.wilds.erase(iter);
            break;
          }
        }
      }
      if(!gd_flags){
        for(auto iter = gd.mobs.begin(); iter != gd.mobs.end(); ++iter){
          if(is_collision(iter->body_box, jm.hammer_box)){
            gd_flags |= FLAG_GAME_FROZEN;
            gd_flags |= FLAG_ENEMY_HIT;
            auto score = rand_score();
            gd.score += score.first;
            spawn_score_popup(gd, gd.jump_man.center_px + con::popup_offset_px, score.second);
            spawn_hit_anim(gd, iter->center_px);
            gd.mobs.erase(iter);
            break;
          }
        }
      }
      if(!gd_flags){
        for(auto iter = gd.tubs.begin(); iter != gd.tubs.end(); ++iter){
          if(is_collision(iter->body_box, jm.hammer_box)){
            gd_flags |= FLAG_GAME_FROZEN;
            gd_flags |= FLAG_ENEMY_HIT;
            auto score = rand_score();
            gd.score += score.first;
            spawn_score_popup(gd, gd.jump_man.center_px + con::popup_offset_px, score.second);
            spawn_hit_anim(gd, {
              std::round(iter->position_px.x + (con::tub_width_px[iter->type] / 2)),
              std::round(iter->position_px.y + (con::tub_height_px[iter->type] / 2))
            });
            gd.tubs.erase(iter);
            break;
          }
        }
      }
      if(gd_flags & FLAG_ENEMY_HIT){
        stop_run_sound();
        play_sound(SND_SMASH, 1.f, false, con::smash_sound_gain);
      }
    }

    if(!global::is_jm_invulnerable){
      if(!gd_flags){
        for(const auto& barrel : gd.barrels){
          if(is_collision(barrel.body_box, jm.body_box)){
            gd_flags |= FLAG_GAME_FROZEN;
            gd_flags |= FLAG_JUMP_MAN_HIT;
            jm.animation_.initialise(*jump_man_animations::jm_hit_animations[jm.character_]);
            break;
          }
        }
      }
      if(!gd_flags){
        for(const auto& wild : gd.wilds){
          if(is_collision(wild.body_box, jm.body_box)){
            gd_flags |= FLAG_GAME_FROZEN;
            gd_flags |= FLAG_JUMP_MAN_HIT;
            jm.animation_.initialise(*jump_man_animations::jm_hit_animations[jm.character_]);
            break;
          }
        }
      }
      if(!gd_flags){
        for(const auto& mob : gd.mobs){
          if(is_collision(mob.body_box, jm.body_box)){
            gd_flags |= FLAG_GAME_FROZEN;
            gd_flags |= FLAG_JUMP_MAN_HIT;
            jm.animation_.initialise(*jump_man_animations::jm_hit_animations[jm.character_]);
            break;
          }
        }
      }
      if(!gd_flags){
        for(const auto& tub : gd.tubs){
          if(is_collision(tub.body_box, jm.body_box)){
            gd_flags |= FLAG_GAME_FROZEN;
            gd_flags |= FLAG_JUMP_MAN_HIT;
            jm.animation_.initialise(*jump_man_animations::jm_hit_animations[jm.character_]);
            break;
          }
        }
      }
      if(!gd_flags){
        for(const auto& spring : gd.springs){
          if(is_collision(spring.body_box, jm.body_box)){
            gd_flags |= FLAG_GAME_FROZEN;
            gd_flags |= FLAG_JUMP_MAN_HIT;
            jm.animation_.initialise(*jump_man_animations::jm_hit_animations[jm.character_]);
            break;
          }
        }
      }
      if(!gd_flags && gd.kong_.flags & FLAG_KONG_ALIVE){
        if(is_collision(jm.body_box, gd.kong_.body_box)){
          gd_flags |= FLAG_GAME_FROZEN;
          gd_flags |= FLAG_JUMP_MAN_HIT;
          jm.animation_.initialise(*jump_man_animations::jm_hit_animations[jm.character_]);
        }
      }
      if(!gd_flags && gd.barrel_kong.flags & FLAG_BARREL_KONG_ALIVE){
        if(is_collision(jm.body_box, gd.barrel_kong.body_box)){
          gd_flags |= FLAG_GAME_FROZEN;
          gd_flags |= FLAG_JUMP_MAN_HIT;
          jm.animation_.initialise(*jump_man_animations::jm_hit_animations[jm.character_]);
        }
      }
      if(gd_flags & FLAG_JUMP_MAN_HIT){
        stop_music();
        stop_run_sound();
        play_sound(SND_DEATH, 1.f, false, con::death_sound_gain);
      }
    }

    for(auto iter = gd.tubs.begin(); iter != gd.tubs.end(); ++iter){
      if(is_collision(gd.oil_can_.body_box, iter->body_box)){
        light_oil_can(gd.oil_can_);
        gd.tubs.erase(iter);
        break;
      }
    }
    for(auto iter = gd.barrels.begin(); iter != gd.barrels.end(); ++iter){
      if(iter->type == barrel_type::blue && is_collision(iter->body_box, gd.oil_can_.body_box)){
        light_oil_can(gd.oil_can_, true);
        gd.barrels.erase(iter);
        break;
      }
    }

    gd.flags |= gd_flags;
  }



  /** scans for jump man successfully jumping over enemies. */
  void scan_for_jumps(game_data& gd)
  {
    if(gd.jump_man.state_ != jump_man_state::jumping)
      return;

    // detects the peak of the jump (the point where jump man changes direction).
    static float last_y_vel = gd.jump_man.velocity.y;
    if(gd.jump_man.velocity.y > 0 && last_y_vel < 0){
      auto jump_count = 0;
      for(const auto& barrel : gd.barrels){
        if(is_collision(gd.jump_man.leap_box, barrel.body_box)){
          jump_count++;
        }
      }
      for(const auto& wild : gd.wilds){
        if(is_collision(gd.jump_man.leap_box, wild.body_box)){
          jump_count++;
        }
      }
      for(const auto& mob : gd.mobs){
        if(is_collision(gd.jump_man.leap_box, mob.body_box)){
          jump_count++;
        }
      }
      if(gd.kong_.flags & FLAG_KONG_ALIVE && is_collision(gd.jump_man.leap_box, gd.kong_.body_box)){
        jump_count++;
      }
      if(jump_count == 1){
        gd.score += 100;
        spawn_score_popup(gd, gd.jump_man.center_px + con::popup_offset_px, score_value::score_100);
      }
      else if(jump_count == 2){
        gd.score += 300;
        spawn_score_popup(gd, gd.jump_man.center_px + con::popup_offset_px, score_value::score_300);
      }
      else if(jump_count >= 3){
        gd.score += 500;
        spawn_score_popup(gd, gd.jump_man.center_px + con::popup_offset_px, score_value::score_500);
      }
      if(jump_count > 0){
        play_sound(SND_SCORE);
      }
    }

    last_y_vel = gd.jump_man.velocity.y;
  }

  void draw_debug(game_data& gd)
  {
    for(const auto& girder : gd.girders){
      al_draw_line(
        girder.left_px,
        girder.surface_px,
        girder.right_px,
        girder.surface_px,
        al_map_rgb(255, 255, 255),
        con::debug_line_thickness
      );
    }
    for(const auto& elevator : gd.elevators){
      al_draw_line(
        elevator.left_px,
        elevator.surface_px,
        elevator.right_px,
        elevator.surface_px,
        al_map_rgb(255, 255, 255),
        con::debug_line_thickness
      );
    }
    for(const auto& conveyor : gd.conveyors){
      al_draw_line(
        conveyor.left_px,
        conveyor.surface_px,
        conveyor.right_px,
        conveyor.surface_px,
        al_map_rgb(255, 255, 255),
        con::debug_line_thickness
      );
    }
    for(const auto& rivet : gd.rivets){
      al_draw_rectangle(
        rivet.hit_body_box.left,
        rivet.hit_body_box.bottom,
        rivet.hit_body_box.right,
        rivet.hit_body_box.top,
        al_map_rgb(255, 120, 0),
        con::debug_line_thickness
      );
      al_draw_rectangle(
        rivet.hit_jump_box.left,
        rivet.hit_jump_box.bottom,
        rivet.hit_jump_box.right,
        rivet.hit_jump_box.top,
        al_map_rgb(255, 120, 0),
        con::debug_line_thickness
      );
    }
    for(const auto& spring : gd.springs){
      al_draw_rectangle(
        spring.body_box.left,
        spring.body_box.bottom,
        spring.body_box.right,
        spring.body_box.top,
        al_map_rgb(0, 255, 0),
        con::debug_line_thickness
      );
    }
    for(const auto& tub : gd.tubs){
      al_draw_rectangle(
        tub.body_box.left,
        tub.body_box.bottom,
        tub.body_box.right,
        tub.body_box.top,
        al_map_rgb(255, 255, 255),
        con::debug_line_thickness
      );
    }
    for(const auto& ladder : gd.ladders){
      al_draw_line(
        ladder.spine_px,
        ladder.bottom_px,
        ladder.spine_px,
        ladder.top_px,
        al_map_rgb(255, 255, 255),
        con::debug_line_thickness
      );
    }
    for(const auto& ladder : gd.moving_ladders){
      al_draw_line(
        ladder.spine_px,
        ladder.bottom_px,
        ladder.spine_px,
        ladder.top_px,
        al_map_rgb(255, 255, 255),
        con::debug_line_thickness
      );
    }
    al_draw_rectangle(
      gd.oil_can_.body_box.left,
      gd.oil_can_.body_box.top,
      gd.oil_can_.body_box.right,
      gd.oil_can_.body_box.bottom,
      al_map_rgb(255, 255, 255),
      con::debug_line_thickness
    );
    for(const auto& pickup : gd.pickups){
      al_draw_rectangle(
        pickup.collect_box.left,
        pickup.collect_box.top,
        pickup.collect_box.right,
        pickup.collect_box.bottom,
        al_map_rgb(0, 0, 255),
        con::debug_line_thickness
      );
    }
    if(gd.kong_.flags & FLAG_KONG_ALIVE){
      al_draw_rectangle(
        gd.kong_.body_box.left,
        gd.kong_.body_box.top,
        gd.kong_.body_box.right,
        gd.kong_.body_box.bottom,
        al_map_rgb(255, 85, 180),
        con::debug_line_thickness
      );
    }
    if(gd.barrel_kong.flags & FLAG_BARREL_KONG_ALIVE){
      al_draw_rectangle(
        gd.barrel_kong.body_box.left,
        gd.barrel_kong.body_box.top,
        gd.barrel_kong.body_box.right,
        gd.barrel_kong.body_box.bottom,
        al_map_rgb(255, 85, 180),
        con::debug_line_thickness
      );
    }
    auto& jm = gd.jump_man;
    al_draw_rectangle(
      jm.body_box.left,
      jm.body_box.top,
      jm.body_box.right,
      jm.body_box.bottom,
      al_map_rgb(255, 85, 180),
      con::debug_line_thickness
    );
    al_draw_rectangle(
      jm.feet_box.left,
      jm.feet_box.top,
      jm.feet_box.right,
      jm.feet_box.bottom,
      al_map_rgb(255, 255, 255),
      con::debug_line_thickness
    );
    al_draw_rectangle(
      jm.leap_box.left,
      jm.leap_box.top,
      jm.leap_box.right,
      jm.leap_box.bottom,
      al_map_rgb(0, 255, 0),
      con::debug_line_thickness
    );
    if(jm.flags & FLAG_JM_HAMMERING){
      al_draw_rectangle(
        jm.hammer_box.left,
        jm.hammer_box.top,
        jm.hammer_box.right,
        jm.hammer_box.bottom,
        al_map_rgb(0, 255, 0),
        con::debug_line_thickness
      );
    }
    for(const auto& barrel : gd.barrels){
      al_draw_rectangle(
        barrel.body_box.left,
        barrel.body_box.top,
        barrel.body_box.right,
        barrel.body_box.bottom,
        al_map_rgb(0, 0, 255),
        con::debug_line_thickness
      );
      al_draw_rectangle(
        barrel.ladder_box.left,
        barrel.ladder_box.top,
        barrel.ladder_box.right,
        barrel.ladder_box.bottom,
        al_map_rgb(0, 255, 255),
        con::debug_line_thickness
      );
    }
    al_draw_line(
      con::wild_left_margin_x,
      0,
      con::wild_left_margin_x,
      con::world_height_px,
      al_map_rgb(40, 40, 40),
      con::debug_line_thickness
    );
    al_draw_line(
      con::wild_right_margin_x,
      0,
      con::wild_right_margin_x,
      con::world_height_px,
      al_map_rgb(40, 40, 40),
      con::debug_line_thickness
    );
    constexpr auto dir_len = 20.f;
    for(const auto& wild : gd.wilds){
      al_draw_rectangle(
        wild.body_box.left,
        wild.body_box.top,
        wild.body_box.right,
        wild.body_box.bottom,
        al_map_rgb(255, 0, 255),
        con::debug_line_thickness
      );
      auto x0 = wild.position_px.x + con::barrel_wild_half_width_px;
      auto y0 = wild.position_px.y + con::barrel_wild_height_px;
      if(wild.wild_type_ == WILD_CALM){
        for(const auto dir : con::narrow_rebound_dirs){
          al_draw_line(
            x0,
            y0,
            x0 + (dir.x * dir_len),
            y0 + (dir.y * dir_len),
            al_map_rgb(100, 100, 100),
            con::debug_line_thickness
          );
        }
      }
      else{
        for(const auto dir : con::wide_rebound_dirs){
          al_draw_line(
            x0,
            y0,
            x0 + (dir.x * dir_len),
            y0 + (dir.y * dir_len),
            al_map_rgb(100, 100, 100),
            con::debug_line_thickness
          );
        }
      }
      al_draw_line(
        x0,
        y0,
        x0 + (wild.speed_dir.x * dir_len),
        y0 + (wild.speed_dir.y * dir_len),
        al_map_rgb(255, 255, 0),
        con::debug_line_thickness
      );
    }
    for(const auto& mob : gd.mobs){
      al_draw_rectangle(
        mob.body_box.left,
        mob.body_box.top,
        mob.body_box.right,
        mob.body_box.bottom,
        al_map_rgb(255, 255, 255),
        con::debug_line_thickness
      );
      al_draw_rectangle(
        mob.ladder_box.left,
        mob.ladder_box.top,
        mob.ladder_box.right,
        mob.ladder_box.bottom,
        al_map_rgb(255, 255, 0),
        con::debug_line_thickness
      );
      al_draw_rectangle(
        mob.left_feeler.left,
        mob.left_feeler.top,
        mob.left_feeler.right,
        mob.left_feeler.bottom,
        al_map_rgb(0, 255, 0),
        con::debug_line_thickness
      );
      al_draw_rectangle(
        mob.right_feeler.left,
        mob.right_feeler.top,
        mob.right_feeler.right,
        mob.right_feeler.bottom,
        al_map_rgb(0, 255, 0),
        con::debug_line_thickness
      );
    }
  }
}

namespace play
{
  void on_enter(game_data& gd)
  {
    assert(gd.current_stage);
    play_music(gd.current_stage->music_track_);
    gd.bonus = gd.start_bonus;
  }

  void on_exit(game_data& gd)
  {
    stop_run_sound();
    stop_climb_sound();
    stop_music();
    stop_countdown_sound();
  }

  void update(game_data& gd, float dt_s)
  {
    assert(gd.current_stage);
    auto& jm = gd.jump_man;

    if(gd.flags & FLAG_ENEMY_HIT){
      gd.hit_anim.animation_.update(dt_s);
      if(gd.hit_anim.animation_.is_done()){
        gd.flags &= ~FLAG_ENEMY_HIT;
        gd.flags &= ~FLAG_GAME_FROZEN;
      }
    }

    if(gd.flags & FLAG_JUMP_MAN_HIT){
      gd.death_freeze_clock_s += dt_s;
      if(gd.death_freeze_clock_s > con::death_freeze_duration_s){
        gd.death_freeze_clock_s = 0.f;
        gd.flags &= ~FLAG_JUMP_MAN_HIT;
        clear_dynamic_world(gd);
        jm_start_dying(jm);
      }
    }
    if(jm.state_ == jump_man_state::dying){
      jm.animation_.update(dt_s);
      if(jm.animation_.is_done()){
        gd.flags = 0;
        --gd.jump_man_life_count;
        jm.flags |= FLAG_JM_DEAD;
        if(gd.jump_man_life_count >= 0){
          play_music(gd.current_stage->music_track_);
          gd.bonus = gd.start_bonus;
          gd.current_stage->reset(gd);
        }
        return;
      }
    }

    if(!(gd.flags & FLAG_GAME_FROZEN)){
      gd.bonus_clock_s += dt_s;
      if(gd.bonus_clock_s > con::bonus_drop_period_s){
        gd.bonus_clock_s = 0.f;
        gd.bonus -= con::bonus_drop;
        if(gd.bonus == 900){
          start_countdown_sound();
          stop_music();
        }
        if(gd.bonus <= 0){
          gd.flags |= FLAG_GAME_FROZEN;
          gd.flags |= FLAG_JUMP_MAN_HIT;
          jm.animation_.initialise(*jump_man_animations::jm_hit_animations[jm.character_]);
          stop_countdown_sound();
          stop_music();
          stop_run_sound();
          play_sound(SND_DEATH, 1.f, false, con::death_sound_gain);
          return;
        }
      }
      gd.current_stage->update(gd, dt_s);
      if(gd.difficulty < con::max_difficulty){
        gd.difficulty_clock_s += dt_s;
        if(gd.difficulty_clock_s > con::difficulty_delta_s){
          ++gd.difficulty;
          gd.difficulty_clock_s = 0.f;
        }
      }
      update_conveyors(gd, dt_s);
      update_moving_ladders(gd, dt_s);
      update_elevators(gd, dt_s);
      update_springs(gd, dt_s);
      update_tubs(gd, dt_s);
      update_jump_man(gd, dt_s);
      update_pauline(gd, dt_s);
      update_pickups(gd);
      update_rivets(gd);
      update_animated_props(gd, dt_s);
      update_popups(gd, dt_s);
      update_oil_can(gd, dt_s);
      update_kong(gd, dt_s);
      update_barrel_kong(gd, dt_s);
      update_barrels(gd, dt_s);
      update_wilds(gd, dt_s);
      update_mobs(gd, dt_s);
      handle_collisions(gd);
      scan_for_jumps(gd);

      if(gd.score == con::score_for_extra_life && !(gd.flags & FLAG_EXTRA_LIFE_EARNED)){
        ++gd.jump_man_life_count;
        gd.flags |= FLAG_EXTRA_LIFE_EARNED;
      }
    }
  }

  void draw(game_data& gd)
  {
    for(const auto& prop : gd.bg_props){
      draw_sprite(
        prop.sid,
        prop.position_px,
        prop.flags & PROP_FLIP_X,
        prop.flags & PROP_FLIP_Y
        );
    }
    for(const auto& ladder : gd.ladders){
      for(auto i = 0; i < ladder.height_blocks; ++i){
        draw_sprite(ladder.sid, vector2f{
          static_cast<float>(ladder.left_px),
          static_cast<float>(ladder.bottom_px + (con::block_size_px * i))
        });
      }
    }
    for(const auto& mover : gd.moving_ladders){
      for(auto i = 1; i <= mover.static_height_blocks; ++i){
        draw_sprite(mover.base_sid, vector2f{
          static_cast<float>(mover.left_px),
          static_cast<float>(mover.top_px - (con::block_size_px * i))
        });
      }
      constexpr auto offset_x = (con::wide_ladder_block_width_px - con::block_size_px) / 2;
      for(auto i = 0; i < mover.moving_height_blocks; ++i){
        draw_sprite(mover.wide_sid, vector2f{
          static_cast<float>(mover.left_px - offset_x),
          static_cast<float>(std::round(mover.bottom_px) + (con::block_size_px * i))
        });
      }
    }
    for(const auto& girder : gd.girders){
      for(auto i = 0; i < girder.width_blocks; ++i){
        draw_sprite(girder.sid, vector2f{
          static_cast<float>(girder.left_px + (con::block_size_px * i)),
          static_cast<float>(girder.surface_px)
        });
      }
    }
    for(const auto& elevator : gd.elevators){
      for(auto i = 0; i < elevator.width_blocks; ++i){
        draw_sprite(elevator.sid, vector2f{
          static_cast<float>(elevator.left_px + (con::block_size_px * i)),
          static_cast<float>(elevator.surface_px)
        });
      }
    }
    for(const auto& conveyor : gd.conveyors){
      for(auto i = 0; i < conveyor.belt_width_blocks; ++i){
        draw_sprite(conveyor.belt_block_sid, vector2f{
          static_cast<float>(conveyor.belt_left_px + (con::block_size_px * i)),
          static_cast<float>(conveyor.surface_px)
        });
      }
      draw_sprite(
        conveyor.left_cap_anim.get_current_frame().sid,
        vector2f{
          static_cast<float>(conveyor.left_cap_left_px),
          static_cast<float>(conveyor.surface_px - 1)
        }
      );
      draw_sprite(
        conveyor.right_cap_anim.get_current_frame().sid,
        vector2f{
          static_cast<float>(conveyor.right_cap_left_px),
          static_cast<float>(conveyor.surface_px - 1)
        },
        true
      );
    }
    for(const auto& rivet : gd.rivets){
      draw_sprite(rivet.sid, vector2f{
        static_cast<float>(rivet.left_px),
        static_cast<float>(rivet.bottom_px)
      });
    }
    for(const auto& prop : gd.animated_props){
      draw_centered_sprite(prop.animation_.get_current_frame().sid, prop.center_px);
    }
    for(const auto& tub : gd.tubs){
      draw_sprite(tub.sid, tub.position_px);
    }
    if(gd.kong_.flags & FLAG_KONG_ALIVE){
      const auto& frame = gd.kong_.animation_.get_current_frame();
      draw_centered_sprite(
        frame.sid,
        gd.kong_.center_px,
        frame.flags & ANIM_FRAME_FLIP_X
      );
    }
    if(gd.barrel_kong.flags & FLAG_BARREL_KONG_ALIVE){
      draw_centered_sprite(
        gd.barrel_kong.kong_sid,
        gd.barrel_kong.center_px,
        gd.barrel_kong.flags & FLAG_BARREL_KONG_FLIP_X
      );
      draw_sprite(gd.barrel_kong.barrel_sid, gd.barrel_kong.barrel_position_px);
    }
    for(const auto& pickup : gd.pickups){
      draw_sprite(pickup.sid, pickup.position_px);
    }
    if(gd.pauline.flags & FLAG_PAULINE_VISIBLE){
      auto pauline_flipped = gd.pauline.flags & FLAG_PAULINE_FLIPPED;
      if(gd.pauline.state == pauline_state::idle){
        draw_centered_sprite(gd.pauline.idle_sid, gd.pauline.center_px, pauline_flipped);
      }
      else{
        draw_centered_sprite(gd.pauline.wiggle_anim.get_current_frame().sid, gd.pauline.center_px, pauline_flipped);
        auto help_sid = pauline_flipped ? gd.pauline.help_flipped_sid : gd.pauline.help_sid;
        int help_offset_x = pauline_flipped ?
          -con::pauline_help_offset.x - con::pauline_help_sign_width_px : con::pauline_help_offset.x;
        draw_sprite(help_sid, vector2f{
          gd.pauline.center_px.x + help_offset_x,
          gd.pauline.center_px.y + con::pauline_help_offset.y
        });
      }
    }
    for(const auto& barrel : gd.barrels){
      draw_sprite(
        barrel.animation_.get_current_frame().sid,
        barrel.position_px
      );
    }
    for(const auto& wild : gd.wilds){
      draw_sprite(
        wild.animation_.get_current_frame().sid,
        wild.position_px
      );
    }
    for(const auto& spring : gd.springs){
      draw_centered_sprite(spring.sid, {spring.center_px.x + spring.shift_x_px, spring.center_px.y});
    }
    for(const auto& mob : gd.mobs){
      draw_centered_sprite(
        mob.animation_.get_current_frame().sid,
        {mob.center_px.x, mob.center_px.y - mob.bounce_y_px},
        mob.look_dir == look_direction::right
      );
    }
    draw_sprite(gd.oil_can_.can_sid, gd.oil_can_.position_px);
    if(gd.oil_can_.state != oil_can_state::not_lit){
      draw_sprite(gd.oil_can_.flames_anim_.get_current_frame().sid, vector2f{
        gd.oil_can_.position_px.x,
        gd.oil_can_.position_px.y - con::oil_can_flames_height_px
      });
    }
    if(!(gd.jump_man.flags & FLAG_JM_DEAD)){
      auto jm = gd.jump_man;
      auto flip_x = false;
      auto flip_y = false;
      // Take into account the look direction.
      flip_x = (jm.state_ == jump_man_state::idle     ||
        jm.state_ == jump_man_state::running  ||
        jm.state_ == jump_man_state::jumping  ||
        jm.state_ == jump_man_state::dying    ||
        gd.flags & FLAG_JUMP_MAN_HIT)         &&
          jm.look_dir == look_direction::right;
      // Take into account the animation flags.
      auto frame = gd.jump_man.animation_.get_current_frame();
      if(frame.flags){
        if(frame.flags & ANIM_FRAME_FLIP_X){
          flip_x = !flip_x;
        }
        if(frame.flags & ANIM_FRAME_FLIP_Y){
          flip_y = !flip_y;
        }
      }
      draw_centered_sprite(frame.sid, gd.jump_man.center_px, flip_x, flip_y);
    }
    if(gd.flags & FLAG_ENEMY_HIT){
      draw_centered_sprite(gd.hit_anim.animation_.get_current_frame().sid, gd.hit_anim.center_px);
    }
    for(const auto& prop : gd.fg_props){
      draw_sprite(
        prop.sid,
        prop.position_px,
        prop.flags & PROP_FLIP_X,
        prop.flags & PROP_FLIP_Y
      );
    }
    for(const auto& popup : gd.popups){
      draw_centered_sprite(popup.sid, popup.center_px);
    }
    {
      draw_sprite(gd.bonus_box.sid, gd.bonus_box.position_px);
      auto txt_pos = gd.bonus_box.position_px + con::bonus_box_text_offset;
      std::stringstream ss{};
      ss << std::fixed << std::setprecision(0) << std::setfill('0') << std::setw(4) << gd.bonus;
      auto txt_color = gd.bonus < 1000 ? gd.bonus_box.warning_txt_color : gd.bonus_box.normal_txt_color;
      draw_text(txt_pos.x, txt_pos.y, ss.str().c_str(), txt_color);
    }
    if(global::is_debug_draw){
      draw_debug(gd);
    }
  }
} // namespace play