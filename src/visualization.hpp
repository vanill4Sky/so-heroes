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
    finish,
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

    void add_gold_mine(size_t id, std::string name);
    void add_dwelling(size_t id, std::string name);
    void add_player(size_t id, std::string name);

    void update_gold_mine_info(size_t id, soh::gold_mine_state new_state, float progress, size_t extracted_amount);
    void update_dwelling_info(size_t id, soh::dwelling_state new_state, float progress, size_t produced_amount);
    void update_player_info(size_t id, soh::player_state new_state, float progress);
    void update_gold_amount(int amount);
    void update_army_size(int size);

    void halt() const;

private:
    visualization();

    std::string make_progressbar(const size_t length, const float value) const;

    std::string get_description(soh::gold_mine_state state) const;
    std::string get_description(soh::dwelling_state state) const;
    std::string get_description(soh::player_state state) const;
    soh::color get_color(soh::gold_mine_state state) const;
    soh::color get_color(soh::dwelling_state state) const;
    soh::color get_color(soh::player_state state) const;

private:
    const int default_column_width{ 13 };
    const int default_progessbar_width{ default_column_width - 2 };

    std::mutex mutex_terminal;
    soh::curses_wrapper cw;
    soh::window& main_window;
    soh::window player_info_window;
    soh::window gold_mine_info_window;
    soh::window dwelling_info_window;
    soh::window resources_window;
    soh::window game_map_window;
    std::vector<soh::thread_info<gold_mine_state>> gold_mine_infos;
    std::vector<soh::thread_info<dwelling_state>> dwelling_infos;
    std::vector<soh::thread_info<player_state>> player_infos;
};

}