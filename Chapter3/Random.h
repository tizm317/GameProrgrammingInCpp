#pragma once

#include <random>
#include "Math.h"
class Random
{
public:
	static Vector2 GetVector(Vector2 s, Vector2 e)
	{
		std::random_device rd;
		std::mt19937 mersenne(rd());
		std::uniform_real_distribution<> distX(s.x, e.x);
		std::uniform_real_distribution<> distY(s.y, e.y);

		return Vector2(float(distX(mersenne)), float(distY(mersenne)));
	}

	static float GetFloatRange(float s, float e)
	{
		std::random_device rd;
		std::mt19937 mersenne(rd());
		std::uniform_real_distribution<> dist(s, e);

		return float(dist(mersenne));
	}
};