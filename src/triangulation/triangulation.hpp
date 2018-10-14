#pragma once
#include <vector>
#include <set>
#include <algorithm>
#include "opengl.hpp"
#include "glm/glm.hpp"
#include <random>

class Triangulation {
	// Represents a single triangle.
	struct Triangle {
		glm::vec2 a;
		glm::vec2 b;
		glm::vec2 c;
		Triangle(glm::vec2 a, glm::vec2 b, glm::vec2 c) :
			a(a), b(b), c(c) {}
	};

	// Represents an edge.
	struct Edge {
		glm::vec2 a;
		glm::vec2 b;

		bool operator==(const Edge& other) const {
			return (a.x == other.a.x && a.y == other.a.y && b.x == other.b.x && b.y == other.b.y) ||
				(a.x == other.b.x && a.y == other.b.y && b.x == other.a.x && b.y == other.a.y);
		}

		Edge(glm::vec2 a, glm::vec2 b) :
			a(a), b(b) {}

	};

	std::vector<Triangle> m_triangles;

	void run(glm::vec2 point);

	// Return true if a point is inside the circumcircle of a triangle. 
	bool inCircumcircle(glm::vec2 point, Triangle triangle);

	// Returns true if successfully added, when there are no other similar edges in the set. 
	bool addToEdges(Edge e, std::vector<Edge> &edges);

	public:
		Triangulation(double width, double height, std::vector<glm::vec2> points) {
			glm::vec2 a = glm::vec2(0, 0);
			glm::vec2 b = glm::vec2(0, height);
			glm::vec2 c = glm::vec2(width, height);
			glm::vec2 d = glm::vec2(width, 0);

			Triangle t1 = Triangle(a, b, c);
			Triangle t2 = Triangle(a, c, d);

			m_triangles.push_back(t1);
			m_triangles.push_back(t2);

			for (glm::vec2 p : points) {
				run(p);
			}
		}

		std::vector<glm::vec2> getPoints();

};
