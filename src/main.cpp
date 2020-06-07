#include <iostream>
#include <ncurses.h>
#include <thread>

#include "army.hpp"
#include "map.hpp"
#include "treasury.hpp"
#include "player.hpp"
#include "gold_mine.hpp"
#include "dwelling.hpp"
#include "visualization.hpp"

int main(int argc, char* argv[])
{
	auto& visualization{ soh::visualization::get_instance() };
	std::atomic<bool> is_game_started{ false };

	soh::treasury treasury;
	soh::army army;
	soh::map map{ 10, 10, 0.5f, 0.5f };

	soh::gold_mine gold_mine_0{ 0, "mine_0", treasury, visualization };
	soh::gold_mine gold_mine_1{ 1, "mine_1", treasury, visualization };
	soh::gold_mine gold_mine_2{ 2, "mine_2", treasury, visualization };

	soh::dwelling dwelling_0{ 0, "dwelling_0", treasury, army, visualization };
	soh::dwelling dwelling_1{ 1, "dwelling_0", treasury, army, visualization };
	soh::dwelling dwelling_2{ 2, "dwelling_0", treasury, army, visualization };

	treasury.ready = true;

	soh::player player_0{ "player_0", treasury, army, map, visualization };
	soh::player player_1{ "player_1", treasury, army, map, visualization };
	soh::player player_2{ "player_2", treasury, army, map, visualization };
	soh::player player_3{ "player_3", treasury, army, map, visualization };

    {
        std::scoped_lock lk{ soh::player::mutex };
        soh::player::current_player = 0;
    }
    soh::player::cv.notify_all();

	std::this_thread::sleep_for(std::chrono::seconds(5));
	treasury.ready = false;
	{
        std::scoped_lock lk{ soh::player::mutex };
        soh::player::gameover = true;
    }
    soh::player::cv.notify_all();

	visualization.halt();
	
	return 0;
}