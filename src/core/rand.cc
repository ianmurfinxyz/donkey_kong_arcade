#include <limits>
#include "rand.hh"

namespace rnd
{

  //
  // global RBG engine instance.
  //
  xorwow generator;

  //////////////////////////////////////////////////////////////////////////////////////////////////
  //
  // xorwow generator
  //
  //////////////////////////////////////////////////////////////////////////////////////////////////

  xorwow::xorwow(result_type value) noexcept : _state{default_seed}
  {
    seed(value);
  }

  xorwow::xorwow(const state_type& seeds) noexcept : _state{default_seed}
  {
    seed(seeds);
  }

  xorwow::xorwow(std::seed_seq& seq) noexcept : _state{default_seed}
  {
    seed(seq);
  }

  void xorwow::seed()
  {
    _state = default_seed;
  }

  void xorwow::seed(result_type seed)
  {
    _state = default_seed;
    _state[state_size - 2] = seed;
  }

  void xorwow::seed(state_type seeds)
  {
    for(int i{0}; i < state_size; ++i)
      _state[i] = seeds[i] != 0 ? seeds[i] : default_seed[i];
  }

  void xorwow::seed(std::seed_seq& seq)
  {
    seq.generate(_state.begin(), _state.end());
    *_state.end() = 0;
  }

  //
  // sequence generator from: https://en.wikipedia.org/wiki/Xorshift
  //
  xorwow::result_type xorwow::operator()()
  {
    //
    // Algorithm "xorwow" from p. 5 of Marsaglia, "Xorshift RNGs"
    //
    result_type t = _state[4];
    result_type s = _state[0];
    _state[4] = _state[3];
    _state[3] = _state[2];
    _state[2] = _state[1];
    _state[1] = s;
    t ^= t >> 2;
    t ^= t << 1;
    t ^= s ^ (s << 4);
    _state[0] = t;
    _state[5] += 362437;
    return t + _state[5];
  }

  void xorwow::discard(unsigned long long z)
  {
    while(z--)
      (*this)();
  }

  constexpr xorwow::result_type xorwow::min()
  {
    return std::numeric_limits<result_type>::min();
  }

  constexpr xorwow::result_type xorwow::max()
  {
    return std::numeric_limits<result_type>::max();
  }

  bool operator==(const xorwow& lhs, const xorwow& rhs)
  {
    const auto& lhss = lhs.getState();
    const auto& rhss = rhs.getState();
    for(int i{0}; i < xorwow::state_size; ++i){
      if(lhss[i] != rhss[i]){
        return false;
      }
    }
    return true;
  }

  bool operator!=(const xorwow& lhs, const xorwow& rhs)
  {
    return !(lhs == rhs);
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////
  //
  // rand module
  //
  //////////////////////////////////////////////////////////////////////////////////////////////////

  //
  // implementation note: creating a new distribution upon generating each number is actually not
  // much slower than using a single distribution but comes with a massive advantage. Distributions
  // cannot be copied or moved thus making them a pain to maintain within the objects that need
  // them since it restricts the ability to copy and move such objects. It is thus far more
  // flexible to just call a function which requires the callee to maintain no internal state.
  //
  // note: my tests (on a different but similar xorshift algorithm) showed generating 10^6 random
  // ints, creating a new distribution each time, took around 3^4us on my machine, whereas using
  // the same distribution took around 2.4^4, i.e. around 80% of the time. This is an acceptable
  // difference for my needs.
  //

  int uniformSignedInt(int lo, int hi)
  {
    std::uniform_int_distribution<int> d {lo, hi};
    return d(generator);
  }

  unsigned int uniformUnsignedInt(unsigned int lo, unsigned int hi)
  {
    std::uniform_int_distribution<unsigned int> d {lo, hi};
    return d(generator);
  }

  double uniformReal(double lo, double hi)
  {
    std::uniform_real_distribution<double> d {lo, hi};
    return d(generator);
  }

} // namespace dk
