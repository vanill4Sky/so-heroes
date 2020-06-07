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

}