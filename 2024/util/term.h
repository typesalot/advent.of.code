#pragma once
#include <cassert>
#include <format>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace term {

template <bool is_background>
class color {
  private:
    uint32_t value = 0;

  public:
    color()  = default;
    ~color() = default;

    color& black() {
      value = 30;
      return *this;
    }

    color& red() {
      value = 31;
      return *this;
    }

    color& green() {
      value = 32;
      return *this;
    }

    color& yellow() {
      value = 33;
      return *this;
    }

    color& blue() {
      value = 34;
      return *this;
    }

    color& magenta() {
      value = 35;
      return *this;
    }

    color& cyan() {
      value = 36;
      return *this;
    }

    color& white() {
      value = 37;
      return *this;
    }

    color& def() {
      value = 39;
      return *this;
    }

    uint32_t get() const {
      uint32_t v = value;
      if constexpr (is_background)
        v += (v != 0) ? 10 : 0;
      return v;
    }

    operator uint32_t() const {
      return value;
    }
};

using bg_color = color<true>;
using fg_color = color<false>;

class attribute {
  public:
    fg_color fg;
    bg_color bg;
    bool     bold;
    bool     underline;
    bool     blink;

    void reset() {
      fg.def();
      bg.def();
      bold      = false;
      underline = false;
      blink     = false;
    }

    static std::string clear() {
      return "\033[0m";
    }

    uint32_t count() const {
      uint32_t cnt = 0;

      if (fg.get() != 0)
        cnt++;
      if (bg.get() != 0)
        cnt++;
      if (bold)
        cnt++;
      if (underline)
        cnt++;
      if (blink)
        cnt++;

      return cnt;
    }

    std::string str() const {
      uint32_t cnt = count();
      if (cnt == 0)
        return clear();

      std::stringstream s;
      s << "\033[";

      if (fg.get()) {
        s << fg.get();
        cnt--;
      }
      if (cnt > 1)
        s << ";";

      if (bg.get()) {
        s << bg.get();
        cnt--;
      }

      assert(cnt == 0);

      s << "m";
      return s.str();
    }
};

class string {
  private:
    std::vector<attribute> _attr;
    std::string            _str;
    bool                   reset_formatting = false;

    void align_size() {
      if (_str.length() != _attr.size()) {
        _attr.clear();
        _attr.resize(_str.length());
      }
    }

  public:
    template <typename... ArgsT>
    string(ArgsT... args) : _str(std::forward<ArgsT>(args)...) {
      reset_formatting = true;
      _attr.clear();
      _attr.resize(_str.size());
    };

    friend std::ostream& operator<<(std::ostream& o, const string& rhs) {
      bool had_formatting = rhs.reset_formatting;
      for (size_t i = 0; i < rhs._str.length(); i++) {
        const attribute& attrib = rhs._attr[i];
        if (attrib.count()) {
          o << rhs._attr[i].str();
          had_formatting = true;
        } else if (had_formatting) {
          o << attrib.clear();
          had_formatting = false;
        }

        o << rhs._str[i];
      }

      return o;
    }

    size_t length() const {
      return _str.length();
    }

    void reset() {
      for (auto& a : _attr)
        a.reset();
    }

    auto foreground(uint32_t position) -> decltype(_attr[position].fg)& {
      align_size();
      return _attr[position].fg;
    }

    auto background(uint32_t position) -> decltype(_attr[position].bg)& {
      align_size();
      return _attr[position].bg;
    }

    char& operator[](uint32_t position) {
      align_size();
      return _str[position];
    }

    string& fg_red() {
      align_size();
      for (auto& atr : _attr)
        atr.fg.red();
      return *this;
    }

    string& fg_default() {
      align_size();
      for (auto& atr : _attr)
        atr.fg.def();
      return *this;
    }
};

class cursor {
  private:
    std::string fmt;

    cursor(const std::string& fmt);

    friend std::ostream& operator<<(std::ostream& o, const cursor& rhs);

  public:
    static cursor move_up(uint32_t lines);

    static cursor add_x(uint32_t offset);
    static cursor sub_x(uint32_t offset);
    static cursor set_x(uint32_t col);

    static cursor save();
    static cursor restore();
    static cursor move(uint32_t x, uint32_t y);
};
}  // namespace term