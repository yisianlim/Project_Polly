#pragma once

#include "cgra/mesh.hpp"
#include "cgra/shader.hpp"

#include "glm/glm.hpp"
#include "mesh_generator.hpp"

class Application {
public:
    // The window object managed by GLFW
    GLFWwindow *m_window;

    // The shader program used for drawing
    cgra::Program m_program;

    // The mesh data
    std::vector<cgra::Mesh> m_terrain_meshes;
	cgra::Mesh m_mesh;
	cgra::Mesh foliage;
	cgra::Mesh island;
	cgra::Mesh water;

    // The current size of the viewport
    glm::vec2 m_viewportSize;
    // The current mouse position
    glm::vec2 m_mousePosition;

    // The translation of the mesh as a vec3
    glm::vec3 m_translation;
    // The scale of the mesh
    float m_scale;

    // A 3x3 matrix representing the rotation of the
    // mesh
    glm::mat4 m_rotationMatrix;

    // Whether or not the left, middle or right buttons are down.
    bool m_mouseButtonDown[3];

	float m_angle = 0;
	float y_look_at = 10.0f;

	MeshGenerator m_meshGenerator;
	std::vector<glm::vec3> m_foliage_coords;

    Application(GLFWwindow *win)
        : m_window(win),
          m_viewportSize(1, 1), m_mousePosition(0, 0),
          m_translation(0), m_scale(1), m_rotationMatrix(1) {
        m_mouseButtonDown[0] = false;
        m_mouseButtonDown[1] = false;
        m_mouseButtonDown[2] = false;
    }

    void setWindowSize(int width, int height) {
        m_viewportSize.x = float(width);
        m_viewportSize.y = float(height);
    }

    void init();

    cgra::Mesh loadObj(const char *filename);

    void drawScene();
	void drawFoliage();
    void doGUI();

    void onKey(int key, int scancode, int action, int mods);

    void onMouseButton(int button, int action, int mods);

    void onCursorPos(double xpos, double ypos);

    glm::vec3 turnTo3DVector(glm::vec2 vector);

    void onScroll(double xoffset, double yoffset);
};
