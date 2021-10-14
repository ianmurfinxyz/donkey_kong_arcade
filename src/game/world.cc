#include "world.hh"
#include "constants.hh"
#include "../assets/animations.hh"
#include "../assets/sounds.hh"
#include "../core/rand.hh"

void update_jump_man_feet_box(jump_man_data& jm);
void update_jump_man_body_box(jump_man_data& jm);
void update_jump_man_leap_box(jump_man_data& jm);
void update_jump_man_hammer_box(jump_man_data& jm);
void update_barrel_body_box(barrel_data& bd);
void update_barrel_ladder_box(barrel_data& bd);
void update_wild_body_box(wild_data& wd);
void update_mob_aabbs(mob_data& mob);
void update_spring_body_box(spring_data& sd);
void update_tub_aabb(tub_data& tub);
void update_kong_aabb(kong_data& kd);
void update_barrel_kong_aabb(barrel_kong_data& kd);
vector2f select_wild_rebound_dir(const wild_data& wd, const jump_man_data& jm);

void set_mob_vulnerability(game_data& gd, bool is_vulnerable)
{
  for(auto& mob : gd.mobs){
    const auto& frames = is_vulnerable ? *mob_vulnerable_animations[mob.type] : *mob_normal_animations[mob.type];
    mob.animation_.initialise(frames);
  }
}

void light_oil_can(oil_can_data& can, bool birth_fireball)
{
  can.state = oil_can_state::lit_hot;
  if(birth_fireball) can.flags |= FLAG_OIL_CAN_BIRTH_FIREBALL;
  can.flames_anim_.initialise(hot_flames_animation);
  can.hot_clock_s = 0.f;
  play_sound(SND_OIL_LIGHT);
}

void spawn_girder(game_data& gd, int left_px, int surface_px, int width_blocks, girder_color color, int flags)
{
  sprite_id sid{};
  switch(color){
    case girder_color::red:
      sid = SID_GIRDER_RED;
      break;
    case girder_color::orange:
      sid = SID_GIRDER_ORANGE;
      break;
    case girder_color::blue:
      sid = SID_GIRDER_BLUE;
      break;
  }
  girder_data g{};
  g.sid = sid;
  g.left_px = left_px;
  g.right_px = left_px + (width_blocks * con::block_size_px);
  g.surface_px = surface_px;
  g.width_blocks = width_blocks;
  g.flags = flags;
  gd.girders.push_back(g);
}

/**
 * y-axis points down so (+) vertical displacement is down the screen. (+) horizontal displacement is
 * rightward across the screen.
 *
 * A (-) displacement will set the argument position as the max position and move toward the min position,
 * a (+) displacement will set the argument position as the min position and move toward the max.
 *
 * The initial phase is a clamped float in the range [0,1], it is the proportion along the displacement
 * at which to spawn the elevator.
 */
void spawn_elevator(game_data& gd, int left_px, int surface_px, int width_blocks, int move_displacement_px,
                    float initial_phase, girder_color color, elevator_dir dir, elevator_mode mode)
{
  assert(move_displacement_px != 0); // if true use a girder instead!
  sprite_id sid{};
  switch(color){
    case girder_color::red:
      sid = SID_GIRDER_RED;
      break;
    case girder_color::orange:
      sid = SID_GIRDER_ORANGE;
      break;
    case girder_color::blue:
      sid = SID_GIRDER_BLUE;
      break;
  }

  elevator_data ed{};
  ed.sid = sid;
  ed.left_px = left_px;
  ed.right_px = left_px + (width_blocks * con::block_size_px);
  ed.surface_px = surface_px;
  ed.width_blocks = width_blocks;
  ed.flags = 0;
  ed.dir = dir;
  ed.mode = mode;
  ed.rest_clock_s = 0.f;

  std::clamp(initial_phase, 0.f, 1.f);

  if(dir == elevator_dir::vertical){
    if(move_displacement_px > 0){
      ed.move_min_px = surface_px;
      ed.move_max_px = surface_px + move_displacement_px;
      ed.move_dir = 1;
      ed.move_pos_px = std::round(ed.move_min_px + (move_displacement_px * initial_phase));
    }
    else {
      ed.move_min_px = surface_px + move_displacement_px;
      ed.move_max_px = surface_px;
      ed.move_dir = -1;
      ed.move_pos_px = std::round(ed.move_max_px + (move_displacement_px * initial_phase));
    }
  }
  else{
    if(move_displacement_px > 0){
      ed.move_min_px = left_px;
      ed.move_max_px = left_px + move_displacement_px;
      ed.move_dir = 1;
      ed.move_pos_px = std::round(ed.move_min_px + (move_displacement_px * initial_phase));
    }
    else {
      ed.move_min_px = left_px + move_displacement_px;
      ed.move_max_px = left_px;
      ed.move_dir = -1;
      ed.move_pos_px = std::round(ed.move_max_px + (move_displacement_px * initial_phase));
    }
  }

  gd.elevators.push_back(ed);
}

void spawn_ladder(game_data& gd, int left_px, int bottom_px, int height_blocks, ladder_color color, int flags)
{
  sprite_id sid{};
  switch(color){
    case ladder_color::cyan:
      sid = SID_LADDER_CYAN;
      break;
    case ladder_color::yellow:
      sid = SID_LADDER_YELLOW;
      break;
    case ladder_color::white:
      sid = SID_LADDER_WHITE;
      break;
  }
  ladder_data ld{};
  ld.bottom_px = bottom_px;
  ld.top_px = bottom_px + (height_blocks * con::block_size_px);
  ld.spine_px = left_px + con::block_half_size_px;
  ld.flags = flags;
  ld.sid = sid;
  ld.left_px = left_px;
  ld.height_blocks = height_blocks;
  gd.ladders.push_back(ld);
}

void spawn_moving_ladder(game_data& gd, int left_px, int bottom_px, int static_height_blocks, int moving_height_blocks,
                         ladder_color color)
{
  sprite_id base_sid{};
  sprite_id wide_sid{};
  switch(color){
    case ladder_color::cyan:
      base_sid = SID_LADDER_CYAN;
      wide_sid = SID_WIDE_LADDER_CYAN;
      break;
    case ladder_color::yellow:
      base_sid = SID_LADDER_YELLOW;
      wide_sid = SID_WIDE_LADDER_YELLOW;
      break;
    case ladder_color::white:
      base_sid = SID_LADDER_WHITE;
      wide_sid = SID_WIDE_LADDER_WHITE;
      break;
  }
  moving_ladder_data ld{};
  ld.left_px = left_px;
  ld.top_px = bottom_px + (static_height_blocks * con::block_size_px) + (moving_height_blocks * con::block_size_px);
  ld.spine_px = left_px + con::block_half_size_px;
  ld.flags = 0;
  ld.base_sid = base_sid;
  ld.wide_sid = wide_sid;
  ld.static_height_blocks = static_height_blocks;
  ld.moving_height_blocks = moving_height_blocks;
  ld.bottom_extended_px = bottom_px;
  ld.bottom_contracted_px = bottom_px + (moving_height_blocks * con::block_size_px);

  // start the ladder in a fully extended state.
  ld.bottom_px = bottom_px;
  ld.last_move_dir = -1;
  ld.move_dir = 0;
  ld.rest_clock_s = 0.f;
  ld.flags = 0;

  gd.moving_ladders.push_back(ld);
}

void spawn_conveyor(game_data& gd, int left_px, int right_px, int surface_px, int left_cap_left_px,
                    int right_cap_left_px, int belt_left_px, int belt_width_blocks, conveyor_color color)
{
  conveyor_data cd {};
  assert(left_px < right_px);
  cd.left_px = left_px;
  cd.right_px = right_px;
  cd.surface_px = surface_px;
  cd.left_cap_left_px = left_cap_left_px;
  cd.right_cap_left_px = right_cap_left_px;
  cd.belt_left_px = belt_left_px;
  cd.belt_width_blocks = belt_width_blocks;
  cd.belt_dir = look_direction::left;

  if(color == conveyor_color::orange){
    cd.belt_block_sid = SID_CONVEYOR_ORANGE;
    cd.left_cap_anim.initialise(conveyor_left_cap_animation);
    cd.right_cap_anim.initialise(conveyor_right_cap_animation);
  }

  gd.conveyors.push_back(cd);
}

void spawn_spring(game_data& gd, const path_data* path)
{
  assert(path != nullptr);
  assert(path->size() >= 2);
  spring_data sd{};
  sd.sid = SID_SPRING_OPEN;
  sd.center_px = (*path)[0].position_px;
  sd.path = path;
  sd.target_index = 0;
  auto displacement = (*path)[1].position_px - (*path)[0].position_px;
  sd.target_dir = norm(displacement);
  sd.target_distance_px = displacement.length();
  sd.shift_x_px = rnd::uniformSignedInt(-con::spring_max_path_shift_x_px, con::spring_max_path_shift_x_px);
  update_spring_body_box(sd);
  gd.springs.push_back(sd);
}

void spawn_tub(game_data& gd, int left_px, int bottom_px, tub_type type)
{
  tub_data td{};
  td.type = type;
  td.state = tub_state::resting;
  td.position_px.x = left_px;
  td.position_px.y = bottom_px;
  td.velocity.zero();

  if(type == tub_type::TUB_CEMENT)
    td.sid = SID_CEMENT_TUB;
  else
    assert(0);

  update_tub_aabb(td);
  gd.tubs.push_back(td);
}

/** If prop is not a background prop then it's a foreground prop. */
void spawn_prop(game_data& gd, sprite_id sid, float left, float bottom , bool is_bg, int flags)
{
  prop p {};
  p.sid = sid;
  p.position_px = vector2f{left, bottom};
  p.flags = flags;
  if(is_bg)
    gd.bg_props.push_back(p);
  else
    gd.fg_props.push_back(p);
}

void spawn_barrel(game_data& gd, vector2f position_px, look_direction look_dir, barrel_type type)
{
  auto velocity = look_dir == look_direction::left ?
                  vector2f{-1.f * con::barrel_speed_px_per_s, 0.f} :
                  vector2f{1.f * con::barrel_speed_px_per_s, 0.f};

  auto animation_mode = look_dir == look_direction::left ?
                        animation::mode::reverse_loop : animation::mode::loop;

  auto& animation_frames = type == barrel_type::common ?
                           common_barrel_rolling : blue_barrel_rolling;

  auto barrel = barrel_data{};
  barrel.position_px = position_px;
  barrel.velocity = velocity;
  barrel.animation_.initialise(animation_frames, animation_mode);
  barrel.look_dir = look_dir;
  barrel.bounces_remaining = 0;
  barrel.type = type;
  barrel.state = barrel_state::rolling;

  update_barrel_body_box(barrel);
  update_barrel_ladder_box(barrel);

  gd.barrels.push_back(barrel);
}

void spawn_wild(game_data& gd, int left_px, int bottom_px, barrel_type btype, wild_type wtype)
{
  wild_data wd{};
  wd.state = wild_state::falling;
  wd.wild_type_ = wtype;
  wd.barrel_type_ = btype;
  wd.position_px.x = left_px;
  wd.position_px.y = bottom_px;
  wd.last_rebound_y = 0.f;
  wd.rebound_count = 0;
  wd.speed_dir = select_wild_rebound_dir(wd, gd.jump_man);
  wd.speed = con::wild_speeds[wtype] * con::wild_speed_start_percentage;
  const auto& frames = btype == barrel_type::common ? common_wild_rolling : blue_wild_rolling;
  wd.animation_.initialise(frames);
  update_wild_body_box(wd);
  wd.rebound_clock_s = 0.f;
  gd.wilds.push_back(wd);
}

/** note: the position_px is that of the top-left corner of the can, not the flames. */
void spawn_oil_can(game_data& gd, int left_px, int bottom_px, bool start_lit)
{
  gd.oil_can_.position_px.x = left_px;
  gd.oil_can_.position_px.y = bottom_px;
  gd.oil_can_.can_sid = SID_OIL_CAN;
  gd.oil_can_.flames_anim_.initialise(cold_flames_animation);
  gd.oil_can_.state = start_lit ? oil_can_state::lit_cold : oil_can_state::not_lit;
  gd.oil_can_.body_box.left = left_px + con::oil_can_margin_px;
  gd.oil_can_.body_box.right = (left_px + con::oil_can_width_px) - con::oil_can_margin_px;
  gd.oil_can_.body_box.bottom = bottom_px + con::oil_can_margin_px;
  gd.oil_can_.body_box.top = bottom_px + con::oil_can_height_px;
}

void spawn_hammer(game_data& gd, vector2f position_px, hammer_number num)
{
  pickup_data pickup{};
  pickup.pickup_type_ = pickup_type::hammer;
  pickup.position_px = position_px;
  pickup.sid = SID_HAMMER_PICKUP;
  pickup.collect_box.left = position_px.x + con::pickup_collect_margin_px;
  pickup.collect_box.right = (position_px.x + con::hammer_pickup_width_px) - con::pickup_collect_margin_px;
  pickup.collect_box.bottom = position_px.y + con::pickup_collect_margin_px;
  pickup.collect_box.top = (position_px.y + con::hammer_pickup_height_px) - con::pickup_collect_margin_px;
  if(num == hammer_number::hammer_0) pickup.flags |= FLAG_PICKUP_HAMMER_0;
  else if(num == hammer_number::hammer_1) pickup.flags |= FLAG_PICKUP_HAMMER_1;
  gd.pickups.push_back(pickup);
}

void spawn_collectable(game_data& gd, vector2f position_px, collectable_type ctype)
{
  pickup_data pickup{};
  pickup.pickup_type_= pickup_type::collectable;
  pickup.position_px = position_px;
  pickup.collect_box.left = position_px.x + con::pickup_collect_margin_px;
  pickup.collect_box.right = (position_px.x + con::collectable_width_px[ctype]) - con::pickup_collect_margin_px;
  pickup.collect_box.bottom = position_px.y + con::pickup_collect_margin_px;
  pickup.collect_box.top = (position_px.y + con::collectable_height_px[ctype]) - con::pickup_collect_margin_px;

  switch(ctype){
    case COLLECTABLE_PARASOL: {pickup.sid = SID_PARASOL_PICKUP; break;}
    case COLLECTABLE_BAG: {pickup.sid = SID_BAG_PICKUP; break;}
    case COLLECTABLE_HAT: {pickup.sid = SID_HAT_PICKUP; break;}
    default: assert(0);
  }

  gd.pickups.push_back(pickup);
}

void spawn_mob(game_data& gd, mob_type type, vector2f center_px, look_direction look_dir, vector2f launch_velocity)
{
  mob_data mob {};
  mob.type = type;
  mob.animation_.initialise(*mob_normal_animations[type]);
  mob.center_px = center_px;
  mob.bounce_y_px = 0.f;
  mob.bounce_dir = 1;
  mob.climb_start_y_px = 0.f;
  mob.flags = 0;
  update_mob_aabbs(mob);

  if(launch_velocity.x == 0.f && launch_velocity.y == 0.f){
    mob.state = mob_state::roaming;
    mob.look_dir = look_dir;
    mob.velocity.x = look_dir == look_direction::left ?
      -con::mob_roam_speed_px_per_s[gd.difficulty - 1] :
      con::mob_roam_speed_px_per_s[gd.difficulty - 1];
    mob.velocity.y = 0.f;
  }
  else{
    mob.state = mob_state::falling;
    mob.look_dir = launch_velocity.x >= 0.f ? look_direction::right : look_direction::left;
    mob.velocity.x = launch_velocity.x;
    mob.velocity.y = launch_velocity.y;
  }

  gd.mobs.push_back(mob);
}

void spawn_pauline(game_data& gd, vector2f center_px, help_color color, bool can_flip)
{
  gd.pauline.state = pauline_state::idle;
  gd.pauline.idle_sid = SID_PAULINE_IDLE;
  gd.pauline.wiggle_anim.initialise(pauline_wiggle_animation, animation::mode::once);
  gd.pauline.center_px = center_px;
  gd.pauline.help_clock_s = 0.f;
  gd.pauline.flags |= FLAG_PAULINE_VISIBLE;
  if(can_flip) gd.pauline.flags |= FLAG_PAULINE_CAN_FLIP;
  switch(color){
    case help_color::cyan: {gd.pauline.help_sid = SID_HELP_CYAN; gd.pauline.help_flipped_sid = SID_HELP_FLIPPED_CYAN; break;}
    case help_color::blue: {gd.pauline.help_sid = SID_HELP_BLUE; gd.pauline.help_flipped_sid = SID_HELP_FLIPPED_BLUE; break;}
    case help_color::white: {gd.pauline.help_sid = SID_HELP_WHITE; gd.pauline.help_flipped_sid = SID_HELP_FLIPPED_WHITE; break;}
  }
}

void spawn_kong(game_data& gd, vector2f center_px, bool on_conveyor)
{
  gd.kong_.state = kong_state::frowny;
  gd.kong_.animation_.initialise(kong_frowny);
  gd.kong_.center_px = center_px;
  gd.kong_.flags = on_conveyor ? FLAG_KONG_ALIVE | FLAG_KONG_ON_CONVEYOR : FLAG_KONG_ALIVE;
  update_kong_aabb(gd.kong_);
}

void spawn_barrel_kong(game_data& gd, vector2f center_px, look_direction roll_dir, bool force_blue, bool force_wild, bool force_drop_wild)
{
  gd.barrel_kong.state = barrel_kong_state::rest;
  gd.barrel_kong.kong_sid = SID_KONG_FROWNY;
  gd.barrel_kong.barrel_sid = SID_COMMON_BARREL_0;
  gd.barrel_kong.barrel_position_px = vector2f{-100, 0};
  gd.barrel_kong.center_px = center_px;
  gd.barrel_kong.roll_dir = roll_dir;
  gd.barrel_kong.state_clock_s = 0.f;
  gd.barrel_kong.flags |= FLAG_BARREL_KONG_ALIVE;
  update_barrel_kong_aabb(gd.barrel_kong);
  if(force_blue) gd.barrel_kong.flags |= FLAG_BARREL_KONG_FORCE_BLUE_BARREL;
  if(force_wild) gd.barrel_kong.flags |= FLAG_BARREL_KONG_FORCE_WILD_ROLL;
  if(force_drop_wild) gd.barrel_kong.flags |= FLAG_BARREL_KONG_FORCE_DROP_WILD;
}

void spawn_jump_man(game_data& gd, vector2f spawn_position_px)
{
  gd.jump_man.state_ = jump_man_state::idle;
  gd.jump_man.look_dir = look_direction::right;
  gd.jump_man.center_px = spawn_position_px;
  gd.jump_man.velocity.zero();
  gd.jump_man.animation_.initialise(*jump_man_animations::jm_idle_animations[0]); // TODO: set character
  update_jump_man_feet_box(gd.jump_man);
  update_jump_man_body_box(gd.jump_man);
  update_jump_man_leap_box(gd.jump_man);
  gd.jump_man.flags = 0;
}

void spawn_hit_anim(game_data& gd, vector2f center_px)
{
  gd.hit_anim.center_px = center_px;
  gd.hit_anim.animation_.initialise(hit_animation, animation::mode::once);
}

void spawn_score_popup(game_data& gd, vector2f center_px, score_value value)
{
  popup_data pd{};
  pd.center_px = center_px;
  pd.popup_clock_s = 0.f;
  switch(value){
    case score_value::score_100: {pd.sid = SID_SCORE_100; break;}
    case score_value::score_200: {pd.sid = SID_SCORE_200; break;}
    case score_value::score_300: {pd.sid = SID_SCORE_300; break;}
    case score_value::score_500: {pd.sid = SID_SCORE_500; break;}
    case score_value::score_800: {pd.sid = SID_SCORE_800; break;}
  }
  gd.popups.push_back(pd);
}

void spawn_bonus_box(game_data& gd, vector2f position_px, bonus_box_color color)
{
  gd.bonus_box.position_px = position_px;
  switch(color){
    case bonus_box_color::pink:
      gd.bonus_box.sid = SID_PINK_BONUS;
      gd.bonus_box.normal_txt_color = TEXT_CYAN;
      gd.bonus_box.warning_txt_color = TEXT_PINK;
      break;
    case bonus_box_color::blue:
      gd.bonus_box.sid = SID_BLUE_BONUS;
      gd.bonus_box.normal_txt_color = TEXT_YELLOW;
      gd.bonus_box.warning_txt_color = TEXT_BLUE;
      break;
    case bonus_box_color::orange:
      gd.bonus_box.sid = SID_ORANGE_BONUS;
      gd.bonus_box.normal_txt_color = TEXT_WHITE;
      gd.bonus_box.warning_txt_color = TEXT_ORANGE;
      break;
    default: assert(0);
  }
}

void spawn_rivet(game_data& gd, int left_px, int bottom_px)
{
  rivet_data rd{};
  rd.sid = SID_RIVET;
  rd.left_px = left_px;
  rd.bottom_px = bottom_px;
  rd.surface_px = bottom_px + 1;

  rd.hit_body_box.left = left_px + con::rivet_hit_box_margin_px;
  rd.hit_body_box.right = rd.hit_body_box.left + con::rivet_hit_box_width_px;
  rd.hit_body_box.bottom = bottom_px + con::rivet_hit_box_margin_px;
  rd.hit_body_box.top = rd.hit_body_box.bottom + con::rivet_hit_box_height_px;

  rd.hit_jump_box = rd.hit_body_box;
  rd.hit_jump_box.bottom += con::rivet_hit_jump_offset_y_px;
  rd.hit_jump_box.top += con::rivet_hit_jump_offset_y_px;

  gd.rivets.push_back(rd);
}

void clear_static_world(game_data& gd)
{
  gd.girders.clear();
  gd.elevators.clear();
  gd.ladders.clear();
  gd.moving_ladders.clear();
  gd.conveyors.clear();
  gd.fg_props.clear();
  gd.bg_props.clear();
  gd.animated_props.clear();
  spawn_oil_can(gd, -100, -100);
  gd.kong_.flags = 0;
  gd.barrel_kong.flags = 0;
  gd.pauline.flags = 0;
}

void clear_dynamic_world(game_data& gd)
{
  gd.barrels.clear();
  gd.wilds.clear();
  gd.mobs.clear();
  gd.pickups.clear();
  gd.tubs.clear();
  gd.springs.clear();
  gd.popups.clear();
  gd.rivets.clear();
}

void clear_world_actors(game_data& gd)
{
  gd.jump_man.flags |= FLAG_JM_DEAD;
  gd.kong_.flags = 0;
  gd.barrel_kong.flags = 0;
  gd.pauline.flags = 0;
}
