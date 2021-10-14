#ifndef DONKEY_KONG_1981_LEVELS_HH
#define DONKEY_KONG_1981_LEVELS_HH

#include <array>
#include "../../assets/cutscenes.hh"
#include "../types.hh"
#include "../world.hh"

/**
 * Levels:
 * - The classic game had 4 stages and had the concept of a 'level' which was a
 * set of stages. Levels consisted of stages as follows:
 *
 * Level 1: barrel (25m), rivets (50m)
 * Level 2: barrel (25m), elevator (50m), rivet (75m)
 * Level 3: barrel (25m), factory (50m), elevator (75m), rivet (100m)
 * Level 4: barrel (25m), factory (50m), barrel (75m), elevator (100m), rivet (125m)
 * Level 5: barrel (25m), factory (50m), barrel (75m), elevator (100m), barrel (125m), rivet (150m)
 * Level 6+: repeats the level 5 order.
 *
 * This clone supports extra stages, but I also want to follow the original games design, thus
 * I have divided stages into 4 types: (1) barrel stages, (2) challenge 1 stages, (3) challenge 2 stages
 * and (4) kill stages.
 *
 * The game then follows the same sequence as above but it chooses a random level from each category in
 * order. The classic game has a single member of each category:
 *  - classic barrel stage is in barrel stages category
 *  - classic factory stage is in challenge 1 stages category
 *  - classic elevators stage is in challenge 2 stages category
 *  - classic rivet stage is in kill stages category
 *
 * Thus if classic mode is enabled this clone will follow the order outlined above, if extended mode is
 * enabled then a random level of each category is chosen instead. Thus the stage sequence is effectively
 * as shown below.
 *
 * Level 1: barrel (25m), kill (50m)
 * Level 2: barrel (25m), challenge 2 (50m), kill (75m)
 * Level 3: barrel (25m), challenge 1 (50m), challenge 2 (75m), kill (100m)
 * Level 4: barrel (25m), challenge 1 (50m), barrel (75m), challenge 2 (100m), kill (125m)
 * Level 5: barrel (25m), challenge 1 (50m), barrel (75m), challenge 2 (100m), barrel (125m), kill (150m)
 * Level 6+: repeats the level 5 order.
 */

namespace stage {
  namespace classic_barrels {
    void setup(game_data& gd);
    void reset(game_data& gd);
    void update(game_data& gd, float dt_s);
    bool is_won(game_data& gd);
  }
  namespace classic_factory {
    void setup(game_data& gd);
    void reset(game_data& gd);
    void update(game_data& gd, float dt_s);
    bool is_won(game_data& gd);
  }
  namespace classic_elevators {
    void setup(game_data& gd);
    void reset(game_data& gd);
    void update(game_data& gd, float dt_s);
    bool is_won(game_data& gd);
  }
  namespace classic_rivets {
    void setup(game_data& gd);
    void reset(game_data& gd);
    void update(game_data& gd, float dt_s);
    bool is_won(game_data& gd);
    void on_won(game_data& gd);
  }

  constexpr auto barrel_stage_count = 1;
  constexpr std::array<stage_data, barrel_stage_count> barrel_stages {
    stage_data {
      classic_barrels::setup,
      classic_barrels::reset,
      classic_barrels::update,
      classic_barrels::is_won,
      nullptr,
      &cutscenes::classic_pauline_kidnap,
      &cutscenes::classic_mario_heart_break_hi,
      SND_INTRO,
      {SND_LEVEL_WIN, SND_COUNT},
      music_track::one
    },
  };

  constexpr auto challenge_1_stage_count = 1;
  constexpr std::array<stage_data, challenge_1_stage_count> challenge_1_stages {
    stage_data {
      classic_factory::setup,
      classic_factory::reset,
      classic_factory::update,
      classic_factory::is_won,
      nullptr,
      nullptr,
      &cutscenes::classic_mario_heart_break_lo,
      SND_COUNT,
      {SND_LEVEL_WIN, SND_COUNT},
      music_track::one
    },
  };
  constexpr auto challenge_2_stage_count = 1;
  constexpr std::array<stage_data, challenge_2_stage_count> challenge_2_stages {
    stage_data {
      classic_elevators::setup,
      classic_elevators::reset,
      classic_elevators::update,
      classic_elevators::is_won,
      nullptr,
      nullptr,
      &cutscenes::classic_mario_heart_break_lo,
      SND_COUNT,
      {SND_LEVEL_WIN, SND_COUNT},
      music_track::one
    },
  };
  constexpr auto kill_stage_count = 3;
  constexpr std::array<stage_data, kill_stage_count> kill_stages {
    stage_data {
      classic_rivets::setup,
      classic_rivets::reset,
      classic_rivets::update,
      classic_rivets::is_won,
      classic_rivets::on_won,
      nullptr,
      &cutscenes::classic_kong_fall,
      SND_COUNT,
      {SND_WIN_1, SND_WIN_2},
      music_track::two
    },
  };

}

#endif //DONKEY_KONG_1981_LEVELS_HH
