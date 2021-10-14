#ifndef DONKEY_KONG_1981_FONTS_HH
#define DONKEY_KONG_1981_FONTS_HH

#include <string>

bool load_fonts();
void unload_fonts();

enum text_color {
  TEXT_WHITE,
  TEXT_RED,
  TEXT_BLUE,
  TEXT_GREEN,
  TEXT_YELLOW,
  TEXT_CYAN,
  TEXT_ORANGE,
  TEXT_PINK,
  TEXT_COLOR_COUNT
};

void draw_text(int left_px, int bottom_px, const char* text, text_color color);
void draw_text(int left_px, int bottom_px, std::string text, text_color color);


#endif //DONKEY_KONG_1981_FONTS_HH
