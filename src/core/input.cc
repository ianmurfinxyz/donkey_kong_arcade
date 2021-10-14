#include <cassert>
#include <array>
#include "input.hh"

namespace input
{
  struct key_state
  {
    bool is_pressed = false;
    bool is_released = false;
    bool is_down = false;
    bool is_seen = true;
  };

  namespace {
    std::array<key_state, ALLEGRO_KEY_MAX> _key_states;
    const control_scheme* _control_scheme = &arrows_control_scheme;
  }

  bool is_action_triggered(action a)
  {
    auto trigger = action_triggers[a];
    switch(trigger){
      case action_trigger::on_press: {
        auto action_keycode = (*_control_scheme)[a];
        return _key_states[action_keycode].is_pressed;
      }
      case action_trigger::while_down: {
        auto action_keycode = (*_control_scheme)[a];
        return _key_states[action_keycode].is_down;
      }
    }
    assert(0);
  }

  void record_key_pressed(keycode code)
  {
    assert(0 <= code && code < ALLEGRO_KEY_MAX);
    auto& state = _key_states[code];
    state.is_pressed = true;
    state.is_down = true;
    state.is_seen = false;
  }

  void record_key_released(keycode code)
  {
    assert(0 <= code && code < ALLEGRO_KEY_MAX);
    auto& state = _key_states[code];
    if(!state.is_seen){
      state.is_released = true;
    }
    else {
      state.is_down = false;
    }
  }

  void set_control_scheme(const control_scheme& scheme)
  {
    _control_scheme = &scheme;
  }

  void update_key_states()
  {
    for(auto& state: _key_states){
      state.is_pressed = false;
      state.is_seen = true;
      if(state.is_released){
        state.is_down = false;
        state.is_released = false;
      }
    }
  }

} // namespace input
