#ifndef DONKEY_KONG_1981_ANIMATION_HH
#define DONKEY_KONG_1981_ANIMATION_HH

#include <vector>
#include "../assets/sprites.hh"

constexpr int ANIM_FRAME_FLIP_X = 0b0001;
constexpr int ANIM_FRAME_FLIP_Y = 0b0010;
constexpr int ANIM_FRAME_HAMMER_UP = 0b0100;
constexpr int ANIM_FRAME_HAMMER_DOWN = 0b1000;

struct animation_frame {
  sprite_id sid;
  int flags;
};

struct animation_frames {
  std::vector<animation_frame> frames;
  float frame_delta_s;
};

class animation {
public:
  enum class state { playing, done };
  enum class mode { loop, once, reverse_loop, reverse_once };
  void initialise(const animation_frames& frames, mode m = mode::loop);
  void reset();
  void update(float dt_s);
  [[nodiscard]] animation_frame get_current_frame() const;
  [[nodiscard]] bool is_done() const {return _state == state::done;}
private:
  const animation_frames* _frames;
  state _state;
  mode _mode;
  std::size_t _current_frame;
  float _timer_s;
};

#endif //DONKEY_KONG_1981_ANIMATION_HH
