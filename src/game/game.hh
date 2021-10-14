#ifndef DONKEY_KONG_1981_GAME_HH
#define DONKEY_KONG_1981_GAME_HH

#include <memory>

struct game_stats {
  int level_number;
  int life_count;
  int score;
};
struct game_data;

namespace game {
  /** Constructs new instance of opaque type game_data. */
  std::unique_ptr<game_data> create();

  /** Called when beginning a new game, prior to update. */
  void on_enter(game_data& gd);

  /** Called once per game tick (potentially multiple times per frame). */
  void update(game_data& gd, float dt_s);

  /** Returns true if gameplay is over. */
  bool is_done(game_data& gd);

  /** Called once per frame to draw the game. */
  void draw(game_data& gd);

  /** Extract the stats of the current game (needed for the HUD). */
  game_stats get_game_stats(game_data& gd);
}

#endif //DONKEY_KONG_1981_GAME_HH
