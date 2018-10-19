#pragma once
#include "cgra/mesh.hpp"
#include "cgra/wavefront.hpp"
#include <iostream>
#include "cgra/matrix.hpp"
#include <glm/gtx/normal.hpp>

class CastleGenerator {
	private:
		// Four corners of the castle.
		glm::vec3 p1;
		glm::vec3 p2;
		glm::vec3 p3;
		glm::vec3 p4;

		// Returns a mesh of the loaded obj file.
		// Should only be called once, during initialization. 
		cgra::Mesh loadObj(const char *filename);

		// Initialization of castle generation starts here.
		void init();
        void generateSpace();
        void generateKeep(glm::vec3, glm::vec3, glm::vec3, glm::vec3);

	public:
		// Empty constructor.
		CastleGenerator() {}

		// Overloaded constructor.
		CastleGenerator(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4)
			: p1(p1), p2(p2), p3(p3), p4(p4) {
			init();
		}
};
