#ifndef DONKEY_KONG_1981_ANIMATIONS_HH
#define DONKEY_KONG_1981_ANIMATIONS_HH

#include "sprites.hh"
#include "../core/animation.hh"
#include "../game/types.hh"

static constexpr auto infinite_delta_s = 1.0e10f;

namespace jump_man_animations::mario
{
  const auto idle = animation_frames{{
    {SID_MARIO_IDLE, 0}
  }, infinite_delta_s};

  const auto run = animation_frames{{
    {SID_MARIO_IDLE, 0},
    {SID_MARIO_RUN_0, 0},
    {SID_MARIO_IDLE, 0},
    {SID_MARIO_RUN_1, 0},
  }, con::jump_man_run_frame_delta_s};

  const auto jump = animation_frames{{
    {SID_MARIO_JUMPING, 0},
  }, infinite_delta_s};

  const auto climbing = animation_frames{{
    {SID_MARIO_CLIMBING, 0},
    {SID_MARIO_CLIMBING, ANIM_FRAME_FLIP_X},
  }, 0.2f};

  const auto climb_idle = animation_frames{{
    {SID_MARIO_CLIMB_IDLE, 0},
  }, infinite_delta_s};

  const auto climb_off = animation_frames{{
    {SID_MARIO_CLIMB_OFF_0, 0},
    {SID_MARIO_CLIMB_OFF_1, 0},
    {SID_MARIO_CLIMB_OFF_2, 0},
  }, 0.10f};

  const auto hit = animation_frames{{
    {SID_MARIO_HIT, 0},
  }, infinite_delta_s};

  const auto dying = animation_frames{{
    {SID_MARIO_DYING_0, 0},
    {SID_MARIO_DYING_1, ANIM_FRAME_FLIP_X},
    {SID_MARIO_DYING_0, ANIM_FRAME_FLIP_Y},
    {SID_MARIO_DYING_1, 0},
    {SID_MARIO_DYING_0, 0},
    {SID_MARIO_DYING_1, ANIM_FRAME_FLIP_X},
    {SID_MARIO_DYING_0, ANIM_FRAME_FLIP_Y},
    {SID_MARIO_DYING_1, 0},
    {SID_MARIO_DYING_0, 0},
    {SID_MARIO_DYING_1, ANIM_FRAME_FLIP_X},
    {SID_MARIO_DYING_0, ANIM_FRAME_FLIP_Y},
    {SID_MARIO_DYING_1, 0},
    {SID_MARIO_DYING_0, 0},
    {SID_MARIO_DYING_2, 0},
    {SID_MARIO_DYING_2, 0},
    {SID_MARIO_DYING_2, 0},
    {SID_MARIO_DYING_2, 0},
    {SID_MARIO_DYING_2, 0},
    {SID_MARIO_DYING_2, 0},
    {SID_MARIO_DYING_2, 0},
    {SID_MARIO_DYING_2, 0},
  }, 0.14f};

  const auto idle_hammering = animation_frames{{
    {SID_MARIO_IDLE_RED_HAMMER_UP, ANIM_FRAME_HAMMER_UP},
    {SID_MARIO_IDLE_RED_HAMMER_DOWN, ANIM_FRAME_HAMMER_DOWN},
  }, con::jump_man_hammer_swing_delta_s};

  const auto run_hammering = animation_frames{{
    {SID_MARIO_IDLE_RED_HAMMER_UP, ANIM_FRAME_HAMMER_UP},
    {SID_MARIO_RUN_0_RED_HAMMER_UP, ANIM_FRAME_HAMMER_UP},
    {SID_MARIO_IDLE_RED_HAMMER_UP, ANIM_FRAME_HAMMER_UP},
    {SID_MARIO_RUN_1_RED_HAMMER_UP, ANIM_FRAME_HAMMER_UP},
    {SID_MARIO_IDLE_RED_HAMMER_DOWN, ANIM_FRAME_HAMMER_DOWN},
    {SID_MARIO_RUN_0_RED_HAMMER_DOWN, ANIM_FRAME_HAMMER_DOWN},
    {SID_MARIO_IDLE_RED_HAMMER_DOWN, ANIM_FRAME_HAMMER_DOWN},
    {SID_MARIO_RUN_1_RED_HAMMER_DOWN, ANIM_FRAME_HAMMER_DOWN},
  }, con::jump_man_run_hammering_frame_delta_s};

  const auto idle_expiring_hammering = animation_frames{{
    {SID_MARIO_IDLE_RED_HAMMER_UP, ANIM_FRAME_HAMMER_UP},
    {SID_MARIO_IDLE_YELLOW_HAMMER_UP, ANIM_FRAME_HAMMER_UP},
    {SID_MARIO_IDLE_RED_HAMMER_DOWN, ANIM_FRAME_HAMMER_DOWN},
    {SID_MARIO_IDLE_YELLOW_HAMMER_DOWN, ANIM_FRAME_HAMMER_DOWN},
    {SID_MARIO_IDLE_RED_HAMMER_UP, ANIM_FRAME_HAMMER_UP},
    {SID_MARIO_IDLE_YELLOW_HAMMER_UP, ANIM_FRAME_HAMMER_UP},
    {SID_MARIO_IDLE_RED_HAMMER_DOWN, ANIM_FRAME_HAMMER_DOWN},
    {SID_MARIO_IDLE_YELLOW_HAMMER_DOWN, ANIM_FRAME_HAMMER_DOWN},
  }, con::jump_man_hammer_swing_delta_s / 2};

  const auto run_expiring_hammering = animation_frames{{
    {SID_MARIO_IDLE_RED_HAMMER_UP, ANIM_FRAME_HAMMER_UP},
    {SID_MARIO_RUN_0_RED_HAMMER_UP, ANIM_FRAME_HAMMER_UP},
    {SID_MARIO_IDLE_YELLOW_HAMMER_UP, ANIM_FRAME_HAMMER_UP},
    {SID_MARIO_RUN_1_YELLOW_HAMMER_UP, ANIM_FRAME_HAMMER_UP},
    {SID_MARIO_IDLE_RED_HAMMER_DOWN, ANIM_FRAME_HAMMER_DOWN},
    {SID_MARIO_RUN_0_RED_HAMMER_DOWN, ANIM_FRAME_HAMMER_DOWN},
    {SID_MARIO_IDLE_YELLOW_HAMMER_DOWN, ANIM_FRAME_HAMMER_DOWN},
    {SID_MARIO_RUN_1_YELLOW_HAMMER_DOWN, ANIM_FRAME_HAMMER_DOWN},
  }, con::jump_man_run_hammering_frame_delta_s};
}

namespace jump_man_animations::luigi
{
  const auto idle = animation_frames{{
    {SID_LUIGI_IDLE, 0}
  }, infinite_delta_s};

  const auto run = animation_frames{{
    {SID_LUIGI_IDLE, 0},
    {SID_LUIGI_RUN_0, 0},
    {SID_LUIGI_IDLE, 0},
    {SID_LUIGI_RUN_1, 0},
  }, 0.15f};

  const auto jump = animation_frames{{
    {SID_LUIGI_JUMPING, 0},
  }, infinite_delta_s};

  const auto climbing = animation_frames{{
    {SID_LUIGI_CLIMBING, 0},
    {SID_LUIGI_CLIMBING, ANIM_FRAME_FLIP_X},
  }, 0.2f};

  const auto climb_idle = animation_frames{{
    {SID_LUIGI_CLIMB_IDLE, 0},
  }, infinite_delta_s};

  const auto climb_off = animation_frames{{
    {SID_LUIGI_CLIMB_OFF_0, 0},
    {SID_LUIGI_CLIMB_OFF_1, 0},
    {SID_LUIGI_CLIMB_OFF_2, 0},
  }, 0.15f};

  const auto hit = animation_frames{{
    {SID_LUIGI_HIT, 0},
  }, infinite_delta_s};

  const auto dying = animation_frames{{
    {SID_LUIGI_DYING_0, 0},
    {SID_LUIGI_DYING_1, 0},
    {SID_LUIGI_DYING_0, 0},
    {SID_LUIGI_DYING_1, 0},
    {SID_LUIGI_DYING_0, 0},
    {SID_LUIGI_DYING_1, 0},
    {SID_LUIGI_DYING_0, 0},
    {SID_LUIGI_DYING_1, 0},
    {SID_LUIGI_DYING_2, 0},
  }, 0.3f};

  const auto idle_hammering = animation_frames{{
    {SID_LUIGI_IDLE_RED_HAMMER_UP, ANIM_FRAME_HAMMER_UP},
    {SID_LUIGI_IDLE_RED_HAMMER_DOWN, ANIM_FRAME_HAMMER_DOWN},
  }, con::jump_man_hammer_swing_delta_s};

  const auto run_hammering = animation_frames{{
    {SID_LUIGI_IDLE_RED_HAMMER_UP, ANIM_FRAME_HAMMER_UP},
    {SID_LUIGI_RUN_0_RED_HAMMER_UP, ANIM_FRAME_HAMMER_UP},
    {SID_LUIGI_IDLE_RED_HAMMER_UP, ANIM_FRAME_HAMMER_UP},
    {SID_LUIGI_RUN_1_RED_HAMMER_UP, ANIM_FRAME_HAMMER_UP},
    {SID_LUIGI_IDLE_RED_HAMMER_DOWN, ANIM_FRAME_HAMMER_DOWN},
    {SID_LUIGI_RUN_0_RED_HAMMER_DOWN, ANIM_FRAME_HAMMER_DOWN},
    {SID_LUIGI_IDLE_RED_HAMMER_DOWN, ANIM_FRAME_HAMMER_DOWN},
    {SID_LUIGI_RUN_1_RED_HAMMER_DOWN, ANIM_FRAME_HAMMER_DOWN},
  }, con::jump_man_run_hammering_frame_delta_s};

  const auto idle_expiring_hammering = animation_frames{{
    {SID_LUIGI_IDLE_RED_HAMMER_UP, ANIM_FRAME_HAMMER_UP},
    {SID_LUIGI_IDLE_YELLOW_HAMMER_UP, ANIM_FRAME_HAMMER_UP},
    {SID_LUIGI_IDLE_RED_HAMMER_DOWN, ANIM_FRAME_HAMMER_DOWN},
    {SID_LUIGI_IDLE_YELLOW_HAMMER_DOWN, ANIM_FRAME_HAMMER_DOWN},
    {SID_LUIGI_IDLE_RED_HAMMER_UP, ANIM_FRAME_HAMMER_UP},
    {SID_LUIGI_IDLE_YELLOW_HAMMER_UP, ANIM_FRAME_HAMMER_UP},
    {SID_LUIGI_IDLE_RED_HAMMER_DOWN, ANIM_FRAME_HAMMER_DOWN},
    {SID_LUIGI_IDLE_YELLOW_HAMMER_DOWN, ANIM_FRAME_HAMMER_DOWN},
  }, con::jump_man_hammer_swing_delta_s / 2};

  const auto run_expiring_hammering = animation_frames{{
    {SID_LUIGI_IDLE_RED_HAMMER_UP, ANIM_FRAME_HAMMER_UP},
    {SID_LUIGI_RUN_0_RED_HAMMER_UP, ANIM_FRAME_HAMMER_UP},
    {SID_LUIGI_IDLE_YELLOW_HAMMER_UP, ANIM_FRAME_HAMMER_UP},
    {SID_LUIGI_RUN_1_YELLOW_HAMMER_UP, ANIM_FRAME_HAMMER_UP},
    {SID_LUIGI_IDLE_RED_HAMMER_DOWN, ANIM_FRAME_HAMMER_DOWN},
    {SID_LUIGI_RUN_0_RED_HAMMER_DOWN, ANIM_FRAME_HAMMER_DOWN},
    {SID_LUIGI_IDLE_YELLOW_HAMMER_DOWN, ANIM_FRAME_HAMMER_DOWN},
    {SID_LUIGI_RUN_1_YELLOW_HAMMER_DOWN, ANIM_FRAME_HAMMER_DOWN},
  }, con::jump_man_run_hammering_frame_delta_s};
}

namespace jump_man_animations
{
  const std::array<const animation_frames*, CHARACTER_COUNT> jm_idle_animations {
    &jump_man_animations::mario::idle,
    &jump_man_animations::luigi::idle,
  };
  const std::array<const animation_frames*, CHARACTER_COUNT> jm_run_animations {
    &jump_man_animations::mario::run,
    &jump_man_animations::luigi::run,
  };
  const std::array<const animation_frames*, CHARACTER_COUNT> jm_jump_animations {
    &jump_man_animations::mario::jump,
    &jump_man_animations::luigi::jump,
  };
  const std::array<const animation_frames*, CHARACTER_COUNT> jm_climbing_animations {
    &jump_man_animations::mario::climbing,
    &jump_man_animations::luigi::climbing,
  };
  const std::array<const animation_frames*, CHARACTER_COUNT> jm_climb_idle_animations {
    &jump_man_animations::mario::climb_idle,
    &jump_man_animations::luigi::climb_idle,
  };
  const std::array<const animation_frames*, CHARACTER_COUNT> jm_climb_off_animations {
    &jump_man_animations::mario::climb_off,
    &jump_man_animations::luigi::climb_off,
  };
  const std::array<const animation_frames*, CHARACTER_COUNT> jm_hit_animations {
    &jump_man_animations::mario::hit,
    &jump_man_animations::luigi::hit,
  };
  const std::array<const animation_frames*, CHARACTER_COUNT> jm_dying_animations {
    &jump_man_animations::mario::dying,
    &jump_man_animations::luigi::dying,
  };
  const std::array<const animation_frames*, CHARACTER_COUNT> jm_idle_hammering_animations {
    &jump_man_animations::mario::idle_hammering,
    &jump_man_animations::luigi::idle_hammering,
  };
  const std::array<const animation_frames*, CHARACTER_COUNT> jm_run_hammering_animations {
    &jump_man_animations::mario::run_hammering,
    &jump_man_animations::luigi::run_hammering,
  };
  const std::array<const animation_frames*, CHARACTER_COUNT> jm_idle_expiring_hammering_animations {
    &jump_man_animations::mario::idle_expiring_hammering,
    &jump_man_animations::luigi::idle_expiring_hammering,
  };
  const std::array<const animation_frames*, CHARACTER_COUNT> jm_run_expiring_hammering_animations {
    &jump_man_animations::mario::run_expiring_hammering,
    &jump_man_animations::luigi::run_expiring_hammering,
  };
}

const auto common_barrel_rolling = animation_frames{{
  {SID_COMMON_BARREL_0, 0},
  {SID_COMMON_BARREL_1, 0},
  {SID_COMMON_BARREL_2, 0},
  {SID_COMMON_BARREL_3, 0},
}, 0.1};

const auto common_wild_rolling = animation_frames{{
  {SID_COMMON_BARREL_WILD_0, 0},
  {SID_COMMON_BARREL_WILD_1, 0},
}, 0.1};

const auto blue_barrel_rolling = animation_frames{{
  {SID_BLUE_BARREL_0, 0},
  {SID_BLUE_BARREL_1, 0},
  {SID_BLUE_BARREL_2, 0},
  {SID_BLUE_BARREL_3, 0},
}, 0.1f};

const auto blue_wild_rolling = animation_frames{{
  {SID_BLUE_BARREL_WILD_0, 0},
  {SID_BLUE_BARREL_WILD_1, 0},
}, 0.1f};

const auto red_firefox = animation_frames{{
  {SID_RED_FIREFOX_0, 0},
  {SID_RED_FIREFOX_1, 0},
}, 0.2f};

const auto blue_firefox = animation_frames{{
  {SID_BLUE_FIREFOX_0, 0},
  {SID_BLUE_FIREFOX_1, 0},
}, 0.2f};

const auto red_fireball = animation_frames{{
  {SID_RED_FIREBALL_0, 0},
  {SID_RED_FIREBALL_1, 0},
}, 0.2f};

const auto blue_fireball = animation_frames{{
  {SID_BLUE_FIREBALL_0, 0},
  {SID_BLUE_FIREBALL_1, 0},
}, 0.2f};

const std::array<const animation_frames*, MOB_COUNT> mob_normal_animations {
  &red_fireball,
  &red_firefox,
};
const std::array<const animation_frames*, MOB_COUNT> mob_vulnerable_animations {
  &blue_fireball,
  &blue_firefox,
};

const auto hit_animation = animation_frames{{
  {SID_HAMMER_BLOW_BIG, 0},
  {SID_HAMMER_BLOW_SMALL, 0},
  {SID_HAMMER_BLOW_BIG, 0},
  {SID_HAMMER_BLOW_SMALL, 0},
  {SID_HAMMER_BLOW_BIG, 0},
  {SID_HAMMER_BLOW_SMALL, 0},
  {SID_HAMMER_BLOW_TINY, 0},
  {SID_HAMMER_BLOW_TINY, 0},
  {SID_HAMMER_BLOW_POP, 0},
  {SID_HAMMER_BLOW_POP, 0},
}, 0.15};

/** note: clockwise rotation is a forward animation loop. */
const auto conveyor_left_cap_animation = animation_frames{{
  {SID_CONVEYOR_ORANGE_CAP_FLAT, 0},
  {SID_CONVEYOR_ORANGE_CAP_DIAG_LEFT, 0},
  {SID_CONVEYOR_ORANGE_CAP_DIAG_RIGHT, 0},
}, 0.15};
const auto conveyor_right_cap_animation = animation_frames{{
  {SID_CONVEYOR_ORANGE_CAP_DIAG_LEFT, 0},
  {SID_CONVEYOR_ORANGE_CAP_FLAT, 0},
  {SID_CONVEYOR_ORANGE_CAP_DIAG_RIGHT, 0},
}, 0.15};

const auto hot_flames_animation = animation_frames{{
  {SID_HOT_FLAME_0, 0},
  {SID_HOT_FLAME_1, 0},
}, 0.3};

const auto cold_flames_animation = animation_frames{{
  {SID_COLD_FLAME_0, 0},
  {SID_COLD_FLAME_1, 0},
}, 0.3};

const auto kong_frowny = animation_frames{{
  {SID_KONG_FROWNY, 0},
}, infinite_delta_s};

const auto kong_beat_chest = animation_frames{{
  {SID_KONG_BEAT_CHEST, 0},
  {SID_KONG_BEAT_CHEST, ANIM_FRAME_FLIP_X},
  {SID_KONG_BEAT_CHEST, 0},
}, 0.5f};

const auto pauline_wiggle_animation = animation_frames{{
  {SID_PAULINE_WIGGLE, 0},
  {SID_PAULINE_IDLE, 0},
  {SID_PAULINE_WIGGLE, 0},
  {SID_PAULINE_IDLE, 0},
  {SID_PAULINE_WIGGLE, 0},
  {SID_PAULINE_IDLE, 0},
  {SID_PAULINE_WIGGLE, 0},
  {SID_PAULINE_IDLE, 0},
  {SID_PAULINE_WIGGLE, 0},
}, 0.15f};


#endif //DONKEY_KONG_1981_ANIMATIONS_HH
