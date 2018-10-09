#include "noise.hpp"

double PerlinNoise::noise(double x, double y) {
	return m_perlinModule.GetValue(x, y, 0) * m_heightMultiplier;
}