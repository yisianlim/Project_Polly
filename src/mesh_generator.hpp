#pragma once
#include "cgra/mesh.hpp"
#include "cgra/matrix.hpp"
#include <stdio.h>
#include <iostream>
#include <glm/gtx/normal.hpp>
#include <map>
#include "noise/noise.hpp"
#include "triangulation/triangulation.hpp"
#include <algorithm>

class MeshGenerator {
	// Various regions of the terrain. 
	enum Region {
		SNOW,
		ROCK,
		RIDGE,
		DARK_GRASS,
		LIGHT_GRASS,
		SAND,
		DARK_SAND,
	};

	public:
		double m_width;
		double m_height;
		double m_subdivisions;
		double minHeight = DBL_MAX;
		double maxHeight = DBL_MIN;
		std::vector<glm::vec2> triangulated_points;
		std::vector<float> fall_off_castle;
		std::vector<float> trim_edge;
		std::vector<glm::vec3> m_vertices;

		std::map<int, glm::vec3> region_to_colour = {
		{ (int)Region::SNOW, glm::vec3(1, 1, 1) },
		{ (int)Region::ROCK, glm::vec3(0.56, 0.56, 0.56) },
		{ (int)Region::RIDGE, glm::vec3(0.29, 0.29, 0.29) },
		{ (int)Region::DARK_GRASS, glm::vec3(0.6, 0.77, 0.47) },
		{ (int)Region::LIGHT_GRASS, glm::vec3(0.62, 0.92, 0.36) },
		{ (int)Region::SAND, glm::vec3(0.43, 0.38, 0.26) },
		{ (int)Region::DARK_SAND, glm::vec3(0.16, 0.5, 0.6) },
		};

		// Empty constructor.
		MeshGenerator() {}

		// Overloaded constructor.
		// points_num determine how many vertices are there in the mesh, which will affect
		// the low poly look of the mesh.
		MeshGenerator(double width, double height, int subdivisions) 
			: m_width(width), m_height(height), m_subdivisions(subdivisions) {
			init();
		}

		// Generates the meshes of different regions making up the terrain. 
		void init();
		std::vector<cgra::Mesh> generateMeshes(Noise &n, float height, float redistribution_factor);

		cgra::Mesh generateWaterMesh(double time);

		// Using a noise map to generate the coordinates for tree placements. 
		std::vector<glm::vec3> getFoliagePlacementCoordinates(Noise &n, int number_of_trees);

		float generateOffset(glm::vec2 coord, double time);

	private:
		// Looks at the vertices and determine which region it should be in. 
		void determineRegionForVertices(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);

		bool majority(float y1, float y2, float y3, float factor);

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
		std::vector<glm::vec3> sand;
		std::vector<glm::vec3> dark_sand;

};
