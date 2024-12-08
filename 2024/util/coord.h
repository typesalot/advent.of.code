#pragma once

template <typename T>
struct coord {
    coord() : y(T(0)), x(T(0)) {};

    coord(int _x, int _y) : y(_y), x(_x) {
    }

    coord(const initializer_list<T>& init) {
      auto itr = init.begin();

      x = *itr;
      itr++;
      y = *itr;
    }

    coord(const coord<T>& copy) : y(copy.y), x(copy.x) {};

    coord(const coord<T>&& rval) : y(rval.y), x(rval.x) {};

    T y;
    T x;

    bool operator==(const coord<T>& rhs) const {
      return x == rhs.x && y == rhs.y;
    }

    coord<T>& operator=(const coord<T>& rhs) {
      x = rhs.x;
      y = rhs.y;
      return *this;
    }

    friend ostream& operator<<(ostream& o, const coord<T>& rhs) {
      o << rhs;
      return o;
    }

    operator std::string() const {
      return format("{},{}", y, x);
    }
};

using point_int  = coord<int32_t>;
using point_uint = coord<uint32_t>;