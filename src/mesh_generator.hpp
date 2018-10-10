#pragma once
#include "cgra/mesh.hpp"
#include "cgra/matrix.hpp"
#include <stdio.h>
#include <iostream>
#include <map>
#include "noise/noise.hpp"

class MeshGenerator {
	// Various regions of the terrain. 
	enum Region {
		SNOW,
		ROCK,
		RIDGE,
		DARK_GRASS,
		LIGHT_GRASS,
		WATER
	};

	public:
		double m_width;
		double m_height;
		double m_subdivisions;

		std::map<int, glm::vec3> region_to_colour = {
			{ (int)Region::SNOW, glm::vec3(1, 1, 1) },
		{ (int)Region::ROCK, glm::vec3(0.55, 0.49, 0.49) },
		{ (int)Region::RIDGE, glm::vec3(0.74, 0.63, 0.62) },
		{ (int)Region::DARK_GRASS, glm::vec3(0.6, 0.77, 0.47) },
		{ (int)Region::LIGHT_GRASS, glm::vec3(0.62, 0.92, 0.36) },
		{ (int)Region::WATER, glm::vec3(0.22, 0.55, 0.73) },
		};

		// Empty constructor.
		MeshGenerator() {}

		// Overloaded constructor.
		MeshGenerator(double width, double height, int subdivisions) 
			: m_width(width), m_height(height), m_subdivisions(subdivisions) {}

		// Generates the terrain mesh.
		cgra::Mesh generate();

		// Generates the meshes of different regions making up the terrain. 
		std::vector <cgra::Mesh> generateMeshes();

	private:
		// Looks at the vertices and determine which region it should be in. 
		void determineRegionForVertices(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);

		// Create a mesh from the vector of vertices. 
		// 3 consecutive vertices from one triangle. 
		cgra::Mesh createMeshFromVertices(std::vector<glm::vec3> v);

		void push_to_region(std::vector<glm::vec3> &region, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);

		// Vertices for each region.
		std::vector<glm::vec3> snow;
		std::vector<glm::vec3> rock;
		std::vector<glm::vec3> ridge;
		std::vector<glm::vec3> dark_grass;
		std::vector<glm::vec3> light_grass;
		std::vector<glm::vec3> water;

};
