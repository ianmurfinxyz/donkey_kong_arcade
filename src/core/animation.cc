#include <cassert>
#include "animation.hh"

void animation::initialise(const animation_frames& frames, mode m)
{
  _frames = &frames;
  _mode = m;
  reset();
}

void animation::reset()
{
  assert(_frames != nullptr);
  assert(!_frames->frames.empty());
  _current_frame = (_mode == mode::reverse_loop || _mode == mode::reverse_once) ?
    _frames->frames.size() - 1 : 0;
  _state = state::playing;
  _timer_s = 0.f;
}

void animation::update(float dt_s)
{
  assert(_frames != nullptr);
  auto size = _frames->frames.size();
  assert(size != 0);
  assert(0 <= _current_frame && _current_frame < size);
  if(_state == state::done){
    return;
  }
  _timer_s += dt_s;
  auto diff_s = _timer_s - _frames->frame_delta_s;
  if(diff_s > 0.f){
    _timer_s = diff_s;
    switch(_mode){
      case mode::loop:
        ++_current_frame;
        _current_frame %= size;
        break;
      case mode::once:
        ++_current_frame;
        if(_current_frame >= size){
          _state = state::done;
          _current_frame = size - 1;
        }
        break;
      case mode::reverse_loop:
        --_current_frame;
        if(_current_frame > size){
          _current_frame = size - 1;
        }
        break;
      case mode::reverse_once:
        --_current_frame;
        if(_current_frame > size){
          _current_frame = 0;
          _state = state::done;
        }
        break;
    }
  }
}

animation_frame animation::get_current_frame() const
{
  assert(_frames != nullptr);
  assert(!_frames->frames.empty());
  assert(0 <= _current_frame && _current_frame < _frames->frames.size());
  return _frames->frames[_current_frame];
}
