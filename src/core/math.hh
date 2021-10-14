#ifndef DONKEY_KONG_1981_MATH_HH
#define DONKEY_KONG_1981_MATH_HH

#include <cmath>

struct rect
{
  float x;
  float y;
  float w;
  float h;
};

struct irect
{
  int x;
  int y;
  int w;
  int h;
};

struct vector2f
{
  float x;
  float y;

  void operator+=(vector2f v) noexcept
  { x += v.x; y += v.y; }

  void operator-=(vector2f v) noexcept
  { x -= v.x; y -= v.y; }

  void operator*=(float scalar) noexcept
  { x *= scalar; y *= scalar; }

  void normalise()
  { auto h = std::hypot(x, y); x /= h; y /= h; }

  [[nodiscard]] float length() const
  { return std::hypot(x, y); }

  void zero()
  { x = 0; y = 0; }
};

inline vector2f operator+(vector2f a, vector2f b)
{ return {a.x + b.x, a.y + b.y}; }

inline vector2f operator-(vector2f a, vector2f b)
{ return {a.x - b.x, a.y - b.y}; }

inline vector2f operator*(vector2f a, float scalar)
{ return {a.x * scalar, a.y * scalar}; }

inline vector2f norm(vector2f a)
{ auto h = std::hypot(a.x, a.y); return {a.x / h, a.y / h}; }

struct vector2i
{
  int x;
  int y;

  void operator+=(vector2i v) noexcept
  { x += v.x; y += v.y; }

  void operator-=(vector2i v) noexcept
  { x -= v.x; y -= v.y; }

  void operator*=(int scalar) noexcept
  { x *= scalar; y *= scalar; }

  [[nodiscard]] float length() const
  { return std::hypot(static_cast<float>(x), static_cast<float>(y)); }

  void zero()
  { x = 0; y = 0; }
};

inline vector2i operator+(vector2i a, vector2i b)
{ return {a.x + b.x, a.y + b.y}; }

inline vector2i operator-(vector2i a, vector2i b)
{ return {a.x - b.x, a.y - b.y}; }

inline vector2i operator*(vector2i a, int scalar)
{ return {a.x * scalar, a.y * scalar}; }

/** Represents an axis-aligned bounding box. */
struct aabb
{
  float left;
  float right;
  float top;
  float bottom;
};

/**
 * note: all collisions are inclusive of edges, e.g. if a point is
 * on the edge of an aabb it is considered colliding with the aabb,
 * and if two aabbs intersect only along there edges they collide.
 */

/** Tests for an intersection between 2 aabbs. */
inline bool is_collision(const aabb& a, const aabb& b)
{ return ((a.left <= b.right) && (a.right >= b.left)) && ((a.bottom <= b.top) && (a.top >= b.bottom)); }

/** Test for an intersection between an aabb and a horizontal line. */
inline bool is_collision_hline(const aabb& a, float left, float right, float top)
{ return ((a.left <= right) && (a.right >= left)) && ((a.top >= top) && (a.bottom <= top)); }

inline bool is_collision_hline(const aabb& a, int left, int right, int top)
{ return is_collision_hline(a, static_cast<float>(left), static_cast<float>(right), static_cast<float>(top)); }

/** Test for an intersection between an aabb and a vertical line. */
inline bool is_collision_vline(const aabb& a, float bottom, float top, float left)
{ return ((a.left <= left) && (a.right >= left)) && ((a.top >= bottom) && (a.bottom <= top)); }

inline bool is_collision_vline(const aabb& a, int bottom, int top, int left)
{ return is_collision_vline(a, static_cast<float>(bottom), static_cast<float>(top), static_cast<float>(left)); }

/** Test if an aabb contains a point. */
inline bool is_collision_point(const aabb& a, vector2f point)
{ return ((a.left <= point.x) && (a.right >= point.x)) && ((a.top >= point.y) && (a.bottom <= point.y)); }

inline bool is_collision_point(const aabb& a, vector2i point)
{ return is_collision_point(a, vector2f{static_cast<float>(point.x), static_cast<float>(point.y)}); }

inline rect aabb_to_rect(const aabb& a)
{ return rect {a.left, a.bottom, a.right - a.left, a.top - a.bottom}; }

inline aabb rect_to_aabb(const rect& r)
{ return aabb { r.x,  r.x + r.w, r.y, r.y + r.h }; }

/** Linear interpolation. */
inline float lerp(float v0, float v1, float t)
{ return (1 - t) * v0 + t * v1; }

#endif //DONKEY_KONG_1981_MATH_HH
