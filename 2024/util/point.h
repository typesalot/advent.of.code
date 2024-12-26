#pragma once
#include <cstdint>
#include <initializer_list>
#include <ostream>
#include <string>

template <typename T>
struct point {
    point() : y(T(0)), x(T(0)) {};

    point(T _x, T _y) : y(_y), x(_x) {
    }

    point(const std::initializer_list<T>& init) {
      auto itr = init.begin();

      x = *itr;
      itr++;
      y = *itr;
    }

    point(const point<T>& copy) : y(copy.y), x(copy.x) {};

    point(const point<T>&& rval) : y(rval.y), x(rval.x) {};

    T y;
    T x;

    bool operator==(const point<T>& rhs) const {
      return x == rhs.x && y == rhs.y;
    }

    point<T>& operator=(const point<T>& rhs) {
      x = rhs.x;
      y = rhs.y;
      return *this;
    }

    bool operator<(const point<T>& other) const {
      return (y == other.y) ? x < other.x : y < other.y;
    }

    friend std::ostream& operator<<(std::ostream& o, const point<T>& rhs) {
      o << rhs;
      return o;
    }

    point<T> operator-(const point<T>& rhs) const {
      point<T> result;
      result.y = y - rhs.y;
      result.x = x - rhs.x;
      return result;
    }

    point<T> operator+(const point<T>& rhs) const {
      point<T> result;
      result.y = y + rhs.y;
      result.x = x + rhs.x;
      return result;
    }

    point<T>& operator+=(const point<T>& rhs) {
      this->x += rhs.x;
      this->y += rhs.y;
      return *this;
    }

    point<T>& operator-=(const point<T>& rhs) {
      this->x -= rhs.x;
      this->y -= rhs.y;
      return *this;
    }

    operator std::string() const {
      return format("{},{}", y, x);
    }

    uint32_t flatten(uint32_t width) const {
      return y * width + x;
    }

    template <typename value_t>
    static point<T> expand(value_t value, uint32_t width) {
      point<T> p;
      p.y = value / width;
      p.x = value % width;
      return p;
    }
};

using point_int  = point<int32_t>;
using point_uint = point<uint32_t>;

template <typename T>
struct rect {
    point<T> top_left;
    point<T> bottom_right;

    point<T> clamp(const point<T>& p) const {
      point<T> clamped;
      clamped.x = min(max(top_left.x, p.x), bottom_right.x);
      clamped.y = min(max(top_left.y, p.y), bottom_right.y);
      return clamped;
    }

    bool in_bounds(const point<T>& p) const {
      bool b = top_left.x <= p.x && p.x <= bottom_right.x;
      b &= top_left.y <= p.y && p.y <= bottom_right.y;
      return b;
    }
};

using rect_uint = rect<uint32_t>;
using rect_int  = rect<int32_t>;