#include "term.h"

namespace term {

cursor::cursor(const std::string& _fmt) : fmt(_fmt) {};

cursor cursor::move_up(uint32_t lines) {
  return cursor(std::format("\033[{}A", lines));
}

cursor cursor::add_x(uint32_t offset) {
  return cursor(std::format("\033[{}C", offset));
}

cursor cursor::sub_x(uint32_t offset) {
  return cursor(std::format("\033[{}D", offset));
}

cursor cursor::set_x(uint32_t x) {
  return cursor(std::format("\033[{}G", x));
}

cursor cursor::save() {
  return cursor("\033[s");
}

cursor cursor::restore() {
  return cursor("\033[u");
}

cursor cursor::clear() {
  return cursor("\033[2J\033[H");
}

cursor cursor::move(uint32_t x, uint32_t y) {
  return cursor(std::format("\033[{};{}H", x, y));
}

std::ostream& operator<<(std::ostream& o, const cursor& rhs) {
  o << rhs.fmt;
  return o;
}

}  // namespace term