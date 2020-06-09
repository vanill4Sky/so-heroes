#include <iostream>
#include <ncurses.h>
#include <memory>
#include <thread>
#include <vector>

#include "army.hpp"
#include "map.hpp"
#include "treasury.hpp"
#include "player.hpp"
#include "gold_mine.hpp"
#include "dwelling.hpp"
#include "visualization.hpp"
#include "configuration.hpp"

void play()
{
	auto& visualization{ soh::visualization::get_instance() };
	
	soh::treasury treasury;
	soh::army army;
	soh::map map{ soh::params::map_width, soh::params::map_height, 
		soh::params::map_max_treasures, soh::params::map_max_creatures };

	{
		visualization.add_map(map.get_height(), map.get_width(), map.get_map());

		std::vector<std::unique_ptr<soh::gold_mine>> gold_mines;
		for (unsigned int i = 0; i < soh::params::gold_mine_count; ++i)
		{
			auto name{ std::string("mine_") + std::to_string(i) };

			gold_mines.emplace_back(
				std::make_unique<soh::gold_mine>(i, name, treasury, visualization));
		}

		std::vector<std::unique_ptr<soh::dwelling>> dwellings;
		for (unsigned int i = 0; i < soh::params::dwelling_count; ++i)
		{
			auto name{ std::string("dwelling_") + std::to_string(i) };

			dwellings.emplace_back(
				std::make_unique<soh::dwelling>(i, name, treasury, army, visualization));
		}

		std::vector<std::unique_ptr<soh::player>> players;
		for (unsigned int i = 0; i < soh::params::player_count; ++i)
		{
			auto name{ std::string("player_") + std::to_string(i) };

			players.emplace_back(
				std::make_unique<soh::player>(name, treasury, army, map, visualization));
		}

		treasury.ready = true;
		{
			std::scoped_lock lk{ soh::player::mutex };
			soh::player::current_player = 0;
		}
		soh::player::cv.notify_all();

		visualization.pause();
		
		treasury.ready = false;
		{
			std::scoped_lock lk{ soh::player::mutex };
			soh::player::gameover = true;
		}
		soh::player::cv.notify_all();
	}

	visualization.pause();
}

int main()
{
	play();
	
	return 0;
}