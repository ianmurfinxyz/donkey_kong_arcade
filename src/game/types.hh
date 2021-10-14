#ifndef DONKEY_KONG_1981_TYPES_HH
#define DONKEY_KONG_1981_TYPES_HH

#include "../assets/sprites.hh"
#include "../assets/fonts.hh"
#include "../assets/sounds.hh"
#include "../core/math.hh"
#include "../core/animation.hh"
#include "../core/cutscene.hh"
#include "game.hh"

enum class look_direction { left, right };

/** Common base data of climbable types. */
constexpr int FLAG_CLIMBABLE_NO_TOP = 0b01;
struct climbable_data {
  int left_px;
  float bottom_px;
  int top_px;
  int spine_px;
  int flags;
};

/** Ladders are just vertical lines in the world. */
enum class ladder_color { cyan, yellow, white };
struct ladder_data : public climbable_data {
  sprite_id sid;
  int height_blocks;
};
struct moving_ladder_data : public climbable_data {
  sprite_id base_sid;
  sprite_id wide_sid;
  int static_height_blocks;
  int moving_height_blocks;
  int bottom_extended_px;
  int bottom_contracted_px;
  int last_move_dir;
  int move_dir;   // +1 down (contracting), -1 up (extending), 0 none (rest state).
  float rest_clock_s;
};

/** Girders are just horizontal lines in the world. */
constexpr int FLAG_BOTTOM_GIRDER = 0b01;
enum class girder_color { red, blue, orange };
struct girder_data {
  sprite_id sid;
  int left_px;
  int right_px;
  int surface_px;
  int width_blocks; // each girder unit is 2 blocks width.
  int flags;
};

enum class elevator_dir { vertical, horizontal };
enum class elevator_mode {
  repeat,    // repeat the same movement (so teleport back to start).
  alternate  // alternate direction to go back and forth.
};
struct elevator_data : public girder_data {
  elevator_dir dir;
  elevator_mode mode;
  float move_pos_px;
  int move_min_px;
  int move_max_px;
  int last_move_dir;
  int move_dir;       // +1 +axis-dir, -1 -axis-dir, 0 (none)
  float rest_clock_s;
};


/** note: the width of a conveyor can be greater than the visible portion; this
 *  allows the cement tubs to travel on/off screen whilst remaining on the conveyor. */
enum class conveyor_color { orange };
struct conveyor_data {
  animation left_cap_anim;
  animation right_cap_anim;
  sprite_id belt_block_sid;
  int left_px;
  int right_px;
  int surface_px;
  int left_cap_left_px;   // where to draw the conveyor caps.
  int right_cap_left_px;
  int belt_left_px;       // where to start drawing the belt.
  int belt_width_blocks;
  look_direction belt_dir;
};

constexpr int FLAG_RIVET_HIT = 0b01;
constexpr int FLAG_RIVET_COLLECTED = 0b10;
struct rivet_data {
  sprite_id sid;
  int left_px;
  int bottom_px;
  int surface_px;       // rivets provide support so jm can walk on them.
  aabb hit_body_box;
  aabb hit_jump_box;
  int flags;
};

struct path_point {
  vector2f position_px;
  int flags;
};
using path_data = std::vector<path_point>;

const int FLAG_OPEN_SPRING = 0b001;
const int FLAG_CLOSE_SPRING = 0b010;
const int FLAG_FALL_SPRING = 0b100;
struct spring_data {
  sprite_id sid;
  aabb body_box;
  vector2f center_px;
  vector2f target_dir; // direction from current position to target.
  float target_distance_px; // linear distance to the target from current position.
  float shift_x_px; // random x-shift to give the springs slight path variations.
  const path_data* path;
  int target_index; // index into path of target point.
};

constexpr int PROP_FLIP_X = 0b01;
constexpr int PROP_FLIP_Y = 0b10;
struct prop {
  vector2f position_px;
  sprite_id sid;
  int flags = 0;
};

struct animated_prop_data {
  vector2f center_px;
  animation animation_;
};

enum class barrel_state { rolling, falling, on_ladder };
enum class barrel_type { common, blue };
struct barrel_data {
  vector2f position_px;
  vector2f velocity;
  animation animation_;
  look_direction look_dir;
  int bounces_remaining;
  int fall_start_y;
  barrel_type type;
  barrel_state state;
  aabb body_box;
  aabb ladder_box;
  int speed_reduced; // temp flag needed in updates.
  float start_ladder_y_px;
};


/** Each type of barrel has different behaviour as follows:
 * drop:
 *  - fast movement.
 *  - drop vertically ignoring girders.
 *  - small chance to hit (rebound off) girder.
 *  - wide range of rebound directions.
 *  - random choice of rebound direction.
 * calm:
 *  - slow movement.
 *  - does not ignore girders.
 *  - narrow range of rebound directions.
 *  - random choice of rebound direction.
 * crazy:
 *  - fast movement.
 *  - does not ignore girders.
 *  - wide range of rebound directions.
 *  - random choice of rebound direction.
 * homing:
 *  - slow movement.
 *  - does not ignore girders.
 *  - wide range of rebound directions.
 *  - selects rebound direction as a function of jump
 *    man position to home in on jump man.
 */
enum wild_type { WILD_DROP, WILD_CALM, WILD_CRAZY, WILD_HOMING, WILD_TYPE_COUNT };
enum class wild_state { rebounding, falling };
constexpr int FLAG_WILD_DEAD = 0b01;
struct wild_data {
  wild_state state;
  wild_type wild_type_;
  barrel_type barrel_type_;
  vector2f position_px;
  vector2f speed_dir;
  float speed;
  animation animation_;
  aabb body_box;
  float last_rebound_y;
  float rebound_clock_s;
  int rebound_count;
  int flags;
};


/** A tub is any object that behaves like a cement tub; a dumb
 * block that falls under gravity without support. */
enum tub_type { TUB_CEMENT, TUB_COUNT };
enum class tub_state { resting, falling };
struct tub_data {
  tub_type type;
  tub_state state;
  sprite_id sid;
  vector2f position_px;
  vector2f velocity;
  aabb body_box;
};


/** When set a mob will be birthed after a hot burn. */
constexpr int FLAG_OIL_CAN_BIRTH_FIREBALL = 0b01;
enum class oil_can_state { not_lit, lit_cold, lit_hot };
struct oil_can_data {
  vector2f position_px;
  sprite_id can_sid;
  animation flames_anim_;
  oil_can_state state;
  aabb body_box;
  float hot_clock_s;
  int flags;
};

constexpr int FLAG_PICKUP_COLLECTED = 0b001;
constexpr int FLAG_PICKUP_HAMMER_0 = 0b010;
constexpr int FLAG_PICKUP_HAMMER_1 = 0b100;
enum class pickup_type {
  hammer,
  collectable
};
enum class hammer_number { hammer_0, hammer_1 };
enum collectable_type {
  COLLECTABLE_NONE = -1,
  COLLECTABLE_PARASOL = 0,
  COLLECTABLE_HAT,
  COLLECTABLE_BAG,
  COLLECTABLE_COUNT
};
struct pickup_data {
  pickup_type pickup_type_;
  vector2f position_px;
  sprite_id sid;
  aabb collect_box;
  int flags;
};


constexpr int FLAG_MOB_VULNERABLE = 0b01;
constexpr int FLAG_MOB_CLIMB_NEXT_LADDER = 0b10;
enum mob_type { MOB_FIREBALL, MOB_FIREFOX, MOB_COUNT };
enum class mob_state { roaming, climbing, falling };
struct mob_data {
  mob_type type;
  mob_state state;
  animation animation_;
  vector2f center_px;
  vector2f velocity;
  look_direction look_dir;
  aabb body_box;
  aabb ladder_box;
  aabb left_feeler;
  aabb right_feeler;
  float bounce_y_px;
  int bounce_dir;  // 1=down, -1=up
  float climb_start_y_px;
  int flags;
};

constexpr int FLAG_KONG_ALIVE = 0b01;
constexpr int FLAG_KONG_ON_CONVEYOR = 0b10;
enum class kong_state { frowny, chest_beating };
struct kong_data {
  kong_state state;
  animation animation_;
  vector2f center_px;
  aabb body_box;
  int flags;
};

/**
 * note: the level can force a wild roll by setting the obvious flag.
 * Once rolled the flag is unset so will have to be set again. Without the
 * force wilds are rolled according to a dice roll.
 *
 * note: the level can also force a blue barrel roll in the same way it forces
 * a wild roll, i.e. with the obvious flag. Once again the flag is reset once
 * rolled.
 *
 * note: can also force the next wild barrel to be a drop wild type again with
 * the obvious flag, and once again the flag is unset once the wild is spawned.
 */
constexpr int FLAG_BARREL_KONG_ALIVE = 0b00001;
constexpr int FLAG_BARREL_KONG_FLIP_X = 0b00010;
constexpr int FLAG_BARREL_KONG_FORCE_WILD_ROLL = 0b00100;
constexpr int FLAG_BARREL_KONG_FORCE_BLUE_BARREL = 0b01000;
constexpr int FLAG_BARREL_KONG_FORCE_DROP_WILD = 0b10000;
enum class barrel_kong_state { rest, grab, wild, roll };
struct barrel_kong_data {
  barrel_kong_state state;
  look_direction roll_dir;
  sprite_id kong_sid;
  sprite_id barrel_sid;
  vector2f center_px;
  vector2f barrel_position_px;
  barrel_type barrel_type_; // the barrel being rolled.
  aabb body_box;
  float state_clock_s;
  int flags;
};

constexpr int FLAG_PAULINE_VISIBLE = 0b001;
constexpr int FLAG_PAULINE_CAN_FLIP = 0b010;
constexpr int FLAG_PAULINE_FLIPPED = 0b100;
enum class pauline_state { idle, calling_help };
enum class help_color { cyan, blue, white };
struct pauline_data {
  pauline_state state;
  sprite_id idle_sid;
  sprite_id help_sid;
  sprite_id help_flipped_sid;
  animation wiggle_anim;
  vector2f center_px;
  float help_clock_s;
  int flags;
};

enum class score_value { score_100, score_200, score_300, score_500, score_800 };

struct popup_data {
  sprite_id sid;
  vector2f center_px;
  float popup_clock_s;
};

enum character {
  CHARACTER_MARIO,
  CHARACTER_LUIGI,
  CHARACTER_COUNT
};

enum class jump_man_state {
  idle,
  running,
  jumping,
  climbing_idle,
  climbing,
  climbing_on_top,
  climbing_off_top,
  dying
};

constexpr int FLAG_JM_DEAD = 0b001;
constexpr int FLAG_JM_HAMMERING = 0b010;
constexpr int FLAG_JM_HAMMER_EXPIRING = 0b100;

struct jump_man_data {
  character character_;
  jump_man_state state_;
  look_direction look_dir;
  vector2f center_px;
  vector2f velocity;
  animation animation_;
  aabb feet_box;
  aabb body_box;
  aabb leap_box;
  aabb hammer_box;
  float fall_start_y_px;
  float hammer_timer_s;
  int flags;
};

/** The music options available to levels. */
enum class music_track { one, two, hammer };

enum class bonus_box_color { pink, blue, orange };
struct bonus_box_data {
  sprite_id sid = SID_PINK_BONUS;
  vector2f position_px = {};
  text_color normal_txt_color;
  text_color warning_txt_color;
};

/**
 * note: cutscenes are optional. If a level has an entrance cutscene it will
 * play before the intermission, if a level has an exit cutscene it will play
 * after the level objective has been completed. The exit cutscene will play
 * (draw) over the world, the entrance cutscene will be drawn in an empty
 * black screen. If the corresponding sounds are not SND_COUNT they will be
 * played upon starting the cutscene.
 *
 * note: the exit cutscene has the option for 2 exit sounds. If it has 2 non
 * SND_COUNT sounds one will be selected at random (this was added specifically
 * to allow for the 2 possible variations of the win sound on the rivets stage).
 */
struct game_data;
struct stage_data {
  void (*setup)(game_data&) = nullptr;
  void (*reset)(game_data&) = nullptr;
  void (*update)(game_data&, float dt_s) = nullptr;
  bool (*is_won)(game_data&) = nullptr;
  void (*on_won)(game_data&) = nullptr;   /** Optional; called after is_won returns true.*/
  const cutscene_frames* enter_cutscene = nullptr;
  const cutscene_frames* exit_cutscene = nullptr;
  sound_id enter_cutscene_sound = SND_COUNT;
  sound_id exit_cutscene_sounds[2] = {SND_COUNT, SND_COUNT};
  music_track music_track_;
};

enum class game_state {
  enter_cutscene,
  intermission,
  play,
  game_over,
  exit_cutscene,
};

constexpr int FLAG_GAME_FROZEN = 0b0000000001;
constexpr int FLAG_ENEMY_HIT = 0b0000000010;
constexpr int FLAG_JUMP_MAN_HIT = 0b0000000100;
constexpr int FLAG_GAME_OVER = 0b0100000000;
constexpr int FLAG_EXTRA_LIFE_EARNED = 0b1000000000;

struct game_data {
  const stage_data* current_stage;
  std::vector<girder_data> girders;
  std::vector<elevator_data> elevators;
  std::vector<spring_data> springs;
  std::vector<ladder_data> ladders;
  std::vector<moving_ladder_data> moving_ladders;
  std::vector<conveyor_data> conveyors;
  std::vector<tub_data> tubs;
  std::vector<prop> fg_props;
  std::vector<prop> bg_props;
  std::vector<animated_prop_data> animated_props;
  std::vector<barrel_data> barrels;
  std::vector<wild_data> wilds;
  std::vector<mob_data> mobs;
  std::vector<pickup_data> pickups;
  std::vector<popup_data> popups;
  std::vector<rivet_data> rivets;
  oil_can_data oil_can_;
  jump_man_data jump_man;
  kong_data kong_;
  barrel_kong_data barrel_kong;
  pauline_data pauline;
  animated_prop_data hit_anim;
  bonus_box_data bonus_box;
  cutscene cutscene_player;
  game_state state;
  int level_number;
  int stage_number;
  const climbable_data* active_climbable;
  int difficulty;
  float difficulty_clock_s;
  float mob_ai_tick_clock_s;
  float death_freeze_clock_s;
  float conveyor_tick_clock_s;
  float mob_spawn_clock_s;
  float kong_beat_chest_clock_s;
  float game_over_clock_s;
  float bonus_clock_s;
  int jump_man_life_count;
  int score;
  int start_bonus;
  int bonus;
  int flags;
};

#endif //DONKEY_KONG_1981_TYPES_HH
