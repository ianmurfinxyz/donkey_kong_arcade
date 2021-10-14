#include <allegro5/allegro_ttf.h>
#include <array>
#include "fonts.hh"
#include "../core/log.hh"

static ALLEGRO_FONT* game_font;
static std::array<ALLEGRO_COLOR, TEXT_COLOR_COUNT> text_color_palette;

bool load_fonts()
{
  text_color_palette[TEXT_WHITE] = al_map_rgb(255, 255, 255);
  text_color_palette[TEXT_RED] = al_map_rgb(255, 0, 0);
  text_color_palette[TEXT_BLUE] = al_map_rgb(0, 0, 170);
  text_color_palette[TEXT_GREEN] = al_map_rgb(0, 255, 0);
  text_color_palette[TEXT_YELLOW] = al_map_rgb(255, 184, 0);
  text_color_palette[TEXT_CYAN] = al_map_rgb(0, 255, 255);
  text_color_palette[TEXT_ORANGE] = al_map_rgb(255, 121, 0);
  text_color_palette[TEXT_PINK] = al_map_rgb(255, 33, 85);

  constexpr const char* font_path = "assets/fonts/PressStart2P-Regular.ttf";
  game_font = al_load_ttf_font(font_path, 8, ALLEGRO_TTF_MONOCHROME);
  if(!game_font){
    std::string msg{"failed to load game font '"};
    msg += font_path;
    msg += "'";
    log(log_lvl::fatal, msg);
  }
  return game_font != nullptr;
}

void unload_fonts()
{
  if(game_font) al_destroy_font(game_font);
}

void draw_text(int left_px, int bottom_px, const char* text, text_color color)
{
  al_draw_text(game_font, text_color_palette[color], left_px, bottom_px, 0, text);
}

void draw_text(int left_px, int bottom_px, std::string text, text_color color)
{
  al_draw_text(game_font, text_color_palette[color], left_px, bottom_px, 0, text.c_str());
}
