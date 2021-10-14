#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <cassert>
#include <cstring>
#include <exception>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "hiscores.hh"
#include "../core/log.hh"
#include "../core/input.hh"
#include "../assets/fonts.hh"
#include "../assets/sprites.hh"

namespace hiscores {
  /** Internally maintained hiscores. */
  hiscore_table table;

  constexpr const char* hiscore_filepath = "assets/hiscores.ini";

  const hiscore_table& load_hiscores()
  {
    const auto set_dummy_hiscore = [](int table_index) {
      assert(0 <= table_index && table_index < hiscore_table_size);
      table[table_index].name = "---";
      table[table_index].value = 0;
    };
    const auto is_name_char = [](char c){
      return ('A' <= c && c <= 'Z') || (c == '.') || (c == '-');
    };
    ALLEGRO_CONFIG* score_file = al_load_config_file(hiscore_filepath);
    if(!score_file){
      log(log_lvl::warning, std::string{"failed to load hiscores file; generating a new blank file at '"} + hiscore_filepath + "'");
      for(auto i = 0; i < hiscore_table_size; ++i){
        set_dummy_hiscore(i);
      }
      score_file = al_create_config();
      for(auto i = 0; i < hiscore_table_size; ++i){
        auto istr = std::to_string(i);
        al_set_config_value(score_file, nullptr, (std::string{"NAME"} + istr).c_str(), "---");
        al_set_config_value(score_file, nullptr, (std::string{"SCORE"} + istr).c_str(), "0");
      }
      al_save_config_file(hiscore_filepath, score_file);
      al_destroy_config(score_file);
    }
    else {
      for(auto i = 0; i < hiscore_table_size; ++i){
        auto istr = std::to_string(i);
        auto name_key = std::string{"NAME"} + istr;
        auto score_key = std::string{"SCORE"} + istr;
        auto name = al_get_config_value(score_file, nullptr, name_key.c_str());
        if(!name){
          log(log_lvl::warning, "failed to load hiscore " + name_key);
          set_dummy_hiscore(i);
          continue;
        }
        auto name_len = std::strlen(name);
        if(name_len != 3){
          log(log_lvl::warning, "loaded hiscore " + name_key + " not a valid name; must have 3 characters.");
          set_dummy_hiscore(i);
          continue;
        }
        if(!std::all_of(name, name + name_len, is_name_char)){
          log(log_lvl::warning, "loaded hiscore " + name_key + " not a valid name; must only have characters [A-B] or . or _");
          set_dummy_hiscore(i);
          continue;
        }
        auto score = al_get_config_value(score_file, nullptr, score_key.c_str());
        if(!score){
          log(log_lvl::warning, "failed to load hiscore " + score_key);
          set_dummy_hiscore(i);
          continue;
        }
        try{
          table[i].value = std::stoi(score);
          table[i].name = name;
        }
        catch(const std::exception& e){
          log(log_lvl::warning, "failed to parse score value for hiscore " + score_key + "; must be a valid integer.");
          set_dummy_hiscore(i);
          continue;
        }
      }
      std::sort(table.begin(), table.end(), [](const hiscore& hs_lhs, const hiscore& hs_rhs){
        return hs_lhs.value < hs_rhs.value;
      });
      al_destroy_config(score_file);
    }
    return table;
  }

  void save_hiscores()
  {
    ALLEGRO_CONFIG* score_file = al_create_config();
    for(auto i = 0; i < hiscore_table_size; ++i){
      auto istr = std::to_string(i);
      auto name_key = std::string{"NAME"} + istr;
      auto score_key = std::string{"SCORE"} + istr;
      al_set_config_value(score_file, nullptr, name_key.c_str(), table[i].name.c_str());
      al_set_config_value(score_file, nullptr, score_key.c_str(), std::to_string(table[i].value).c_str());
    }
    al_save_config_file(hiscore_filepath, score_file);
    al_destroy_config(score_file);
  }

  const hiscore_table& get_hiscore_table()
  {
    return table;
  }

  int get_top_hiscore()
  {
    return table[hiscore_table_size - 1].value;
  }

  bool is_hiscore(int new_score)
  {
    return new_score > table[0].value;
  }

  void log_hiscores()
  {
    log(log_lvl::info, "hiscore table");
    log(log_lvl::info, "-------------");
    for(const auto& score : table){
      log(log_lvl::info, std::string{score.name} + " : " + std::to_string(score.value));
    }
  }

  void draw_hiscore_table(int left_px, int bottom_px)
  {
    std::stringstream ss{};
    static const char* prefix[hiscore_table_size] {"1ST", "2ND", "3RD", "4TH", "5TH"};
    for(auto i = 0; i < hiscore_table_size; ++i){
      auto txt_color = (i < hiscore_table_size / 2) ? TEXT_YELLOW : TEXT_RED;
      auto y_px = bottom_px + (i * 16);
      auto table_index = hiscore_table_size - 1 - i;
      draw_text(left_px, y_px, prefix[i], txt_color);
      std::stringstream{}.swap(ss);
      ss << std::fixed << std::setprecision(0) << std::setfill('0') << std::setw(7) << table[table_index].value;
      draw_text(left_px + 35, y_px, ss.str(), txt_color);
      draw_text(left_px + 100, y_px, table[table_index].name, txt_color);
    }
  }
}

namespace hiscores::reg {
  struct key{
    vector2i position_px;
    char key;
  };

  constexpr float rub_end_width_px = 14;
  constexpr float rub_end_height_px = 5;

  static constexpr int char_key_count = 26;
  static constexpr int sym_key_count = 2; // . and _
  static constexpr int ctrl_key_count = 2; // 'rub' and 'end'.
  static constexpr int key_count = char_key_count + sym_key_count + ctrl_key_count;
  static std::array<key, key_count> key_pad {};

  static constexpr int keys_per_row = 10;
  static constexpr int key_spacing = 13;
  static constexpr int key_size = 8;

  static constexpr int focus_box_padding = 2;
  static int focused_key_index;

  static constexpr float key_repeat_period_s = 0.1f;
  static float key_repeat_clock_s = 0.f;

  static char input_name_buffer[score_name_len + 1];
  static int input_count;

  static int input_hiscore;

  static constexpr float reg_time_period_s = 30;
  static constexpr float signal_success_period_s = 3.f;
  static float expiration_clock_s;
  static bool score_registered;
  static bool reg_done;

  void initialise()
  {
    for(auto i = 0; i < char_key_count; ++i){
      auto col = i % keys_per_row;
      auto row = static_cast<int>(i / keys_per_row);
      key_pad[i].position_px = vector2i{48 + (col * key_spacing), 88 + (row * key_spacing)};
      key_pad[i].key = 'A' + i;
    }
    auto index = char_key_count;
    key_pad[index].position_px = {126, 114};
    key_pad[index++].key = '.';
    key_pad[index].position_px = {139, 114};
    key_pad[index++].key = '-';

    key_pad[index].position_px = {152, 115};
    key_pad[index++].key = 'r'; // rub
    key_pad[index].position_px = {170, 115};
    key_pad[index].key = 'e'; // end
  }

  void on_enter(int new_hiscore)
  {
    focused_key_index = 0;
    std::memset(input_name_buffer, '\0', score_name_len);
    input_count = 0;
    input_hiscore = new_hiscore;
    key_repeat_clock_s = 0.f;
    expiration_clock_s = 0.f;
    score_registered = false;
    reg_done = false;
  }

  void update(float dt_s)
  {
    expiration_clock_s += dt_s;
    key_repeat_clock_s += dt_s;
    if((score_registered && expiration_clock_s > signal_success_period_s) ||
       (!score_registered && expiration_clock_s > reg_time_period_s)) reg_done = true;
    if(key_repeat_clock_s > key_repeat_period_s){
      if(input::is_action_triggered(input::ACTION_MENU_PREV)){
        key_repeat_clock_s = 0.f;
        --focused_key_index;
        if(focused_key_index < 0) focused_key_index = key_count - 1;
      }
      if(input::is_action_triggered(input::ACTION_MENU_NEXT)){
        key_repeat_clock_s = 0.f;
        ++focused_key_index;
        if(focused_key_index >= key_count) focused_key_index = 0;
      }
    }
    if(input::is_action_triggered(input::ACTION_MENU_ENTER) && input_count < score_name_len){
      auto input_key = key_pad[focused_key_index].key;
      if(input_key == 'r'){
        if(input_count != 0) input_name_buffer[--input_count] = '\0';
      }
      else if(input_key == 'e'){
        while(input_count < score_name_len) input_name_buffer[input_count++] = '-';
      }
      else{
        input_name_buffer[input_count++] = input_key;
      }
      if(input_count >= score_name_len){
        auto table_pos = 0;
        while(input_hiscore > table[table_pos].value && table_pos < hiscore_table_size)
          ++table_pos;
        table_pos = std::clamp(table_pos - 1, 0, hiscore_table_size - 1);
        std::shift_left(table.begin(), table.begin() + table_pos + 1, 1);
        table[table_pos].name = input_name_buffer;
        table[table_pos].value = input_hiscore;
        score_registered = true;
        expiration_clock_s = 0.f;
      }
    }
  }

  void draw()
  {
    draw_text(48, 48, "NAME REGISTRATION", TEXT_RED);
    draw_text(64, 64, "NAME:___", TEXT_CYAN);
    draw_text(104, 62, input_name_buffer, TEXT_CYAN);

    if(score_registered){
      draw_text(16, 144, "YOUR NAME WAS REGISTERED", TEXT_CYAN);
    }
    else{
      draw_text(55, 144, "REGI TIME", TEXT_CYAN);
      auto time_str = std::to_string(static_cast<int>(reg_time_period_s - expiration_clock_s));
      draw_text(140, 144, "<" + time_str + ">", TEXT_CYAN);
    }

    for(const auto& key : key_pad){
      if(key.key == 'r'){
        draw_sprite(SID_RUB, key.position_px);
      }
      else if(key.key == 'e'){
        draw_sprite(SID_END, key.position_px);
      }
      else{
        char key_str[2] {key.key, '\0'};
        draw_text(key.position_px.x, key.position_px.y, key_str, TEXT_GREEN);
      }
    }

    draw_hiscore_table(20, 160);

    const auto& focused_key = key_pad[focused_key_index];
    if(focused_key.key == 'r' || focused_key.key == 'e'){
      al_draw_rectangle(
        focused_key.position_px.x - focus_box_padding,
        focused_key.position_px.y - focus_box_padding - 2,
        (focused_key.position_px.x + rub_end_width_px) + focus_box_padding,
        (focused_key.position_px.y + rub_end_height_px) + focus_box_padding + 1,
        al_map_rgb(255, 255, 255),
        1
      );
    }
    else{
      al_draw_rectangle(
        focused_key.position_px.x - focus_box_padding,
        focused_key.position_px.y - focus_box_padding - 1,
        (focused_key.position_px.x + key_size) + focus_box_padding,
        (focused_key.position_px.y + key_size) + focus_box_padding - 1,
        al_map_rgb(255, 255, 255),
        1
      );
    }
  }

  bool is_done()
  {
    return reg_done;
  }
}
