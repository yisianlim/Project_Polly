#include "triangulation.hpp";

void Triangulation::run(glm::vec2 point) {

	std::vector<Triangle> bad_triangles;
	std::vector<Edge> edges;
	std::vector<Edge> to_remove;
	std::vector<glm::vec2> m_points; 

	for (int i = m_triangles.size() - 1; i >= 0; i--) {
		Triangle current = m_triangles[i];
		// If this point is in the circumcircle of the current triangle.
		// Put this triangle in the bad_triangles vector. 
		if (inCircumcircle(point, current)) {
			bad_triangles.push_back(current);
			m_triangles.erase(m_triangles.begin() + i);
		}
	}

	for (Triangle tri : bad_triangles) {
		// For each edge in tri. 
		// If edge is not shared by any other triangles in bad_triangles.
		Edge ab = Edge(tri.a, tri.b);
		Edge bc = Edge(tri.b, tri.c);
		Edge ac = Edge(tri.a, tri.c);

		if (!addToEdges(ab, edges)) to_remove.push_back(ab);
		if (!addToEdges(bc, edges)) to_remove.push_back(bc);
		if (!addToEdges(ac, edges)) to_remove.push_back(ac);
	}

	for (Edge e : to_remove) {
		edges.erase(remove(edges.begin(), edges.end(), e), edges.end());
	}

	for (Edge e : edges) {
		Triangle tri = Triangle(e.a, e.b, point);
		m_triangles.push_back(tri);
	}
}

bool Triangulation::addToEdges(Edge e, std::vector<Edge> &edges) {
	for (Edge edge : edges) {
		if (edge == e) {
			return false;
		}
	}
	edges.push_back(e);
	return true;
}

bool Triangulation::inCircumcircle(glm::vec2 point, Triangle triangle) {
	// First, we need the find the center of the circumcircle of the triangle.
	// Given the points.
	glm::vec2 A = triangle.a;
	glm::vec2 B = triangle.b;
	glm::vec2 C = triangle.c;

	// Center of the circumcircle.
	float xc, yc;

	// Find the midpoints of the triangle edges.
	glm::vec2 mid_AB = glm::vec2((A.x + B.x) / 2, (A.y + B.y) / 2);
	glm::vec2 mid_AC = glm::vec2((A.x + C.x) / 2, (A.y + C.y) / 2);

	// Find the slopes of the perpendicular bisector of the edges.
	// Which is the negative reciprocal of the given slope of the edges. 
	float slope_AB = (B.y - A.y) / (B.x - A.x);
	slope_AB = -1.0f / slope_AB;
	float slope_AC = (C.y - A.y) / (C.x - A.x);
	slope_AC = -1.0f / slope_AC;

	float EPSILON = 0.01f;

	// Check for coincident points. 
	if (std::abs(A.y - B.y) < EPSILON && std::abs(B.y - C.y) < EPSILON) {
		return false;
	}
	else if (std::abs(B.y - A.y) < EPSILON) {
		xc = (B.x + A.x) / 2.0f;
		yc = slope_AC * (xc - mid_AC.x) + mid_AC.y;
	}
	else if (std::abs(C.y - A.y) < EPSILON) {
		xc = (C.x + A.x) / 2.0f;
		yc = slope_AB * (xc - mid_AB.x) + mid_AB.y;
	}
	else {
		xc = (slope_AB * mid_AB.x - slope_AC * mid_AC.x + mid_AC.y - mid_AB.y) / (slope_AB - slope_AC);
		yc = slope_AB * (xc - mid_AB.x) + mid_AB.y;
	}

	//printf("points (%f, %f), (%f, %f), (%f, %f)\n", A.x, A.y, B.x, B.y, C.x, C.y);
	//printf("xc=%f, yc=%f\n", xc, yc);

	// Check if the given point is inside the circumcircle. 
	float dx = B.x - xc;
	float dy = B.y - yc;
	float r2 = dx * dx + dy * dy;
	float radius = std::sqrt(r2);
	dx = point.x - xc;
	dy = point.y - yc;
	float d2 = dx * dx + dy * dy;
	return((d2 <= r2) ? true : false);
}

std::vector<glm::vec2> Triangulation::getPoints() {
	std::vector<glm::vec2> points;
	for (Triangle tri : m_triangles) {
		points.push_back(tri.a);
		points.push_back(tri.b);
		points.push_back(tri.c);
	}
	return points;

}