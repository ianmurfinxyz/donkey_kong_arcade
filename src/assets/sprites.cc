#include <array>
#include <cassert>
#include <sstream>
#include <cmath>
#include "sprites.hh"
#include "../core/log.hh"

namespace
{
  /** internal store of all loaded sprite sheets. */
  std::array<sprite_sheet, SSID_COUNT> sprite_sheets;

  /** hard-coded names of all the sprite sheet files. */
  constexpr std::array<const char*, SSID_COUNT> ss_names {
    "jump_man.png",
    "world.png",
    "kong.png",
    "title.png",
  };

  /** hard-coded metadata of all sprites. */
  constexpr std::array<sprite, SID_COUNT> sprites {
    // mario idle
    sprite{SSID_JUMP_MAN, {2, 10, 14, 16}, {0.f, 0.f}},
    // mario run 0
    sprite{SSID_JUMP_MAN, {16, 10, 17, 16}, {0.f, 0.f}},
    // mario run 1
    sprite{SSID_JUMP_MAN, {34, 11, 17, 16}, {0.f, 0.f}},
    // mario climb off 0
    sprite{SSID_JUMP_MAN, {52, 11, 17, 15}, {0.f, 0.f}},
    // mario climb off 1
    sprite{SSID_JUMP_MAN, {68, 14, 19, 11}, {0.f, -1.f}},
    // mario climb off 2
    sprite{SSID_JUMP_MAN, {88, 11, 18, 15}, {0.f, -7.f}},
    // mario climb idle
    sprite{SSID_JUMP_MAN, {106, 11, 16, 15}, {0.f, 0.f}},
    // mario climbing 0
    sprite{SSID_JUMP_MAN, {122, 10, 15, 16}, {0.f, 0.f}},
    // mario jumping
    sprite{SSID_JUMP_MAN, {138, 11, 18, 15}, {0.f, 0.f}},
    // mario stopping
    sprite{SSID_JUMP_MAN, {156, 10, 18, 16}, {0.f, 0.f}},
    // mario dying 0
    sprite{SSID_JUMP_MAN, {175, 11, 18, 15}, {0.f, 0.f}},
    // mario dying 1
    sprite{SSID_JUMP_MAN, {193, 10, 18, 16}, {0.f, 0.f}},
    // mario dying 2
    sprite{SSID_JUMP_MAN, {211, 10, 18, 16}, {0.f, 0.f}},

    // mario move idle - red hammer up
    sprite{SSID_JUMP_MAN, {229, 0, 12, 26}, {0.f, -5.f}},
    // mario move idle - red hammer down
    sprite{SSID_JUMP_MAN, {243, 10, 30, 16}, {-9.f, 0.f}},
    // mario move run 0 - red hammer up
    sprite{SSID_JUMP_MAN, {275, 0, 13, 26}, {0.f, -5.f}},
    // mario move run 0 - red hammer down
    sprite{SSID_JUMP_MAN, {290, 10, 32, 16}, {-9.f, 0.f}},
    // mario move run 1 - red hammer up
    sprite{SSID_JUMP_MAN, {323, 0, 15, 26}, {0.f, -5.f}},
    // mario move run 1 - red hammer down
    sprite{SSID_JUMP_MAN, {340, 11, 32, 15}, {-9.f, 0.f}},
    // mario move idle - yellow hammer up
    sprite{SSID_JUMP_MAN, {374, 0, 12, 26}, {0.f, -5.f}},
    // mario move idle - yellow hammer down
    sprite{SSID_JUMP_MAN, {388, 10, 30, 16}, {-9.f, 0.f}},
    // mario move run 0 - yellow hammer up
    sprite{SSID_JUMP_MAN, {420, 0, 13, 26}, {0.f, -5.f}},
    // mario move run 0 - yellow hammer down
    sprite{SSID_JUMP_MAN, {435, 10, 32, 16}, {-9.f, 0.f}},
    // mario move run 1 - yellow hammer up
    sprite{SSID_JUMP_MAN, {468, 0, 15, 26}, {0.f, -5.f}},
    // mario move run 1 - yellow hammer down
    sprite{SSID_JUMP_MAN, {485, 11, 32, 15}, {-9.f, 0.f}},

    // luigi idle
    sprite{SSID_JUMP_MAN, {2, 36, 14, 16}, {0.f, 0.f}},
    // luigi run 0
    sprite{SSID_JUMP_MAN, {16, 36, 17, 16}, {0.f, 0.f}},
    // luigi run 1
    sprite{SSID_JUMP_MAN, {34, 37, 17, 15}, {0.f, 0.f}},
    // luigi climb off 0
    sprite{SSID_JUMP_MAN, {52, 37, 17, 15}, {0.f, 0.f}},
    // luigi climb off 1
    sprite{SSID_JUMP_MAN, {68, 40, 19, 11}, {0.f, -1.f}},
    // luigi climb off 2
    sprite{SSID_JUMP_MAN, {88, 37, 18, 15}, {0.f, -7.f}},
    // luigi climb idle
    sprite{SSID_JUMP_MAN, {106, 37, 16, 15}, {0.f, 0.f}},
    // luigi climbing 0
    sprite{SSID_JUMP_MAN, {122, 36, 15, 16}, {0.f, 0.f}},
    // luigi jumping
    sprite{SSID_JUMP_MAN, {138, 37, 18, 15}, {0.f, 0.f}},
    // luigi stopping
    sprite{SSID_JUMP_MAN, {156, 36, 18, 16}, {0.f, 0.f}},
    // luigi dying 0
    sprite{SSID_JUMP_MAN, {175, 37, 18, 15}, {0.f, 0.f}},
    // luigi dying 1
    sprite{SSID_JUMP_MAN, {193, 36, 18, 16}, {0.f, 0.f}},
    // luigi dying 2
    sprite{SSID_JUMP_MAN, {211, 36, 18, 16}, {0.f, 0.f}},

    // luigi move idle - red hammer up
    sprite{SSID_JUMP_MAN, {220, 26, 12, 26}, {0.f, 5.f}},
    // luigi move idle - red hammer down
    sprite{SSID_JUMP_MAN, {243, 36, 30, 16}, {7.f, 0.f}},
    // luigi move run 0 - red hammer up
    sprite{SSID_JUMP_MAN, {275, 26, 13, 26}, {0.f, 5.f}},
    // luigi move run 0 - red hammer down
    sprite{SSID_JUMP_MAN, {290, 36, 32, 16}, {8.f, 0.f}},
    // luigi move run 1 - red hammer up
    sprite{SSID_JUMP_MAN, {323, 26, 15, 26}, {0.f, 5.f}},
    // luigi move run 1 - red hammer down
    sprite{SSID_JUMP_MAN, {340, 37, 32, 15}, {8.f, 0.f}},
    // luigi move idle - yellow hammer up
    sprite{SSID_JUMP_MAN, {374, 26, 12, 26}, {0.f, 5.f}},
    // luigi move idle - yellow hammer down
    sprite{SSID_JUMP_MAN, {388, 36, 30, 16}, {7.f, 0.f}},
    // luigi move run 0 - yellow hammer up
    sprite{SSID_JUMP_MAN, {420, 26, 13, 26}, {0.f, 5.f}},
    // luigi move run 0 - yellow hammer down
    sprite{SSID_JUMP_MAN, {435, 36, 32, 16}, {8.f, 0.f}},
    // luigi move run 1 - yellow hammer up
    sprite{SSID_JUMP_MAN, {468, 26, 15, 26}, {0.f, 5.f}},
    // luigi move run 1 - yellow hammer down
    sprite{SSID_JUMP_MAN, {485, 37, 32, 15}, {8.f, 0.f}},

    // girder red
    sprite{SSID_WORLD, {0, 8, 8, 8}},
    // girder orange
    sprite{SSID_WORLD, {0, 0, 8, 8}},
    // girder blue
    sprite{SSID_WORLD, {0, 16, 8, 8}},
    // ladder cyan
    sprite{SSID_WORLD, {16, 0, 8, 8}},
    // ladder yellow
    sprite{SSID_WORLD, {16, 8, 8, 8}},
    // ladder white
    sprite{SSID_WORLD, {16, 16, 8, 8}},

    // common barrel 0
    sprite{SSID_WORLD, {26, 1, 12, 10}},
    // common barrel 1
    sprite{SSID_WORLD, {40, 1, 12, 10}},
    // common barrel 2
    sprite{SSID_WORLD, {54, 1, 12, 10}},
    // common barrel 3
    sprite{SSID_WORLD, {68, 1, 12, 10}},
    // common wild barrel 0
    sprite{SSID_WORLD, {82, 1, 16, 10}},
    // common wild barrel 1
    sprite{SSID_WORLD, {100, 1, 16, 10}},

    // oil barrel 0
    sprite{SSID_WORLD, {26, 13, 12, 10}},
    // oil barrel 1
    sprite{SSID_WORLD, {40, 13, 12, 10}},
    // oil barrel 2
    sprite{SSID_WORLD, {54, 13, 12, 10}},
    // oil barrel 3
    sprite{SSID_WORLD, {68, 13, 12, 10}},
    // oil wild barrel 0
    sprite{SSID_WORLD, {82, 13, 16, 10}},
    // oil wild barrel 1
    sprite{SSID_WORLD, {100, 13, 16, 10}},

    // hot flame 0
    sprite{SSID_WORLD, {119, 3, 16, 16}},
    // hot flame 1
    sprite{SSID_WORLD, {138, 3, 16, 16}},
    // cold flame 0
    sprite{SSID_WORLD, {156, 3, 16, 16}, {1, 0}},
    // cold flame 1
    sprite{SSID_WORLD, {174, 3, 16, 16}, {1, 0}},
    // oil can
    sprite{SSID_WORLD, {191, 3, 16, 16}},
    // rivet
    sprite{SSID_WORLD, {209, 9, 8, 9}},
    // barrel stack
    sprite{SSID_WORLD, {0, 27, 20, 32}},

    // red fireball 0
    sprite{SSID_WORLD, {24, 27, 16, 16}, {0, 0}},
    // red fireball 1
    sprite{SSID_WORLD, {42, 27, 16, 16}, {0, 0}},
    // blue fireball 0
    sprite{SSID_WORLD, {24, 45, 16, 16}, {0, 0}},
    // blue fireball 1
    sprite{SSID_WORLD, {42, 45, 16, 16}, {0, 0}},

    // red firefox 0
    sprite{SSID_WORLD, {59, 30, 15, 12}, {0, 2}},
    // red firefox 1
    sprite{SSID_WORLD, {58, 75, 16, 12}, {0, 2}},
    // blue firefox 0
    sprite{SSID_WORLD, {59, 49, 15, 12}, {0, 2}},
    // blue firefox 1
    sprite{SSID_WORLD, {76, 75, 16, 12}, {0, 2}},

    // spring 0
    sprite{SSID_WORLD, {77, 28, 16, 11}},
    // spring 1
    sprite{SSID_WORLD, {95, 27, 12, 15}},

    // hammer pickup
    sprite{SSID_WORLD, {79, 45, 9, 10}},
    // parasol pickup
    sprite{SSID_WORLD, {130, 40, 16, 15}},
    // hat pickup
    sprite{SSID_WORLD, {110, 47, 15, 8}},
    // bag pickup
    sprite{SSID_WORLD, {94, 46, 9, 9}},

    // orange conveyor block
    sprite{SSID_WORLD, {0, 64, 8, 8}},
    // orange conveyor cap flat
    sprite{SSID_WORLD, {76, 61, 11, 10}},
    // orange conveyor cap diag left
    sprite{SSID_WORLD, {89, 61, 11, 10}},
    // orange conveyor cap diag right
    sprite{SSID_WORLD, {102, 61, 11, 10}},

    // wire fence
    sprite{SSID_WORLD, {136, 59, 32, 24}},

    // hammer blow tiny
    sprite{SSID_WORLD, {111, 29, 6, 6}},
    // hammer blow small
    sprite{SSID_WORLD, {120, 27, 10, 8}},
    // hammer blow big
    sprite{SSID_WORLD, {132, 23, 14, 12}},
    // hammer blow pop
    sprite{SSID_WORLD, {151, 22, 16, 14}},

    // heart full
    sprite{SSID_WORLD, {172, 24, 15, 13}},
    // heart broken
    sprite{SSID_WORLD, {189, 24, 16, 13}},

    // mario life
    sprite{SSID_WORLD, {153, 42, 7, 8}},
    // luigi life
    sprite{SSID_WORLD, {163, 42, 7, 8}},

    // wide white ladder
    sprite{SSID_WORLD, {2, 74, 10, 8}},
    // wide yellow ladder
    sprite{SSID_WORLD, {13, 74, 10, 8}},
    // wide cyan ladder
    sprite{SSID_WORLD, {24, 74, 10, 8}},

    // cement tub
    sprite{SSID_WORLD, {38, 75, 16, 8}},

    // pink bonus
    sprite{SSID_WORLD, {174, 42, 43, 19}},
    // blue bonus
    sprite{SSID_WORLD, {173, 65, 43, 19}},
    // orange bonus
    sprite{SSID_WORLD, {208, 20, 43, 19}},

    // red pole
    sprite{SSID_WORLD, {124, 61, 2, 32}},
    // cyan pole
    sprite{SSID_WORLD, {130, 61, 2, 32}},
    // elevator box
    sprite{SSID_WORLD, {100, 75, 16, 16}},

    // kong frowny
    sprite{SSID_KONG, {0, 2, 40, 32}},
    // kong beat chest
    sprite{SSID_KONG, {42, 2, 46, 32}},
    // kong smiley
    sprite{SSID_KONG, {295, 4, 40, 32}},
    // kong with barrel
    sprite{SSID_KONG, {425, 4, 40, 32}},
    // kong rolling
    sprite{SSID_KONG, {380, 4, 43, 32}},
    // kong climb
    sprite{SSID_KONG, {90, 2, 33, 36}},
    // kong climb pauline 0
    sprite{SSID_KONG, {125, 2, 38, 36}},
    // kong climb pauline 1
    sprite{SSID_KONG, {165, 4, 43, 32}},

    // kong stun 0
    sprite{SSID_KONG, {338, 1, 40, 38}},
    // kong stun 1
    sprite{SSID_KONG, {209, 1, 40, 38}},
    // kong stun 2
    sprite{SSID_KONG, {252, 1, 40, 38}},

    // orangutan
    sprite{SSID_KONG, {466, 4, 46, 32}},

    // pauline idle
    sprite{SSID_WORLD, {221, 42, 15, 22}},
    // pauline wiggle
    sprite{SSID_WORLD, {239, 42, 15, 22}},
    // pauline rescued
    sprite{SSID_WORLD, {221, 69, 16, 22}},
    // pauline arse
    sprite{SSID_WORLD, {239, 71, 16, 13}},

    // help cyan
    sprite{SSID_WORLD, {137, 88, 24, 8}},
    // help blue
    sprite{SSID_WORLD, {164, 88, 24, 8}},
    // help white
    sprite{SSID_WORLD, {190, 88, 24, 8}},
    // help flipped cyan
    sprite{SSID_WORLD, {2, 90, 24, 8}},
    // help flipped blue
    sprite{SSID_WORLD, {28, 90, 24, 8}},
    // help flipped white
    sprite{SSID_WORLD, {54, 90, 24, 8}},

    // score 100
    sprite{SSID_WORLD, {20, 64, 15, 7}},
    // score 200
    sprite{SSID_WORLD, {39, 64, 15, 7}},
    // score 300
    sprite{SSID_WORLD, {82, 91, 15, 7}},
    // score 500
    sprite{SSID_WORLD, {220, 2, 15, 7}},
    // score 800
    sprite{SSID_WORLD, {239, 2, 15, 7}},

    // blue title donkey
    sprite{SSID_TITLE, {0, 42, 200, 40}},
    // blue title kong
    sprite{SSID_TITLE, {204, 42, 174, 40}},
    // red title donkey
    sprite{SSID_TITLE, {0, 0, 200, 40}},
    // red title kong
    sprite{SSID_TITLE, {204, 0, 174, 40}},
    // cyan copy
    sprite{SSID_TITLE, {380, 62, 9, 8}},
    // gold copy
    sprite{SSID_TITLE, {380, 72, 9, 8}},

    // black box
    sprite{SSID_WORLD, {0, 102, 50, 16}},

    // rub
    sprite{SSID_WORLD, {54, 102, 14, 5}},
    // end
    sprite{SSID_WORLD, {70, 102, 14, 5}},
  };

  /** hard-coded filesystem path to the sprite sheets. */
  const std::string ss_path {"assets/sprites/"};
}

bool load_sprite_sheets()
{
  log(log_lvl::info, "loading sprite sheets");

  auto log_loading = [](const char* sheet_name){
    std::stringstream ss{};
    ss << "loading sprite sheet '" << fmt_bold << sheet_name << fmt_clear << "'.";
    log(log_lvl::info, ss.str());
  };

  auto log_load_fail = [](const char* sheet_name) {
    std::stringstream ss{};
    ss << "failed to load sprite sheet '" << fmt_bold << sheet_name << fmt_clear << "'.";
    log(log_lvl::fatal, ss.str());
  };

  for(auto ssid = 0; ssid < SSID_COUNT; ++ssid){
    log_loading(ss_names[ssid]);
    sprite_sheets[ssid] = al_load_bitmap((ss_path + ss_names[ssid]).c_str());
    if(!sprite_sheets[ssid]){
      log_load_fail(ss_names[ssid]);
      return false;
    }
  }

  return true;
}

void unload_sprite_sheets()
{
  log(log_lvl::info, "unloading sprite sheets");
  for(const auto& ss : sprite_sheets){
    al_destroy_bitmap(ss);
  }
}

std::pair<const sprite&, sprite_sheet> get_sprite_data(sprite_id sid)
{
  assert(0 <= sid && sid <= SID_COUNT);
  const auto& sprite = sprites[sid];
  auto sprite_sheet = sprite_sheets[sprite.ssid];
  return {sprite, sprite_sheet};
}

void draw_sprite(sprite_id sid, vector2f position_px, bool flip_x, bool flip_y)
{
  const auto& sprite_data = get_sprite_data(sid);
  auto flags = 0;
  if(flip_x){
    flags |= ALLEGRO_FLIP_HORIZONTAL;
  }
  if(flip_y){
    flags |= ALLEGRO_FLIP_VERTICAL;
  }
  al_draw_bitmap_region(
    sprite_data.second,
    sprite_data.first.clip.x,
    sprite_data.first.clip.y,
    sprite_data.first.clip.w,
    sprite_data.first.clip.h,
    position_px.x + sprite_data.first.offset.x,
    position_px.y + sprite_data.first.offset.y,
    flags
  );
}

void draw_sprite(sprite_id sid, vector2i position_px, bool flip_x, bool flip_y)
{
  vector2f pos{};
  pos.x = static_cast<float>(position_px.x);
  pos.y = static_cast<float>(position_px.y);
  draw_sprite(sid, pos, flip_x, flip_y);
}

void draw_centered_sprite(sprite_id sid, vector2f position_px, bool flip_x, bool flip_y)
{
  const auto& sprite_data = get_sprite_data(sid);
  auto offset_x = sprite_data.first.offset.x;
  auto offset_y = sprite_data.first.offset.y;
  auto flags = 0;
  if(flip_x){
    flags |= ALLEGRO_FLIP_HORIZONTAL;
    offset_x *= -1.f;
  }
  if(flip_y){
    flags |= ALLEGRO_FLIP_VERTICAL;
    offset_y *= -1.f;
  }
  al_draw_bitmap_region(
    sprite_data.second,
    sprite_data.first.clip.x,
    sprite_data.first.clip.y,
    sprite_data.first.clip.w,
    sprite_data.first.clip.h,
   std::floor((position_px.x - sprite_data.first.clip.w * 0.5f) + offset_x),
    std::floor((position_px.y - sprite_data.first.clip.h * 0.5f) + offset_y),
    flags
  );
}

