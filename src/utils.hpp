#pragma once

#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>

namespace utils
{

template <typename T>
struct vec2
{
	vec2() {}
	vec2(T x, T y): x{x}, y{y} {}
	T x, y;

	bool operator==(const vec2<T>& rhs) const
	{
		return this->x == rhs.x && this->y == rhs.y;
	}

	bool operator!=(const vec2<T>& rhs) const
	{
		return !(this->x == rhs.x && this->y == rhs.y);
	}
};

template <typename T>
T pow2(T x)
{
	return x * x;
}

template <typename T>
std::string to_string(T arg)
{
	std::stringstream ss;
	ss << arg;
	return ss.str();
}

template<typename ...Args>
std::string format_row(Args&&... args)
{
    std::stringstream ss;
    ((ss << args.first << std::string(args.second - utils::to_string(args.first).size(), ' ' )), ...);

	return ss.str();
}

template <typename T>
T taxicab_distance(utils::vec2<T> a, utils::vec2<T> b)
{
	return static_cast<T>(std::abs(a.x - b.x)) +  static_cast<T>(std::abs(a.y - b.y)); 
}

}