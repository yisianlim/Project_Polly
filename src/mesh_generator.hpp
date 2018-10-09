#pragma once
#include "cgra/mesh.hpp"
#include "cgra/matrix.hpp"
#include <stdio.h>
#include <iostream>
#include "noise/noise.hpp"

class MeshGenerator {
	public:
		double m_width;
		double m_height;
		double m_subdivisions;

		// Empty constructor.
		MeshGenerator() {}

		// Overloaded constructor.
		MeshGenerator(double width, double height, int subdivisions) 
			: m_width(width), m_height(height), m_subdivisions(subdivisions) {}

		// Generates the terrain mesh.
		cgra::Mesh generate();

	private:
		std::vector<float> generateHeightMap();

};
