#include "triangulation.hpp";

void Triangulation::run(glm::vec2 point) {

	std::vector<Triangle> bad_triangles;
	std::vector<Edge> edges;
	std::vector<Edge> to_remove;
	std::vector<glm::vec2> m_points; 

	for (int i = 0; i < m_triangles.size(); i++) {
		Triangle &current = m_triangles[i];
		if (inCircumcircle(point, current)) {
			current.isBad = true;
			bad_triangles.push_back(current);
			edges.push_back(Edge(current.a, current.b));
			edges.push_back(Edge(current.b, current.c));
			edges.push_back(Edge(current.c, current.a));
		}
	}

	m_triangles.erase(std::remove_if(begin(m_triangles), end(m_triangles), [](Triangle &t) {
		return t.isBad;
	}), end(m_triangles));


	for (int i = 0; i < edges.size(); i++)
	{
		Edge &e1 = edges[i];
		for (int j = i + 1; j < edges.size(); j++)
		{
			Edge &e2 = edges[j];

			if (e1 == e2) 
			{
				e1.isBad = true;
				e2.isBad = true;
			}
		}
	}

	edges.erase(std::remove_if(begin(edges), end(edges), [](Edge &e) {
		return e.isBad;
	}), end(edges));


	for (Edge e : edges) {
		Triangle tri = Triangle(e.a, e.b, point);
		m_triangles.push_back(tri);
	}

	for (int i = 0; i < m_triangles.size(); i++) {
		Triangle current = m_triangles[i];
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
	const float ab = triangle.a.x * triangle.a.x + triangle.a.y * triangle.a.y;
	const float cd = triangle.b.x * triangle.b.x + triangle.b.y * triangle.b.y;
	const float ef = triangle.c.x * triangle.c.x + triangle.c.y * triangle.c.y;

	const float circum_x = (ab * (triangle.c.y - triangle.b.y) + cd * (triangle.a.y - triangle.c.y) + ef * (triangle.b.y - triangle.a.y)) / 
		(triangle.a.x * (triangle.c.y - triangle.b.y) + triangle.b.x * (triangle.a.y - triangle.c.y) + triangle.c.x * (triangle.b.y - triangle.a.y));
	const float circum_y = (ab * (triangle.c.x - triangle.b.x) + cd * (triangle.a.x - triangle.c.x) + ef * (triangle.b.x - triangle.a.x)) / 
		(triangle.a.y * (triangle.c.x - triangle.b.x) + triangle.b.y * (triangle.a.x - triangle.c.x) + triangle.c.y * (triangle.b.x - triangle.a.x));

	glm::vec2 circum = glm::vec2(circum_x / 2.0f, circum_y / 2.0f);
	const float dist = glm::distance2(circum, point);
	const float circum_radius = glm::distance2(triangle.a, circum);
	return dist <= circum_radius;
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