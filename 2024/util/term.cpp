#include "term.h"

namespace term {

cursor::cursor(const std::string& _fmt) : fmt(_fmt) {};

cursor cursor::move_up(uint32_t lines) {
  return cursor(std::format("\033[{}A", lines));
}

std::ostream& operator<<(std::ostream& o, const cursor& rhs) {
  o << rhs.fmt;
  return o;
}

}  // namespace term