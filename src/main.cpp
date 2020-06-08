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
	soh::map map{ 10, 10, 0.5f, 0.5f };

	{
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

		std::this_thread::sleep_for(std::chrono::seconds(soh::params::simulation_time));

		treasury.ready = false;
		{
			std::scoped_lock lk{ soh::player::mutex };
			soh::player::gameover = true;
		}
		soh::player::cv.notify_all();
	}

	visualization.halt();
}

int main()
{
	play();

	// auto& visualization{ soh::visualization::get_instance() };

	// {
	// 	soh::treasury treasury;
	// 	soh::army army;
	// 	soh::map map{ 10, 10, 0.5f, 0.5f };

	// 	soh::gold_mine gold_mine_0{ 0, "mine_0", treasury, visualization };
	// 	soh::gold_mine gold_mine_1{ 1, "mine_1", treasury, visualization };
	// 	soh::gold_mine gold_mine_2{ 2, "mine_2", treasury, visualization };

	// 	soh::dwelling dwelling_0{ 0, "dwelling_0", treasury, army, visualization };
	// 	soh::dwelling dwelling_1{ 1, "dwelling_0", treasury, army, visualization };
	// 	soh::dwelling dwelling_2{ 2, "dwelling_0", treasury, army, visualization };

	// 	treasury.ready = true;

	// 	soh::player player_0{ "player_0", treasury, army, map, visualization };
	// 	soh::player player_1{ "player_1", treasury, army, map, visualization };
	// 	soh::player player_2{ "player_2", treasury, army, map, visualization };
	// 	soh::player player_3{ "player_3", treasury, army, map, visualization };

	// 	{
	// 		std::scoped_lock lk{ soh::player::mutex };
	// 		soh::player::current_player = 0;
	// 	}
	// 	soh::player::cv.notify_all();

	// 	std::this_thread::sleep_for(std::chrono::seconds(5));
	// 	treasury.ready = false;
	// 	{
	// 		std::scoped_lock lk{ soh::player::mutex };
	// 		soh::player::gameover = true;
	// 	}
	// 	soh::player::cv.notify_all();

	// }

	// visualization.halt();
	
	return 0;
}