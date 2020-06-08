#include "visualization.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <utility>

#include "utils.hpp"

soh::visualization::visualization()
    : cw{}
    , main_window{ cw.get_main_window() }
    , player_info_window{ cw.make_window(
        { main_window.get_size().x / 2, main_window.get_size().y / 3 }, 
        { 0, 0 }) }
    , gold_mine_info_window{ cw.make_window(
        { main_window.get_size().x / 2, main_window.get_size().y / 3 }, 
        { 0, main_window.get_size().y / 3  }) }
    , dwelling_info_window{ cw.make_window(
        { main_window.get_size().x / 2, 0 }, 
        { 0, main_window.get_size().y / 3 * 2 }) }
    , resources_window{ cw.make_window(
        { main_window.get_size().x / 2, 4 },
        { main_window.get_size().x / 2, 0 }) }
    , game_map_window{ cw.make_window(
        { main_window.get_size().x / 2, 0 }, 
        { main_window.get_size().x / 2, 4 }) }
{
    player_info_window.print_title("Player info");
    gold_mine_info_window.print_title("Gold mine info");
    dwelling_info_window.print_title("Dwelling info");
    resources_window.print_title("Resources");
    game_map_window.print_title("Game map");

    player_info_window.print_header(
        std::make_pair("name", default_column_width),
        std::make_pair("status", default_column_width),
        std::make_pair("progress", default_column_width)
    );
    gold_mine_info_window.print_header(
        std::make_pair("name", default_column_width),
        std::make_pair("status", default_column_width),
        std::make_pair("progress", default_column_width),
        std::make_pair("extracted", default_column_width)
    );
    dwelling_info_window.print_header(
        std::make_pair("name", default_column_width),
        std::make_pair("status", default_column_width),
        std::make_pair("progress", default_column_width),
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
    update_gold_mine_info(id, soh::gold_mine_state::waiting, 0.0f, 0);
}

void soh::visualization::add_dwelling(size_t id, std::string name)
{
    dwelling_infos.emplace_back(
        std::move(name), soh::dwelling_state::waiting);
    update_dwelling_info(id, soh::dwelling_state::waiting, 0.0f, 0);
}

void soh::visualization::add_player(size_t id, std::string name)
{
    player_infos.emplace_back(
        std::move(name), soh::player_state::waiting);
    update_player_info(id, soh::player_state::waiting, 0.0f);
}

void soh::visualization::add_map(size_t height, size_t width, const std::vector<int>& map_data)
{
    map_top = (game_map_window.get_size().y - height) / 2 - 2;
    map_left = ((game_map_window.get_size().x - width) / 2);

    for (size_t row = 0; row < height; ++row)
    {
        for (size_t col = 0; col < width; ++col)
        {
            draw_tile(row, col, map_data[row * width + col]);
        }
    }
    game_map_window.update();
}

void soh::visualization::update_gold_mine_info(
    size_t id, soh::gold_mine_state new_state, float progress, size_t extracted_amount)
{
    gold_mine_infos[id].state = new_state;
    auto row{ utils::format_row(
        std::make_pair(gold_mine_infos[id].name, default_column_width),
        std::make_pair(get_description(new_state), default_column_width),
        std::make_pair(make_progressbar(default_progessbar_width, progress), default_column_width),
        std::make_pair(extracted_amount, default_column_width)) };

    std::scoped_lock lk{ mutex_terminal };

    auto sc{ gold_mine_info_window.set_scoped_color(get_color(new_state)) };
    gold_mine_info_window.print(row, id, 0);
    gold_mine_info_window.update();
}

void soh::visualization::update_dwelling_info(
    size_t id, soh::dwelling_state new_state, float progress, size_t produced_amount)
{
    dwelling_infos[id].state = new_state;
    auto row{ utils::format_row(
        std::make_pair(dwelling_infos[id].name, default_column_width),
        std::make_pair(get_description(new_state), default_column_width),
        std::make_pair(make_progressbar(default_progessbar_width, progress), default_column_width),
        std::make_pair(produced_amount, default_column_width)) };

    std::scoped_lock lk{ mutex_terminal };

    auto sc{ dwelling_info_window.set_scoped_color(get_color(new_state)) };
    dwelling_info_window.print(row, id, 0);
    dwelling_info_window.update();
}

void soh::visualization::update_player_info(
    size_t id, soh::player_state new_state, float progress)
{
    player_infos[id].state = new_state;
    auto row{ utils::format_row(
        std::make_pair(player_infos[id].name, default_column_width),
        std::make_pair(get_description(new_state), default_column_width),
        std::make_pair(make_progressbar(default_progessbar_width, progress), default_column_width)) };

    std::scoped_lock lk{ mutex_terminal };

    auto sc{ player_info_window.set_scoped_color(get_color(new_state)) };
    player_info_window.print(row, id, 0);
    player_info_window.update();
}

void soh::visualization::update_gold_amount(int amount)
{
    auto row{ utils::format_row(
        std::make_pair("Gold", default_column_width), 
        std::make_pair(amount, default_column_width)) };
    
    std::scoped_lock lk{ mutex_terminal };
    
    auto sc{ resources_window.set_scoped_color(soh::color::yellow) };
    resources_window.print(row, 0, 0, 0);
    resources_window.update();
}

void soh::visualization::update_army_size(int size)
{
    auto row{ utils::format_row(
        std::make_pair("Army", default_column_width), 
        std::make_pair(size, default_column_width)) };
    
    std::scoped_lock lk{ mutex_terminal };
    
    auto sc{ resources_window.set_scoped_color(soh::color::red) };
    resources_window.print(row, 1, 0, 0);
    resources_window.update();
}

void soh::visualization::update_tile(int row, int col, int new_value, bool is_player)
{
    draw_tile(row, col, new_value, is_player);
    game_map_window.update();
}

void soh::visualization::halt() const
{
    player_info_window.stop();
}

void soh::visualization::draw_tile(int row, int col, int value, bool is_player)
{
    if (is_player)
    {
        game_map_window.print(std::to_string(value), row + map_top, col + map_left);
    }
    else
    {
        if (value == 0)
        {
            auto sc{ game_map_window.set_scoped_color(soh::color::white) };
            game_map_window.print(".", row + map_top, col + map_left);
        }
        else if(value < 0)
        {
            auto sc{ game_map_window.set_scoped_color(soh::color::red) };
            game_map_window.print("O", row + map_top, col + map_left);
        }
        else
        {
            auto sc{ game_map_window.set_scoped_color(soh::color::yellow) };
            game_map_window.print("G", row + map_top, col + map_left);
        }
    }
}

std::string soh::visualization::make_progressbar(
    const size_t length, const float value) const
{
    assert(length > 2);
    assert(value >= 0.0f);

    auto inner_length{ length - 2 };
    
    size_t progress{ static_cast<size_t>(std::ceil(inner_length * value)) };
    std::string progressbar{
        "[" + std::string(progress, '=') +
        std::string(inner_length - progress, ' ') + "]"};

    return progressbar;  
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

soh::color soh::visualization::get_color(soh::gold_mine_state state) const
{
    switch(state)
    {
        case soh::gold_mine_state::digging:
            return soh::color::magenta;
        case soh::gold_mine_state::finish:
            return soh::color::green;
        case soh::gold_mine_state::transporting:
            return soh::color::red;
        case soh::gold_mine_state::waiting:
            return soh::color::blue;
        default:
            return soh::color::white;
    }
}

soh::color soh::visualization::get_color(soh::dwelling_state state) const
{
    switch(state)
    {
        case soh::dwelling_state::producing:
            return soh::color::magenta;
        case soh::dwelling_state::finish:
            return soh::color::green;
        case soh::dwelling_state::adding:
            return soh::color::red;
        case soh::dwelling_state::waiting:
            return soh::color::blue;
        default:
            return soh::color::white;
    }
}

soh::color soh::visualization::get_color(soh::player_state state) const
{
    switch(state)
    {
        case soh::player_state::moving:
            return soh::color::magenta;
        case soh::player_state::collecting:
            return soh::color::red;
        case soh::player_state::finish:
            return soh::color::green;
        case soh::player_state::fighting:
            return soh::color::red;
        case soh::player_state::waiting:
            return soh::color::blue;
        default:
            return soh::color::white;
    }
}

