#pragma once
#include <iostream>
#include "../libnoise/include/noise/noise.h"
#include "../libnoise/include/noise/interp.h";

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
public:
	PerlinNoise(int seed, float persistence, float frequency, float lacunarity, int octave) {
		m_perlinModule.SetSeed(seed);
		m_perlinModule.SetPersistence(persistence);
		m_perlinModule.SetFrequency(frequency);
		m_perlinModule.SetLacunarity(lacunarity);
		m_perlinModule.SetOctaveCount(octave);
	}
	virtual double noise(double x, double y) override;
};

/**
* Uses a combination of noises to create smoother transition between terrain types. 
*/
class ComplexNoise : public Noise {
private:
	module::RidgedMulti m_mountain_terrain;
	module::Billow m_base_flat_terrain;
	module::Perlin m_terrain_type;
public:
	ComplexNoise() {
		m_base_flat_terrain.SetFrequency(2.0);

		m_terrain_type.SetFrequency(0.5);
		m_terrain_type.SetPersistence(0.25);

		//m_finalTerrain.SetSourceModule(0, flatTerrain);
		//m_finalTerrain.SetSourceModule(1, mountainTerrain);
		//m_finalTerrain.SetControlModule(terrainType);
		//m_finalTerrain.SetBounds(0.0, 1000.0);
		//m_finalTerrain.SetEdgeFalloff(0.125);
	}
	virtual double noise(double x, double y) override;
};

