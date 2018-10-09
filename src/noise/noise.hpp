#pragma once
#include <iostream>
#include "../libnoise/include/noise/noise.h"

using namespace noise;

class Noise {
protected:
	int m_seed;
public:
	virtual double noise(double x, double y) = 0;
};

class PerlinNoise : public Noise {
private:
	module::Perlin m_perlinModule;
	int m_heightMultiplier;
public:
	PerlinNoise(int seed, float persistence, float frequency, int heightMultiplier) {
		m_perlinModule.SetSeed(seed);
		m_perlinModule.SetPersistence(persistence);
		m_perlinModule.SetFrequency(frequency);
		m_heightMultiplier = heightMultiplier;
	}
	virtual double noise(double x, double y) override;
};
