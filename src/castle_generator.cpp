#include "castle_generator.hpp";


void CastleGenerator::init() {
	// Code here. :) 

}

cgra::Mesh CastleGenerator::loadObj(const char *filename) {
	cgra::Wavefront obj;
	cgra::Mesh mesh;
	// Wrap the loading in a try..catch block
	try {
		obj = cgra::Wavefront::load(filename);
	}
	catch (std::exception e) {
		std::cerr << "Couldn't load file: '" << e.what() << "'" << std::endl;
		return mesh;
	}

	/************************************************************
	* 2. Create a Mesh                                         *
	*                                                          *
	* Use the data in `obj` to create appropriately-sized      *
	* vertex and index matrices and fill them with data.       *
	*                                                          *
	* Assume that all the faces in the file are triangles.     *
	************************************************************/

	// Replace these with appropriate values
	unsigned int numVertices = obj.m_positions.size();
	unsigned int numTriangles = obj.m_faces.size();

	cgra::Matrix<double> vertices(numVertices, 3);
	cgra::Matrix<unsigned int> triangles(numTriangles, 3);

	for (size_t i = 0; i < obj.m_positions.size(); i++) {
		// Add each position to the vertices matrix
		vertices(i, 0) = obj.m_positions[i].x;
		vertices(i, 1) = obj.m_positions[i].y;
		vertices(i, 2) = obj.m_positions[i].z;
	}

	for (size_t i = 0; i < obj.m_faces.size(); i++) {
		// Add each triangle's indices to the triangles matrix
		// Remember that Wavefront files use indices that start at 1
		for (int j = 0; j < obj.m_faces[i].m_vertices.size(); j++) {
			triangles(i, j) = obj.m_faces[i].m_vertices[j].m_p - 1;
		}
	}

	mesh.setData(vertices, triangles);
	return mesh;
}