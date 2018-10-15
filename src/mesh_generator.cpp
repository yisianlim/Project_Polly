#include "mesh_generator.hpp";
#include "triangulation/delaunator.hpp";

std::vector<cgra::Mesh> MeshGenerator::generateMeshes() {
	std::vector<cgra::Mesh> meshes;
	PerlinNoise noise_map_1 = PerlinNoise(0, 0.1, 0.5, 2);

	// Initial positions of the mesh.
	double minX = -m_width / 2;
	double minY = -m_height / 2;

	int num_of_points = m_width * m_height * m_subdivisions * m_subdivisions;

	//// Carry out Delaunay triangulation.
	//delaunator::Delaunator d(coords);

	// Generate the coordinates of all vertices making up the mesh.
	std::vector<glm::vec2> coords;
	for (int i = 0; i < num_of_points; i++) {
			float ranX = ((float)rand() / (float)(RAND_MAX));
			float ranY = ((float)rand() / (float)(RAND_MAX));
			coords.push_back(glm::vec2(minX + ranX * m_width, minY + ranY * m_height));
	}

	// Carry out Delaunay triangulation.
	//delaunator::Delaunator d(coords);
	Triangulation triangulation = Triangulation(coords);
	std::vector<glm::vec2> triangulated_points = triangulation.getPoints();

	// Map each vertices generated to a height based on the noise function.
	std::vector<glm::vec3> vertices;
	for (std::size_t i = 0; i < triangulated_points.size(); i ++) {
		double x1 = triangulated_points[i].x;
		double y1 = triangulated_points[i].y;
		double h1 = std::max(-3.0, noise_map_1.noise(x1, y1));
		//h1 = std::pow(h1, 2);
		minHeight = std::min(minHeight, h1);
		maxHeight = std::max(maxHeight, h1);
		vertices.push_back(glm::vec3(x1, h1, y1));
	}


	// Go through all of the vertices and determine which region it is. 
	for (int i = 0; i < vertices.size(); i+=3) {
			glm::vec3 vert_a = vertices[i];
			glm::vec3 vert_b = vertices[i+1];
			glm::vec3 vert_c = vertices[i+2];
			glm::vec3 norm = glm::triangleNormal(vert_a, vert_b, vert_c);
			determineRegionForVertices(vert_a, vert_b, vert_c);
	}

	// Create the meshes. 
	cgra::Mesh snow_mesh = createMeshFromVertices(snow);
	cgra::Mesh rock_mesh = createMeshFromVertices(rock);
	cgra::Mesh ridge_mesh = createMeshFromVertices(ridge);
	cgra::Mesh dark_grass_mesh = createMeshFromVertices(dark_grass);
	cgra::Mesh light_grass_mesh = createMeshFromVertices(light_grass);
	cgra::Mesh sand_mesh = createMeshFromVertices(sand);

	// Push the created meshes to be returned.
	meshes.push_back(snow_mesh);
	meshes.push_back(rock_mesh);
	meshes.push_back(ridge_mesh);
	meshes.push_back(dark_grass_mesh);
	meshes.push_back(light_grass_mesh);
	meshes.push_back(sand_mesh);
	return meshes;
}

void MeshGenerator::determineRegionForVertices(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
	float sublength = (maxHeight - minHeight) / 6.0f;

	// Determining factors has to be between 1 to 5.
	if (majority(v1.y, v2.y, v3.y, (minHeight + sublength * 3))) {
		push_to_region(snow, v1, v2, v3);
	}
	else if(majority(v1.y, v2.y, v3.y, (minHeight + sublength * 1))) {
		push_to_region(rock, v1, v2, v3);
	}
	else if (majority(v1.y, v2.y, v3.y, (minHeight + sublength * 0.8))) {
		push_to_region(ridge, v1, v2, v3);
	}
	else if (majority(v1.y, v2.y, v3.y, (minHeight + sublength * 0.5))) {
		push_to_region(dark_grass, v1, v2, v3);
	}
	else if (majority(v1.y, v2.y, v3.y, (minHeight + sublength * 0.2))) {
		push_to_region(light_grass, v1, v2, v3);
	}
	else {
		push_to_region(sand, v1, v2, v3);
	}
}

bool MeshGenerator::majority(float y1, float y2, float y3, float factor) {
	if (y1 > factor && y2 > factor) return true;
	if (y1 > factor && y3 > factor) return true;
	if (y2 > factor && y3 > factor) return true;
	return false;

}

void MeshGenerator::push_to_region(std::vector<glm::vec3> &region, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
	region.push_back(v1);
	region.push_back(v2);
	region.push_back(v3);
}

cgra::Mesh MeshGenerator::createMeshFromVertices(std::vector<glm::vec3> v) {
	cgra::Mesh mesh;
	cgra::Matrix<double> vertices(v.size(), 3);
	cgra::Matrix<unsigned int> triangles(v.size() / 3, 3);
	int count = 0;
	for (int i = 0; i < v.size(); i += 3) {
		vertices.setRow(i, { v[i].x, v[i].y, v[i].z });
		vertices.setRow(i + 1, { v[i + 1].x, v[i + 1].y, v[i + 1].z });
		vertices.setRow(i + 2, { v[i + 2].x, v[i + 2].y, v[i + 2].z });
		triangles.setRow(count++, { (unsigned int)i, (unsigned int)i + 1, (unsigned int)i + 2 });
	}
	mesh.setData(vertices, triangles);
	return mesh;
}
