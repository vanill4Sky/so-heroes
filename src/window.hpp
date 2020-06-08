
#pragma once

#include <algorithm>
#include <ncurses.h>
#include <sstream>
#include <iomanip>
#include <string>
#include <string_view>

#include "utils.hpp"

namespace soh
{

enum class color
{
    black = COLOR_BLACK, 
    red = COLOR_RED,   
    green = COLOR_GREEN,
    yellow = COLOR_YELLOW,  
    blue = COLOR_BLUE,
    magenta = COLOR_MAGENTA, 
    cyan = COLOR_CYAN,    
    white = COLOR_WHITE     
};

class window;

struct scoped_color
{
    scoped_color(const soh::window& window, soh::color fg, soh::color bg);
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

    void print(std::string_view text, size_t row, size_t col, 
        size_t header_height = 2, size_t padding = 1) const;
    void print_centered(std::string_view text, size_t row, 
        size_t header_height = 2, size_t padding = 1) const;
    void print_title(std::string_view text) const;
    template<typename ...Args>
    void print_header(Args&&... args) const;

    utils::vec2<size_t> get_size() const;
    void set_color(unsigned int fg, unsigned int bg) const;
    void unset_color(unsigned int fg, unsigned int bg) const;
    soh::scoped_color set_scoped_color(soh::color fg, soh::color bg = soh::color::black) const;

    friend class curses_wrapper;

private:
    window(const soh::window& parent, utils::vec2<size_t> size, utils::vec2<size_t> pos);
    window(WINDOW* win_ptr);

    WINDOW* win_ptr;
};

}

template<typename ...Args>
void soh::window::print_header(Args&&... args) const
{
    auto row{ utils::format_row(std::forward<Args>(args)...) };
    print(row, 0, 0, 0);
    print(std::string(get_size().x - 1, '-' ), 1, 0, 0);
    update();
}