#include <iostream>
#include <ncurses.h>
#include <thread>

#include "army.hpp"
#include "map.hpp"
#include "treasury.hpp"
#include "player.hpp"
#include "gold_mine.hpp"

int main(int argc, char* argv[])
{
	soh::treasury treasury;
	soh::army army;
	soh::map map{ 10, 10, 0.5f, 0.5f };

	soh::gold_mine gold_mine_0{ "mine_0", treasury };
	soh::gold_mine gold_mine_1{ "mine_1", treasury };
	soh::gold_mine gold_mine_2{ "mine_2", treasury };
	std::this_thread::sleep_for(std::chrono::seconds(1));

	{
		std::scoped_lock lk{ treasury.mutex };
		std::cout << treasury.gold << '\n';
	}

	// soh::player player_0{ "player_0", treasury, army, map };
	// soh::player player_1{ "player_1", treasury, army, map };
	// soh::player player_2{ "player_2", treasury, army, map };
	// soh::player player_3{ "player_3", treasury, army, map };

    // {
    //     std::scoped_lock lk{ soh::player::mutex };
    //     soh::player::current_player = 0;
    // }
    // soh::player::cv.notify_all();

	// std::this_thread::sleep_for(std::chrono::seconds(1));

	// {
    //     std::scoped_lock lk{ soh::player::mutex };
    //     soh::player::gameover = true;
    // }
    // soh::player::cv.notify_all();


	return 0;
}