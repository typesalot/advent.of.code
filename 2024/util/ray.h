#pragma once
#include "point.h"

enum ORIENTATION {
  ORIENTATION_NORTH   = 0,
  ORIENTATION_EAST    = 1,
  ORIENTATION_SOUTH   = 2,
  ORIENTATION_WEST    = 3,
  ORIENTATION_COUNT   = ORIENTATION_WEST + 1,
  ORIENTATION_UNKNOWN = -1
};

template <typename container_t, typename point_t, bool inclusive = false>
auto castRay(const container_t& map, typename container_t::value_type& wall, const point_t& start, const point_t& dir)
    -> point_t {
  point result = start + dir;
  while (map[result.y][result.x] != wall)
    result += dir;
  if constexpr (inclusive)
    return result;
  else
    return result - dir;
}

template <typename point_t>
uint32_t getRayOrientation(const point_t& ray) {
  if (ray.x == 1)
    return ORIENTATION_EAST;
  else if (ray.x == -1)
    return ORIENTATION_WEST;
  else if (ray.y == 1)
    return ORIENTATION_SOUTH;
  return ORIENTATION_NORTH;
}