#pragma once
#include <format>
#include <iostream>
#include <string>
#include <vector>

namespace term {

class color {
  private:
    uint32_t value = 0;

  public:
    color()  = default;
    ~color() = default;

    color& black();
    color& red();
    color& green();
    color& yellow();
    color& blue();
    color& magenta();
    color& cyan();
    color& white();
    color& def();

    uint32_t get() const;
    operator uint32_t() const;
};

class attribute {
  public:
    color fg;
    color bg;
    bool  bold;
    bool  underline;
    bool  blink;

    void reset() {
      fg.def();
      bg.def();
      bold      = false;
      underline = false;
      blink     = false;
    }
};

class string {
  private:
    std::vector<attribute> _attr;
    std::string            _str;

    void align_size() {
      if (_str.length() != _attr.size()) {
        _attr.clear();
        _attr.resize(_str.length());
      }
    }

  public:
    template <typename... ArgsT>
    string(ArgsT... args) : _str(std::forward<ArgsT>(args)...) {
      _attr.resize(_str.size());
    };

    friend std::ostream& operator<<(std::ostream& o, const string& rhs) {
      for (size_t i = 0; i < rhs._str.length(); i++) {
        const auto& attrib = rhs._attr[i];
        o << std::format("\033[{};{}m{}", attrib.fg.get(), attrib.bg.get(), rhs._str[i]);
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

    color& foreground(uint32_t position) {
      align_size();
      return _attr[position].fg;
    }

    color& background(uint32_t position) {
      align_size();
      return _attr[position].bg;
    }

    char& operator[](uint32_t position) {
      align_size();
      return _str[position];
    }
};

class cursor {
  private:
    std::string fmt;

    cursor(const std::string& fmt);

    friend std::ostream& operator<<(std::ostream& o, const cursor& rhs);

  public:
    static cursor move_up(uint32_t lines);
};
}  // namespace term