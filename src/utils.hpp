#pragma once

#include <cmath>

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

}