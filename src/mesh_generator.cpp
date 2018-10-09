#include "mesh_generator.hpp";

cgra::Mesh MeshGenerator::generate(double width, double height) {
	cgra::Mesh mesh;

	// Initial positions of the mesh.
	double minX = - width / 2;
	double minY = - height / 2;

	// Number of subdivisions for each mesh. 
	int width_subdivisions = width * 2;
	int height_subdivisions = height * 2;

	// Dimensions of each triangle making up the mesh.
	float subWidth = width / width_subdivisions;
	float subHeight = height / height_subdivisions;

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
			vertices.setRow(count++, {x, 1.0, y});
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