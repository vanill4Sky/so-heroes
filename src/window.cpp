#include "window.hpp"

#include <cassert>
#include <utility>
#include <ncurses.h>

#include "curses_wrapper.hpp"

soh::scoped_color::scoped_color(const soh::window& window, soh::color fg, soh::color bg)
    : window{window}
    , fg{ static_cast<unsigned int>(fg) }
    , bg{ static_cast<unsigned int>(bg) }
{ 
    window.set_color(this->fg, this->bg);
}

soh::scoped_color::~scoped_color()
{
    window.unset_color(fg, bg);
}

soh::window::window(const soh::window& parent, utils::vec2<size_t> size, utils::vec2<size_t> pos)
    : win_ptr{ subwin(parent.win_ptr, size.y, size.x, pos.y, pos.x) }
{
    box(win_ptr, 0, 0);
    update();
}

soh::window::window(WINDOW* win_ptr)
    : win_ptr{ win_ptr }
{}

soh::window::window(soh::window&& other)
{
    win_ptr = std::exchange(other.win_ptr, nullptr);
}

soh::window::~window()
{
    // :((
    if (win_ptr != nullptr && win_ptr != stdscr)
    {
        wborder(win_ptr, ' ', ' ', ' ',' ',' ',' ',' ',' ');
        update();
        delwin(win_ptr);
        win_ptr = nullptr;
    }
}

soh::window& soh::window::operator=(soh::window&& other)
{
    win_ptr = std::exchange(other.win_ptr, nullptr);
    return *this;
}

void soh::window::update() const
{
    wrefresh(win_ptr);
}

void soh::window::stop() const
{
    wgetch(win_ptr);
}

void soh::window::print(std::string_view text, size_t row, size_t col, 
    size_t header_height, size_t padding) const
{
    mvwprintw(win_ptr, row + padding + header_height, col + padding, "%s" ,text.data());
}

void soh::window::print_centered(std::string_view text, size_t row,
    size_t header_height, size_t padding) const
{
    assert(row < get_size().y);
    assert(get_size().x >= text.size());

    auto col{ static_cast<size_t>((get_size().x -  text.size()) / 2) };
    print(text, row, col, header_height, padding);
}

void soh::window::print_title(std::string_view text) const
{
    print_centered(text, 0, 0, 0);
    update();
}

utils::vec2<size_t> soh::window::get_size() const
{
    return { 
        static_cast<size_t>(win_ptr->_maxx), 
        static_cast<size_t>(win_ptr->_maxy) };
}

void soh::window::set_color(unsigned int fg, unsigned int bg) const
{
    wattron(win_ptr, COLOR_PAIR(soh::curses_wrapper::color_pair_id(fg, bg)));
}

void soh::window::unset_color(unsigned int fg, unsigned int bg) const
{
    wattroff(win_ptr, COLOR_PAIR(soh::curses_wrapper::color_pair_id(fg, bg)));
}

soh::scoped_color soh::window::set_scoped_color(soh::color fg, soh::color bg) const
{
    return {*this, fg, bg};
}

char soh::window::get_char() const
{
    return static_cast<char>(wgetch(win_ptr));
}