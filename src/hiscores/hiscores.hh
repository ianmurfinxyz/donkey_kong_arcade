#ifndef DONKEY_KONG_1981_HISCORES_HH
#define DONKEY_KONG_1981_HISCORES_HH

#include <array>
#include <string>

namespace hiscores {
  /** An entry into the high score table. */
  constexpr int score_name_len {3};
  struct hiscore {
    std::string name;
    int value;
  };

  /** Lower indices have lower scores. */
  constexpr int hiscore_table_size = 5;
  using hiscore_table = std::array<hiscore, hiscore_table_size>;

  /** Load the hiscore table from disk. Creates an empty table if fails to load. */
  const hiscore_table& load_hiscores();

  /** Write the hiscore table to disk. */
  void save_hiscores();

  /** Provides access to the internally maintained hiscore table. */
  const hiscore_table& get_hiscore_table();

  /** Return the value of the score at the top of the leaderboard. */
  int get_top_hiscore();

  /** Tests if the arg score should be inserted into the hiscore table. */
  bool is_hiscore(int new_score);

  /** Logs all hiscores to stdout. */
  void log_hiscores();

  void draw_hiscore_table(int left_px, int bottom_px);
}

/** The registration state which allows the user to enter a name for their new hiscore. This
 * state will insert the new score into the table once the name is entered. */
namespace hiscores::reg {
  /** Must be called before any other method in the reg namespace. */
  void initialise();

  /** Should only enter if the score is actually a new hiscore. */
  void on_enter(int new_hiscore);

  void update(float dt_s);
  void draw();
  bool is_done();
}

#endif //DONKEY_KONG_1981_HISCORES_HH
