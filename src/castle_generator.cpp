#include "castle_generator.hpp";
#include "glm/ext.hpp"

void CastleGenerator::init() {
	// Code here. :) 
    generateSpace();
}

void CastleGenerator::generateSpace() {
    //distance from center to corner of keep
    float keepDist = 0.8f;
    //the middle of the 4 points
    glm::vec3 p0;
    p0 = (p1+p2+p3+p4)/4;
    // directional vectors from center to corners
    glm::vec3 d1 = p1-p0;
    glm::vec3 d2 = p2-p0;
    glm::vec3 d3 = p3-p0;
    glm::vec3 d4 = p4-p0;
    //position vectors of bounding areas of the keep
    glm::vec3 keep1 = p0 + keepDist*d1;
    glm::vec3 keep2 = p0 + keepDist*d2;
    glm::vec3 keep3 = p0 + keepDist*d3;
    glm::vec3 keep4 = p0 + keepDist*d4;
    //generate the keep
    generateKeep(keep1, keep2, keep3, keep4);
}

void CastleGenerator::generateKeep(glm::vec3 k1, glm::vec3 k2, glm::vec3 k3, glm::vec3 k4) {
    
    
    std::cout<<glm::to_string(k1)<<std::endl;
    
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
