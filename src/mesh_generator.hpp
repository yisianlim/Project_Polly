#pragma once
#include "cgra/mesh.hpp"
#include "cgra/matrix.hpp"
#include <stdio.h>


class MeshGenerator {
	public:
		// Empty constructor.
		MeshGenerator() {}

		/*
		* Generates a plane. 
		*/
		cgra::Mesh generate(double width, double height);
};
