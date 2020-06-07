#pragma once

#include <mutex>
#include <vector>

#include "curses_wrapper.hpp"
#include "utils.hpp"

namespace soh
{

enum class gold_mine_state
{
    digging = 0,
    waiting,
    transporting,
    finish,
    other
};

enum class dwelling_state
{
    producing = 0,
    waiting,
    adding,
    finish,
    other
};

enum class player_state
{
    moving = 0,
    waiting,
    fighting,
    collecting,
    other
};

template <typename T>
struct thread_info
{
    thread_info(std::string name, T state)
        : name{ std::move(name) }, state{ state } 
    {}

    std::string name;
    T state;
};

class visualization
{
public:
    static visualization& get_instance();

    visualization(soh::visualization const&) = delete;
    void operator=(visualization const&) = delete;

private:
    visualization();

private:

    std::mutex mutex_terminal;
    soh::curses_wrapper cw;
    soh::window& main_window;
    std::vector<soh::thread_info<gold_mine_state>> gold_mine_infos;
    std::vector<soh::thread_info<dwelling_state>> dwelling_infos;
    std::vector<soh::thread_info<player_state>> player_infos;
};

}