#ifndef DONKEY_KONG_1981_HUD_HH
#define DONKEY_KONG_1981_HUD_HH

namespace hud {
  struct hud_stats {
    int high_score;
    int current_score;
    int level_number;
    int life_count;
  };
  void update(float dt_s);
  void draw(hud_stats stats);
}

#endif //DONKEY_KONG_1981_HUD_HH
