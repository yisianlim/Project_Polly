#include "mesh_generator.hpp";
#include "triangulation/delaunator.hpp";

cgra::Mesh MeshGenerator::generate() {
	//cgra::Mesh mesh;

	// Combine variations of noise maps.
	PerlinNoise noise_map_1 = PerlinNoise(0, 0.1, 0.5, 2);
	PerlinNoise noise_map_2 = PerlinNoise(500, 0.1, 1, 0.5);
	PerlinNoise noise_map_3 = PerlinNoise(17, 0.1, 4, 1);

	// Initial positions of the mesh.
	double minX = - m_width / 2;
	double minY = - m_height / 2;

	// Number of subdivisions for each mesh. 
	int width_subdivisions = m_width * m_subdivisions;
	int height_subdivisions = m_height * m_subdivisions;

	// Dimensions of each triangle making up the mesh.
	float subWidth = m_width / width_subdivisions;
	float subHeight = m_height / height_subdivisions;

	// Number of vertices and triangles making up the mesh.
	int num_vertices = (width_subdivisions + 1) * (height_subdivisions + 1);
	int num_triangles = (width_subdivisions * height_subdivisions) * 2;

	//cgra::Matrix<double> vertices(num_vertices, 3);
	//cgra::Matrix<unsigned int> triangles(num_triangles, 3);

	std::vector<glm::vec2> points;
	std::vector<double> coords;

	// Generate the coordinates of all vertices making up the mesh.
	for (int i = 0; i <= width_subdivisions; i++) {
		float x = minX + subWidth * i;
		for (int j = 0; j <= height_subdivisions; j++) {
			float y = minY + subHeight * j;

			float scale = ((float)rand() / (float)(RAND_MAX));
			float ranX = scale;

			float scale2 = ((float)rand() / (float)(RAND_MAX));
			float ranY = scale2;

			// For triangulation.
			//points.push_back(glm::vec2(ranX, ranY));
			coords.push_back(minX + ranX * m_width);
			coords.push_back(minY + ranY * m_height);
		}
	}

	delaunator::Delaunator d(coords);
	std::vector<glm::vec3> vertices;

	for (std::size_t i = 0; i < d.triangles.size(); i += 3) {
		double x1 = d.coords[2 * d.triangles[i]];
		double y1 = d.coords[2 * d.triangles[i] + 1];
		double h1 = noise_map_1.noise(x1, y1);

		double x2 = d.coords[2 * d.triangles[i + 1]];
		double y2 = d.coords[2 * d.triangles[i + 1] + 1];
		double h2 = noise_map_1.noise(x2, y2);

		double x3 = d.coords[2 * d.triangles[i + 2]];
		double y3 = d.coords[2 * d.triangles[i + 2] + 1];
		double h3 = noise_map_1.noise(x3, y3);

		vertices.push_back(glm::vec3(x1, h1, y1));
		vertices.push_back(glm::vec3(x2, h2, y2));
		vertices.push_back(glm::vec3(x3, h3, y3));
	}

	cgra::Mesh mesh = createMeshFromVertices(vertices);
	
	//cgra::Matrix<double> vertices(triangulated_points.size(), 3);
	//cgra::Matrix<unsigned int> triangles(triangulated_points.size() / 3, 3);
	//int count = 0;
	//for (int i = 0; i < triangulated_points.size(); i += 3) {
	//	vertices.setRow(i, { minX + triangulated_points[i].x, 0, minY + triangulated_points[i].y });
	//	vertices.setRow(i + 1, { minX + triangulated_points[i + 1].x, 0, minY + triangulated_points[i + 1].y });
	//	vertices.setRow(i + 2, { minX + triangulated_points[i + 2].x, 0, minY + triangulated_points[i + 2].y });
	//	triangles.setRow(count++, { (unsigned int)i, (unsigned int)i + 1, (unsigned int)i + 2 });
	//}
	//mesh.setData(vertices, triangles);
	return mesh;

	//// Set the triangle data by specifying the respective vertices to make up each triangle. 
	//count = 0;
	//for (int x = 0; x < width_subdivisions; x++) {
	//	for (int y = 0; y < height_subdivisions; y++) {
	//		unsigned int a = x * (width_subdivisions + 1) + y;
	//		unsigned int b = (x + 1) * (width_subdivisions + 1) + y;
	//		unsigned int c = x * (width_subdivisions + 1) + (y + 1);
	//		unsigned int d = (x + 1) * (width_subdivisions + 1) + (y + 1);
	//		triangles.setRow(count, { d, b, a });
	//		triangles.setRow(count + 1, { d, a, c });
	//		count += 2;
	//	}
	//}

	//mesh.setData(vertices, triangles);
	//return mesh;
}

std::vector<cgra::Mesh> MeshGenerator::generateMeshes() {
	std::vector<cgra::Mesh> meshes;
	PerlinNoise noise_map_1 = PerlinNoise(0, 0.1, 0.5, 2);

	// Initial positions of the mesh.
	double minX = -m_width / 2;
	double minY = -m_height / 2;

	// Number of subdivisions for each mesh. 
	int width_subdivisions = m_width * m_subdivisions;
	int height_subdivisions = m_height * m_subdivisions;

	// Dimensions of each triangle making up the mesh.
	float subWidth = m_width / width_subdivisions;
	float subHeight = m_height / height_subdivisions;

	// Generate the coordinates of all vertices making up the mesh.
	std::vector<double> coords;
	for (int i = 0; i <= width_subdivisions; i++) {
		float x = minX + subWidth * i;
		for (int j = 0; j <= height_subdivisions; j++) {
			float y = minY + subHeight * j;

			float scale = ((float)rand() / (float)(RAND_MAX));
			float ranX = scale;

			float scale2 = ((float)rand() / (float)(RAND_MAX));
			float ranY = scale2;

			// For triangulation.
			//points.push_back(glm::vec2(ranX, ranY));
			coords.push_back(minX + ranX * m_width);
			coords.push_back(minY + ranY * m_height);
		}
	}

	delaunator::Delaunator d(coords);
	std::vector<glm::vec3> vertices;
	for (std::size_t i = 0; i < d.triangles.size(); i += 3) {
		double x1 = d.coords[2 * d.triangles[i]];
		double y1 = d.coords[2 * d.triangles[i] + 1];
		double h1 = noise_map_1.noise(x1, y1);

		double x2 = d.coords[2 * d.triangles[i + 1]];
		double y2 = d.coords[2 * d.triangles[i + 1] + 1];
		double h2 = noise_map_1.noise(x2, y2);

		double x3 = d.coords[2 * d.triangles[i + 2]];
		double y3 = d.coords[2 * d.triangles[i + 2] + 1];
		double h3 = noise_map_1.noise(x3, y3);

		vertices.push_back(glm::vec3(x1, h1, y1));
		vertices.push_back(glm::vec3(x2, h2, y2));
		vertices.push_back(glm::vec3(x3, h3, y3));
	}

	printf("%d\n", vertices.size());

	int count = 0;
	// Go through all of the vertices and determine which region it is. 
	for (int i = 0; i < vertices.size(); i+=3) {
			glm::vec3 vert_a = vertices[i];
			glm::vec3 vert_b = vertices[i+1];
			glm::vec3 vert_c = vertices[i+2];

			determineRegionForVertices(vert_a, vert_b, vert_c);
	}

	// Create the meshes. 
	cgra::Mesh snow_mesh = createMeshFromVertices(snow);
	cgra::Mesh rock_mesh = createMeshFromVertices(rock);
	//cgra::Mesh ridge_mesh = createMeshFromVertices(ridge);
	//cgra::Mesh dark_grass_mesh = createMeshFromVertices(dark_grass);
	//cgra::Mesh light_grass_mesh = createMeshFromVertices(light_grass);
	//cgra::Mesh water_mesh = createMeshFromVertices(water);

	// Push the created meshes to be returned.
	meshes.push_back(snow_mesh);
	meshes.push_back(rock_mesh);
	//meshes.push_back(ridge_mesh);
	//meshes.push_back(dark_grass_mesh);
	//meshes.push_back(light_grass_mesh);
	//meshes.push_back(water_mesh);
	printf("snow size %d\n", snow.size());
	printf("rock size %d\n", rock.size());
	return meshes;
}

void MeshGenerator::determineRegionForVertices(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
	if (v3.y > 0.1) {
		push_to_region(snow, v1, v2, v3);
	}
	else {
		push_to_region(rock, v1, v2, v3);
	}
	//else if (v3.y > 0.8) {
	//	push_to_region(ridge, v1, v2, v3);
	//}
	//else if (v3.y > 0.2) {
	//	push_to_region(dark_grass, v1, v2, v3);
	//}
	//else if (v3.y > 0.1) {
	//	push_to_region(light_grass, v1, v2, v3);
	//}
	//else {
	//	push_to_region(water, v1, v2, v3);
	//}
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
