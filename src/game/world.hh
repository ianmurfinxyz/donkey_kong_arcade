#ifndef DONKEY_KONG_1981_WORLD_HH
#define DONKEY_KONG_1981_WORLD_HH

#include "types.hh"
#include "../core/math.hh"

void set_mob_vulnerability(game_data& gd, bool is_vulnerable);
void light_oil_can(oil_can_data& can, bool birth_fireball = false);
void spawn_girder(game_data& gd, int left_px, int surface_px, int width_blocks, girder_color color, int flags = 0);
void spawn_elevator(game_data& gd, int left_px, int surface_px, int width_blocks, int move_displacement_px, float initial_phase, girder_color color, elevator_dir dir, elevator_mode mode);
void spawn_ladder(game_data& gd, int left_px, int bottom_px, int height_blocks, ladder_color color, int flags = 0);
void spawn_moving_ladder(game_data& gd, int left_px, int bottom_px, int static_height_blocks, int moving_height_blocks, ladder_color color);
void spawn_conveyor(game_data& gd, int left_px, int right_px, int surface_px, int left_cap_left_px, int right_cap_left_px, int belt_left_px, int belt_width_blocks, conveyor_color color);
void spawn_spring(game_data& gd, const path_data* path);
void spawn_tub(game_data& gd, int left_px, int bottom_px, tub_type type);
void spawn_prop(game_data& gd, sprite_id sid, float left, float bottom , bool is_bg = true, int flags = 0);
void spawn_barrel(game_data& gd, vector2f position_px, look_direction look_dir, barrel_type type);
void spawn_wild(game_data& gd, int left_px, int bottom_px, barrel_type btype, wild_type wtype);
void spawn_oil_can(game_data& gd, int left_px, int bottom_px, bool start_lit = false);
void spawn_hammer(game_data& gd, vector2f position_px, hammer_number num);
void spawn_collectable(game_data& gd, vector2f position_px, collectable_type ctype);
void spawn_mob(game_data& gd, mob_type type, vector2f center_px, look_direction look_dir, vector2f launch_velocity = {});
void spawn_pauline(game_data& gd, vector2f center_px, help_color color, bool can_flip = false);
void spawn_kong(game_data& gd, vector2f center_px, bool on_conveyor);
void spawn_barrel_kong(game_data& gd, vector2f center_px, look_direction roll_dir, bool force_blue = false, bool force_wild = false, bool force_drop_wild = false);
void spawn_jump_man(game_data& gd, vector2f spawn_position_px);
void spawn_hit_anim(game_data& gd, vector2f center_px);
void spawn_score_popup(game_data& gd, vector2f center_px, score_value value);
void spawn_bonus_box(game_data& gd, vector2f position_px, bonus_box_color color);
void spawn_rivet(game_data& gd, int left_px, int bottom_px);
void clear_static_world(game_data& gd);
void clear_dynamic_world(game_data& gd);
void clear_world_actors(game_data& gd); // hides jump man, pauline and kong.

#endif //DONKEY_KONG_1981_WORLD_HH
