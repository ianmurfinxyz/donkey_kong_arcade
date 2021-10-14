#include "constants.hh"

namespace con {
  const vector2f wide_rebound_dirs[wide_rebound_dirs_count] = {
    {std::sin(4 * -wdT), std::cos(4 * -wdT)},
    {std::sin(3 * -wdT), std::cos(3 * -wdT)},
    {std::sin(2 * -wdT), std::cos(2 * -wdT)},
    {std::sin(1 * -wdT), std::cos(1 * -wdT)},
    {0, 1},
    {std::sin(1 * wdT), std::cos(1 * wdT)},
    {std::sin(2 * wdT), std::cos(2 * wdT)},
    {std::sin(3 * wdT), std::cos(3 * wdT)},
    {std::sin(4 * wdT), std::cos(4 * wdT)},
  };
  const vector2f narrow_rebound_dirs[narrow_rebound_dirs_count] = {
    {std::sin(2 * -ndT), std::cos(2 * -ndT)},
    {std::sin(1 * -ndT), std::cos(1 * -ndT)},
    {0, 1},
    {std::sin(1 * ndT), std::cos(1 * ndT)},
    {std::sin(2 * ndT), std::cos(2 * ndT)},
  };
}