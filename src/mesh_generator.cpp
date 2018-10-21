#include "mesh_generator.hpp";
#include "triangulation/delaunator.hpp";

void MeshGenerator::init() {
	// Initial positions of the mesh.
	double minX = -m_width / 2;
	double minY = -m_height / 2;

	int num_of_points = m_width * m_height * m_subdivisions * m_subdivisions;

	// Generate the coordinates of all vertices making up the mesh.
	std::vector<glm::vec2> coords;
	for (int i = 0; i < num_of_points; i++) {
		float ranX = ((float)rand() / (float)(RAND_MAX));
		float ranY = ((float)rand() / (float)(RAND_MAX));
		coords.push_back(glm::vec2(minX + ranX * (m_width), minY + ranY * (m_height)));
	}

	// Carry out Delaunay triangulation.
	Triangulation triangulation = Triangulation(coords);
	triangulated_points = triangulation.getPoints();

	//// Temporarily using the faster Delaunay library. 
	//std::vector<double> coords;
	//for (int i = 0; i < num_of_points; i++) {
	//	float ranX = ((float)rand() / (float)(RAND_MAX));
	//	float ranY = ((float)rand() / (float)(RAND_MAX));
	//	coords.push_back(minX + ranX * (m_width));
	//	coords.push_back(minY + ranY * (m_height));
	//}

	//delaunator::Delaunator d(coords);
	//
	//for (std::size_t i = 0; i < d.triangles.size(); i ++) {
	//	float x = d.coords[2 * d.triangles[i]];        
	//	float y = d.coords[2 * d.triangles[i] + 1];   
	//	triangulated_points.push_back(glm::vec2(x, y));
	//}
	
	// Generate a radial fall off map for each coordinate, for castle placement.  
	float centerX = 2;
	float centerY = 2;
	float radius = 3;
	for (int i = 0; i < triangulated_points.size(); i++) {
		float distanceX = std::pow(centerX - triangulated_points[i].x, 2);
		float distanceY = std::pow(centerY - triangulated_points[i].y, 2);

		float distanceToCenter = std::sqrt(distanceX + distanceY);

		if (distanceToCenter < radius) {
			fall_off_castle.push_back(0);
		}
		else {
			fall_off_castle.push_back(distanceToCenter);
		}
	}

	// Generate a second fall off map for each coordinate, to trim off edges. 
	centerX = 0;
	centerY = 0;
	for (int i = 0; i < triangulated_points.size(); i++) {
		float distanceX = std::pow(centerX - triangulated_points[i].x, 2);
		float distanceY = std::pow(centerY - triangulated_points[i].y, 2);

		float distanceToCenter = std::sqrt(distanceX + distanceY);

		if ((triangulated_points[i].x > minX && triangulated_points[i].x < minX + 0.01 * m_width) ||
			(triangulated_points[i].y > minY && triangulated_points[i].y < minY + 0.01 * m_height) ||
			(triangulated_points[i].x >(m_width / 2) - (0.01 * m_width)) ||
			(triangulated_points[i].y > (m_height / 2) - (0.01 * m_height))) {
			trim_edge.push_back(distanceToCenter);
		}
		else {
			trim_edge.push_back(0);
		}
	}
}

float MeshGenerator::generateOffset(glm::vec2 coord, double time) {
	float waveLength = 10;
	float waveAmplitude = 0.2;
	float speed = time / 2;
	float radiansX = (coord.x / waveLength + speed) * 2.0 * std::_Pi;
	float radiansY = (coord.y / waveLength + speed) * 2.0 * std::_Pi;
	return waveAmplitude * 0.5 * (std::sin(radiansX) + std::cos(radiansY));
}

cgra::Mesh MeshGenerator::generateWaterMesh(double time) {
	cgra::Mesh mesh;
	// Initial positions of the mesh.
	double minX = -m_width / 2;
	double minY = -m_height / 2;

	// Number of subdivisions for each mesh. 
	int width_subdivisions = m_width * 2;
	int height_subdivisions = m_height * 2;

	// Dimensions of each triangle making up the mesh.
	float subWidth = m_width / width_subdivisions;
	float subHeight = m_height / height_subdivisions;

	// Number of vertices and triangles making up the mesh.
	int num_vertices = (width_subdivisions + 1) * (height_subdivisions + 1);
	int num_triangles = (width_subdivisions * height_subdivisions) * 2;
	cgra::Matrix<double> vertices(num_vertices, 3);
	cgra::Matrix<unsigned int> triangles(num_triangles, 3);

	// Generate the coordinates of all vertices making up the mesh.
	int count = 0;
	for (int i = 0; i <= width_subdivisions; i++) {
		float x = minX + subWidth * i;
		for (int j = 0; j <= height_subdivisions; j++) {
			float y = minY + subHeight * j;
			float offset = generateOffset(glm::vec2(x, y), time);
			vertices.setRow(count++, { x + offset, -0.5 + offset, y + offset });
		}
	}
	// Set the triangle data by specificying the respective vertices to make up each triangle. 
	count = 0;
	for (int x = 0; x < width_subdivisions; x++) {
		for (int y = 0; y < height_subdivisions; y++) {
			unsigned int a = x * (width_subdivisions + 1) + y;
			unsigned int b = (x + 1) * (width_subdivisions + 1) + y;
			unsigned int c = x * (width_subdivisions + 1) + (y + 1);
			unsigned int d = (x + 1) * (width_subdivisions + 1) + (y + 1);
			triangles.setRow(count, { d, b, a });
			triangles.setRow(count + 1, { d, a, c });
			count += 2;
		}
	}
	mesh.setData(vertices, triangles);
	return mesh;
}

std::vector<cgra::Mesh> MeshGenerator::generateMeshes(Noise &n, float height, float redistribution_factor) {
	printf("Generating meshes now!\n");
	std::vector<cgra::Mesh> meshes;

	// Map each vertices generated to a height based on the noise function.
	PerlinNoise river_map = PerlinNoise(0, 0.2, 0.1, 2.0, 6);
	std::vector<glm::vec3> vertices;
	for (std::size_t i = 0; i < triangulated_points.size(); i++) {
		double x1 = triangulated_points[i].x;
		double y1 = triangulated_points[i].y;

		// Low frequency noises are added to create smooth regions
		// for a more natural look. 
		PerlinNoise n1 = PerlinNoise(500, 0.1, 0.1, 0.5, 6);
		PerlinNoise n2 = PerlinNoise(0, 0.01, 0.5, 2, 6);

		double h1 = 1.0 * n.noise(x1, y1) + 1.5 * n1.noise(x1, y1) + 0.15 * n2.noise(x1, y1);
		//double h1 = n.noise(x1, y1);

		// Castle space.
		h1 = (h1 - (-1.0f)) / 2.0f;
		//h1 = (fall_off_castle[i] / (m_width / 4.0f)) - h1;
		h1 = h1 - (fall_off_castle[i] / (m_width/0.7));

		// Trimming edges.
		h1 = h1 - (trim_edge[i] / (m_width / 2.0f));

		// More tweaking. 
		
		h1 = h1 <= 0 ? 0 : h1;
		h1 = std::pow(h1, redistribution_factor);
		h1 *= height;

		// Create water at that region.
		if (h1 <= 0 && x1 > -m_width/2 && x1 < 0 ) {
			h1 -= std::abs(river_map.noise(x1, y1) * 4);
		}

		minHeight = std::min(minHeight, h1);
		maxHeight = std::max(maxHeight, h1);
		vertices.push_back(glm::vec3(x1, h1, y1));
	}

	m_vertices = vertices;

	// Clear the vertices for each region first.
	snow.clear();
	rock.clear();
	ridge.clear();
	dark_grass.clear();
	light_grass.clear();
	sand.clear();
	dark_sand.clear();

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
	cgra::Mesh dark_sand_mesh = createMeshFromVertices(dark_sand);

	// Push the created meshes to be returned.
	meshes.push_back(snow_mesh);
	meshes.push_back(rock_mesh);
	meshes.push_back(ridge_mesh);
	meshes.push_back(dark_grass_mesh);
	meshes.push_back(light_grass_mesh);
	meshes.push_back(sand_mesh);
	meshes.push_back(dark_sand_mesh);
	return meshes;
}

void MeshGenerator::determineRegionForVertices(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
	float sublength = (maxHeight - minHeight) / 6.0f;

	// Determining factors has to be between 1 to 5.
	if (majority(v1.y, v2.y, v3.y, (7))) {
		push_to_region(snow, v1, v2, v3);
	}
	else if(majority(v1.y, v2.y, v3.y, (5))) {
		push_to_region(rock, v1, v2, v3);
	}
	else if (majority(v1.y, v2.y, v3.y, (2))) {
		push_to_region(ridge, v1, v2, v3);
	}
	else if (majority(v1.y, v2.y, v3.y, (1))) {
		push_to_region(dark_grass, v1, v2, v3);
	}
	else if (majority(v1.y, v2.y, v3.y, (0.5))) {
		push_to_region(light_grass, v1, v2, v3);
	}
	else if(majority(v1.y, v2.y, v3.y, (-0.5))) {
		push_to_region(sand, v1, v2, v3);
	}
	else {
		push_to_region(dark_sand, v1, v2, v3);
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

	if (v.size() <= 0) {
		mesh.setNull();
		return mesh;
	}
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

// https://gamedevacademy.org/complete-guide-to-procedural-level-generation-in-unity-part-3/
std::vector<glm::vec3> MeshGenerator::getFoliagePlacementCoordinates(Noise &n, int density) {
	std::vector<glm::vec3> tree_coords;
	for (int i = 0; i < m_vertices.size(); i+= density * 3) {
		glm::vec3 current = m_vertices[i];

		// We are at a mountain or a lake, no trees should be placed. 
		if (current.y > 1 || current.y < 0) {
			continue;
		}

		float neighbour_radius = 0.1;

		// Compare current noise values with neighbouring noise values. 
		int neighborZBegin = (int)std::max(0.0f, current.z - neighbour_radius);
		int neighborZEnd = (int)std::min((float)(m_height/2), current.z + neighbour_radius);
		int neighborXBegin = (int)std::max(0.0f, current.x - neighbour_radius);
		int neighborXEnd = (int)std::min((float)(m_width/2), current.x + neighbour_radius);

		float maxValue = FLT_MIN;
		for (int neighborZ = neighborZBegin; neighborZ <= neighborZEnd; neighborZ++) {
			for (int neighborX = neighborXBegin; neighborX <= neighborXEnd; neighborX++) {
				float neighborValue = n.noise(neighborX, neighborZ);

				// saves the maximum tree noise value in the radius
				if (neighborValue >= maxValue) {
					maxValue = neighborValue;
				}
			}
		}

		float tree_value = n.noise(current.x, current.z);

		// if the current tree noise value is the maximum one, place a tree in this location
		if (tree_value > maxValue) {
			tree_coords.push_back(current);
		}
	}

	return tree_coords;

}
