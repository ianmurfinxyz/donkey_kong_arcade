#ifndef DONKEY_KONG_1981_CUTSCENE_HH
#define DONKEY_KONG_1981_CUTSCENE_HH

#include <vector>
#include "math.hh"
#include "../assets/sprites.hh"

constexpr int FLIP_FRAME_X = 0b01;
constexpr int FLIP_FRAME_Y = 0b10;

/** Static frames don't move. */
struct static_frame {
  sprite_id sid;
  float t0_s;
  float t1_s;
  vector2f p0_px;
  int z_index; // lower z drawn first.
  int flags;
};

/** Moving frames lerp between p0 and p1 between t0 and t1. */
struct moving_frame : public static_frame {
  vector2f p1_px;
};

/**
 * When building cutscenes it is expected that the frames are ordered by ascending t0_s,
 * i.e. by start time. This is assumed to be true and is not checked. If it is false
 * then the run logic will fail and the cutscenes wont play correctly.
 */
struct cutscene_frames {
  const std::vector<static_frame>& static_frames;
  const std::vector<moving_frame>& moving_frames;
};

class cutscene {
public:
  enum class state {playing, done};
  void initialise(const cutscene_frames& frames);
  void update(float dt_s);
  void draw();
  [[nodiscard]] bool is_done() const {return _state == state::done;}
private:
  const cutscene_frames* _cutscene_frames;
  struct static_run_data {
    const static_frame* frame;
  };
  struct moving_run_data {
    const moving_frame* frame;
    vector2f p_px;
  };
  std::vector<static_run_data> _rd_static_frames;
  std::vector<moving_run_data> _rd_moving_frames;
  /** Indexes into the cutscene's frames of the next frame waiting to begin; all frames
   *  before these indices having already started (and perhaps ended). Remember frames
   *  are ordered in ascending start time in the cutscene. */
  std::size_t _next_static_frame;
  std::size_t _next_moving_frame;
  float _clock_s;
  std::size_t _frames_done; // how many frames have been played.
  std::size_t _frame_count; // total frames in the cutscene.
  state _state = state::done;
};

#endif
