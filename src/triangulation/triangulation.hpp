#pragma once
#include <vector>
#include <set>
#include <algorithm>
#include "opengl.hpp"
#include "glm/glm.hpp"
#include <random>
#include <stdio.h>
#include <iostream>
#include <glm/gtx/norm.hpp>

class Triangulation {
	// Represents a single triangle.
	struct Triangle {
		glm::vec2 a;
		glm::vec2 b;
		glm::vec2 c;
		bool isBad;
		Triangle(glm::vec2 a, glm::vec2 b, glm::vec2 c) :
			a(a), b(b), c(c), isBad(false) {}

		Triangle() {}

		bool contains(glm::vec2 p) {
			return p == a || p == b || p == c;
		}

	};

	// Represents an edge.
	struct Edge {
		glm::vec2 a;
		glm::vec2 b;
		bool isBad;
		bool operator==(const Edge& other) const {
			return (a.x == other.a.x && a.y == other.a.y && b.x == other.b.x && b.y == other.b.y) ||
				(a.x == other.b.x && a.y == other.b.y && b.x == other.a.x && b.y == other.a.y);
		}

		Edge(glm::vec2 a, glm::vec2 b) :
			a(a), b(b), isBad(false) {}

	};

	std::vector<Triangle> m_triangles;

	void run(glm::vec2 point);

	// Return true if a point is inside the circumcircle of a triangle. 
	bool inCircumcircle(glm::vec2 point, Triangle triangle);

	// Returns true if successfully added, when there are no other similar edges in the set. 
	bool addToEdges(Edge e, std::vector<Edge> &edges);

	public:
		Triangulation(std::vector<glm::vec2> points) {
			// Determine the super triangle.
			float minX = points[0].x;
			float minY = points[0].y;
			float maxX = minX;
			float maxY = minY;

			for (std::size_t i = 0; i < points.size(); ++i)
			{
				if (points[i].x < minX) minX = points[i].x;
				if (points[i].y < minY) minY = points[i].y;
				if (points[i].x > maxX) maxX = points[i].x;
				if (points[i].y > maxY) maxY = points[i].y;
			}

			const float dx = maxX - minX;
			const float dy = maxY - minY;
			const float deltaMax = std::max(dx, dy);
			const float midx = (minX + maxX)/2.0f;
			const float midy = (minY + maxY)/2.0f;

			const glm::vec2 p1 = glm::vec2(midx - 20 * deltaMax, midy - deltaMax);
			const glm::vec2 p2 = glm::vec2(midx, midy + 20 * deltaMax);
			const glm::vec2 p3 = glm::vec2(midx + 20 * deltaMax, midy - deltaMax);

			Triangle super_triangle = Triangle(p1, p2, p3);
			m_triangles.push_back(super_triangle);



			for (int i = 0; i < points.size(); i++) {
				run(points[i]);

				// Log the progress.
				if (i % 250 == 0) {
					printf("Progress: %.2f%% \n", ((float)i/(float)points.size())*100.0f);
				}
			}

			m_triangles.erase(std::remove_if(begin(m_triangles), end(m_triangles), [p1, p2, p3](Triangle &t) {
				return t.contains(p1) || t.contains(p2) || t.contains(p3);
			}), end(m_triangles));
		};

		std::vector<glm::vec2> getPoints();

};
