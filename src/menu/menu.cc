#include "menu.hh"
#include "../hiscores/hiscores.hh"
#include "../assets/fonts.hh"
#include "../core/input.hh"
#include "../core/cutscene.hh"
#include "../assets/cutscenes.hh"

namespace menu {
  /** The time to wait before switching to the title screen (screen saver sort of thing). */
  constexpr float wait_period_s = 10.f;
  float clock_s;
  bool is_play_triggered;

  cutscene cutscene_player;

  void on_enter()
  {
    clock_s = 0.f;
    is_play_triggered = false;
    cutscene_player.initialise(cutscenes::menu_barrel_jump);
  }

  void update(float dt_s)
  {
    clock_s += dt_s;

    if(input::is_action_triggered(input::ACTION_MENU_ENTER))
      is_play_triggered = true;

    cutscene_player.update(dt_s);
    if(cutscene_player.is_done()){
      cutscene_player.initialise(cutscenes::menu_barrel_jump);
      cutscene_player.update(0.001f);
    }
  }

  void draw()
  {
    draw_text(32, 56, "PRESS ENTER TO PLAY", TEXT_CYAN);
    draw_text(66, 144, "LEADERBOARD", TEXT_CYAN);
    hiscores::draw_hiscore_table(48, 160);
    cutscene_player.draw();
  }

  bool is_title_time()
  {
    return clock_s > wait_period_s;
  }

  bool is_playtime()
  {
    return is_play_triggered;
  }
}
