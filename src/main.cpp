#include <iostream>
#include <ncurses.h>
#include <thread>

#include "army.hpp"
#include "map.hpp"
#include "player.hpp"
#include "treasury.hpp"

int main(int argc, char* argv[])
{
	soh::treasury treasury;
	soh::army army;
	soh::map map{ 10, 10, 0.5f, 0.5f };
	soh::player player_0{ "player_0", treasury, army, map };
	soh::player player_1{ "player_1", treasury, army, map };
	soh::player player_2{ "player_2", treasury, army, map };
	soh::player player_3{ "player_3", treasury, army, map };
    {
        std::lock_guard<std::mutex> lk(soh::player::mutex);
        soh::player::current_player = 0;
    }
    soh::player::cv.notify_all();

	std::this_thread::sleep_for(std::chrono::seconds(10));

	return 0;
}