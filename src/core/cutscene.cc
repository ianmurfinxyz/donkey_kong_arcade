#include <algorithm>
#include "cutscene.hh"

void cutscene::initialise(const cutscene_frames& frames)
{
  _cutscene_frames = &frames;
  _rd_static_frames.clear();
  _rd_moving_frames.clear();
  _next_static_frame = 0;
  _next_moving_frame = 0;
  _clock_s = 0.f;
  _frames_done = 0;
  _frame_count = frames.static_frames.size() + frames.moving_frames.size();
  _state = _frame_count ? state::playing : state::done;
}

void cutscene::update(float dt_s)
{
  if(_state == state::done){
    return;
  }

  auto time_s = _clock_s + dt_s;

  // remove expired static frames.
  {
    auto new_end = std::remove_if(
      _rd_static_frames.begin(),
      _rd_static_frames.end(),
      [time_s](const static_run_data& d){ return time_s > d.frame->t1_s; }
      );
    _frames_done += std::distance(new_end, _rd_static_frames.end());
    _rd_static_frames.erase(new_end, _rd_static_frames.end());
  }

  // remove expired moving frames.
  {
    auto new_end = std::remove_if(
      _rd_moving_frames.begin(),
      _rd_moving_frames.end(),
      [time_s](const moving_run_data& d){ return time_s > d.frame->t1_s; }
      );
    _frames_done += std::distance(new_end, _rd_moving_frames.end());
    _rd_moving_frames.erase(new_end, _rd_moving_frames.end());
  }


  // add static frames that have become due.
  bool dirty_flag = false;
  while(_next_static_frame < _cutscene_frames->static_frames.size() &&
        time_s > _cutscene_frames->static_frames[_next_static_frame].t0_s){
    _rd_static_frames.push_back(static_run_data{
      &_cutscene_frames->static_frames[_next_static_frame++]
    });
    dirty_flag = true;
  }
  if(dirty_flag){
    std::sort(_rd_static_frames.begin(), _rd_static_frames.end(), [](const auto& rd0, const auto& rd1){
      return rd0.frame->z_index < rd1.frame->z_index;
    });
  }

  // add moving frames that have become due.
  dirty_flag = false;
  while(_next_moving_frame < _cutscene_frames->moving_frames.size() &&
        time_s > _cutscene_frames->moving_frames[_next_moving_frame].t0_s){
    const auto& frame = _cutscene_frames->moving_frames[_next_moving_frame];
    auto t = (time_s - frame.t0_s) / (frame.t1_s - frame.t0_s);
    if(t < 1.f){
      auto p_px = vector2f{
        lerp(frame.p0_px.x, frame.p1_px.x, t),
        lerp(frame.p0_px.y, frame.p1_px.y, t)
      };
      _rd_moving_frames.push_back(moving_run_data{&frame, p_px});
      dirty_flag = true;
    }
    ++_next_moving_frame;
  }
  if(dirty_flag){
    std::sort(_rd_moving_frames.begin(), _rd_moving_frames.end(), [](const auto& rd0, const auto& rd1){
      return rd0.frame->z_index < rd1.frame->z_index;
    });
  }

  // integrate moving frames.
  for(auto& rd : _rd_moving_frames){
    auto t = std::min((time_s - rd.frame->t0_s) / (rd.frame->t1_s - rd.frame->t0_s), 1.f);
    rd.p_px.x = lerp(rd.frame->p0_px.x, rd.frame->p1_px.x, t);
    rd.p_px.y = lerp(rd.frame->p0_px.y, rd.frame->p1_px.y, t);
  }

  // check if cutscene has finished.
  if(_frames_done == _frame_count){
    _state = state::done;
  }

  _clock_s = time_s;
}

void cutscene::draw()
{
  for(const auto& rd : _rd_static_frames){
    const auto& frame = *(rd.frame);
    draw_sprite(
      frame.sid,
      frame.p0_px,
      frame.flags & FLIP_FRAME_X,
      frame.flags & FLIP_FRAME_Y
    );
  }
  for(const auto& rd : _rd_moving_frames){
    const auto& frame = *(rd.frame);
    draw_sprite(
      frame.sid,
      rd.p_px,
      frame.flags & FLIP_FRAME_X,
      frame.flags & FLIP_FRAME_Y
    );
  }
}
