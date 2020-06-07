
#pragma once

#include <ncurses.h>
#include <string_view>

#include "utils.hpp"

namespace soh
{

class window;

struct scoped_color
{
    scoped_color(const soh::window& window, unsigned int fg, unsigned int bg);
    ~scoped_color();

private:
    const soh::window& window;
    const unsigned int fg, bg;
};

class window
{
public:
    window(const soh::window&) = delete;
    window(soh::window&& other);
    ~window();

    soh::window& operator=(const soh::window&) = delete;
    soh::window& operator=(soh::window&& other);

    void update() const;
    void stop() const;

    void print(std::string_view text, size_t row, size_t col, size_t padding = 1) const;
    void print_centered(std::string_view text, size_t row, size_t padding = 1) const;
    void print_title(std::string_view text) const;

    utils::vec2<size_t> get_size() const;
    void set_color(unsigned int fg, unsigned int bg) const;
    void unset_color(unsigned int fg, unsigned int bg) const;
    soh::scoped_color set_scoped_color(unsigned int fg, unsigned int bg) const;

    friend class curses_wrapper;

private:
    window(const soh::window& parent, utils::vec2<size_t> size, utils::vec2<size_t> pos);
    window(WINDOW* win_ptr);

    WINDOW* win_ptr;
};

}