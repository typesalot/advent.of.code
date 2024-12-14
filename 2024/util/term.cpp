#include "term.h"

namespace term {

color& color::black() {
  value = 30;
  return *this;
}

color& color::red() {
  value = 31;
  return *this;
}

color& color::green() {
  value = 32;
  return *this;
}

color& color::yellow() {
  value = 33;
  return *this;
}

color& color::blue() {
  value = 34;
  return *this;
}

color& color::magenta() {
  value = 35;
  return *this;
}

color& color::cyan() {
  value = 36;
  return *this;
}

color& color::white() {
  value = 37;
  return *this;
}

color& color::def() {
  value = 39;
  return *this;
}

uint32_t color::get() const {
  return value;
}

color::operator uint32_t() const {
  return value;
}

cursor::cursor(const std::string& _fmt) : fmt(_fmt) {};

cursor cursor::move_up(uint32_t lines) {
  return cursor(std::format("\033[{}A", lines));
}

std::ostream& operator<<(std::ostream& o, const cursor& rhs) {
  o << rhs.fmt;
  return o;
}

}  // namespace term