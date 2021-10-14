#ifndef DONKEY_KONG_1981_MENU_HH
#define DONKEY_KONG_1981_MENU_HH

namespace menu {
  void on_enter();
  void update(float dt_s);
  void draw();
  bool is_title_time();
  bool is_playtime();
}

#endif //DONKEY_KONG_1981_MENU_HH
