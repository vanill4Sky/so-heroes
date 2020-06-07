#include "visualization.hpp"

#include <algorithm>
#include <iostream>
#include <utility>

#include "utils.hpp"

soh::visualization::visualization()
    : main_window{ cw.get_main_window() }
    , player_info_window{ cw.make_window(
        { main_window.get_size().x / 2, main_window.get_size().y / 3 }, 
        { 0, 0 }) }
    , gold_mine_info_window{ cw.make_window(
        { main_window.get_size().x / 2, main_window.get_size().y / 3 }, 
        { 0, main_window.get_size().y / 3  }) }
    , dwelling_info_window{ cw.make_window(
        { main_window.get_size().x / 2, 0 }, 
        { 0, main_window.get_size().y / 3 * 2 }) }
    , game_map_window{ cw.make_window(
        { main_window.get_size().x / 2, 0 }, 
        { main_window.get_size().x / 2, 0 }) }
{
    player_info_window.print_title("Player info");
    gold_mine_info_window.print_title("Gold mine info");
    dwelling_info_window.print_title("Dwelling info");
    game_map_window.print_title("Game map");

    player_info_window.print_header(
        std::make_pair("name", default_column_width),
        std::make_pair("status", default_column_width)
    );
    gold_mine_info_window.print_header(
        std::make_pair("name", default_column_width),
        std::make_pair("status", default_column_width),
        std::make_pair("extracted", default_column_width)
    );
    dwelling_info_window.print_header(
        std::make_pair("name", default_column_width),
        std::make_pair("status", default_column_width),
        std::make_pair("produced", default_column_width)
    );
}

soh::visualization& soh::visualization::get_instance()
{
    static soh::visualization instance;
    return instance;
}

void soh::visualization::add_gold_mine(size_t id, std::string name)
{
    gold_mine_infos.emplace_back(
        std::move(name), soh::gold_mine_state::waiting);
    update_gold_mine_info(id, soh::gold_mine_state::waiting, 0);
}

void soh::visualization::add_dwelling(size_t id, std::string name)
{
    dwelling_infos.emplace_back(
        std::move(name), soh::dwelling_state::waiting);
    update_dwelling_info(id, soh::dwelling_state::waiting, 0);
}

void soh::visualization::add_player(size_t id, std::string name)
{
    player_infos.emplace_back(
        std::move(name), soh::player_state::waiting);
    update_player_info(id, soh::player_state::waiting);
}

void soh::visualization::update_gold_mine_info(
    size_t id, soh::gold_mine_state new_state, size_t extracted_amount)
{
    gold_mine_infos[id].state = new_state;
    auto row{ utils::format_row(
        std::make_pair(gold_mine_infos[id].name, default_column_width),
        std::make_pair(get_description(new_state), default_column_width),
        std::make_pair(extracted_amount, default_column_width)) };

    std::scoped_lock lk{ mutex_terminal };
    gold_mine_info_window.print(row, id, 0);
    gold_mine_info_window.update();
}

void soh::visualization::update_dwelling_info(
    size_t id, soh::dwelling_state new_state, size_t produced_amount)
{
    dwelling_infos[id].state = new_state;
    auto row{ utils::format_row(
        std::make_pair(dwelling_infos[id].name, default_column_width),
        std::make_pair(get_description(new_state), default_column_width),
        std::make_pair(produced_amount, default_column_width)) };

    std::scoped_lock lk{ mutex_terminal };
    dwelling_info_window.print(row, id, 0);
    dwelling_info_window.update();
}

void soh::visualization::update_player_info(size_t id, soh::player_state new_state)
{
    player_infos[id].state = new_state;
    auto row{ utils::format_row(
        std::make_pair(player_infos[id].name, default_column_width),
        std::make_pair(get_description(new_state), default_column_width)) };

    std::scoped_lock lk{ mutex_terminal };
    player_info_window.print(row, id, 0);
    player_info_window.update();
}

void soh::visualization::halt() const
{
    player_info_window.stop();
}

std::string soh::visualization::get_description(soh::gold_mine_state state) const
{
    switch(state)
    {
        case soh::gold_mine_state::digging:
            return "digging";
        case soh::gold_mine_state::finish:
            return "finish";
        case soh::gold_mine_state::transporting:
            return "transporting";
        case soh::gold_mine_state::waiting:
            return "waiting";
        default:
            return "other";
    }
}

std::string soh::visualization::get_description(soh::dwelling_state state) const
{
    switch(state)
    {
        case soh::dwelling_state::producing:
            return "producing";
        case soh::dwelling_state::finish:
            return "finish";
        case soh::dwelling_state::adding:
            return "adding";
        case soh::dwelling_state::waiting:
            return "waiting";
        default:
            return "other";
    }
}

std::string soh::visualization::get_description(soh::player_state state) const
{
    switch(state)
    {
        case soh::player_state::moving:
            return "moving";
        case soh::player_state::collecting:
            return "collecting";
        case soh::player_state::finish:
            return "finish";
        case soh::player_state::fighting:
            return "fighting";
        case soh::player_state::waiting:
            return "waiting";
        default:
            return "other";
    }
}
