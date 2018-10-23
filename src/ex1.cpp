#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>

#include "opengl.hpp"
#include "imgui.h"

#include "cgra/matrix.hpp"
#include "cgra/wavefront.hpp"

#include "ex1.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "../libnoise/include/noise/noise.h"

using namespace noise;

void Application::init() {
    // Load the shader program
    // The use of CGRA_SRCDIR "/path/to/shader" is so you don't
    // have to run the program from a specific folder.
    m_program = cgra::Program::load_program(
        CGRA_SRCDIR "/res/shaders/Project_Polly.vs.glsl",
        CGRA_SRCDIR "/res/shaders/Project_Polly.fs.glsl");

    // Create a view matrix.
    glm::mat4 viewMatrix(1);
	viewMatrix = glm::lookAt(
					glm::vec3(4, 10, 23),
					glm::vec3(0, 0, 0),
					glm::vec3(0, 1, 0));
    m_program.setViewMatrix(viewMatrix);

	// Generate the terrain
	m_meshGenerator = MeshGenerator(50, 50, 2);
	m_terrain_meshes = m_meshGenerator.generateMeshes(PerlinNoise(0, 0.1, 0.35, 2.0, 6), 5, 2, 28);
	m_foliage_coords = m_meshGenerator.getFoliagePlacementCoordinates(PerlinNoise(0, 0.2, 0.1, 2.0, 6), 28);
	water = m_meshGenerator.generateWaterMesh(glfwGetTime());

	foliage = loadObj(CGRA_SRCDIR "/res/models/tree_leaves.obj");

	// Test plane.
	cgra::Matrix<double> vertices(4, 3);
	cgra::Matrix<unsigned int> triangles(2, 3);

	int size = 5;

	vertices.setRow(0, { 3.0 + size,  0.2,  13.5 + size });
	vertices.setRow(1, { 1,  0.2,  13.5 + size});
	vertices.setRow(2, { 3.0 + size,  0.2, 11.5});
	vertices.setRow(3, { 1,  0.2, 11.5});

	triangles.setRow(0, { 1, 0, 2 });
	triangles.setRow(1, { 2, 3, 1 });

	m_mesh.setData(vertices, triangles);
}

cgra::Mesh Application::loadObj(const char *filename) {
	cgra::Mesh mesh;
    cgra::Wavefront obj;
    // Wrap the loading in a try..catch block
    try {
        obj = cgra::Wavefront::load(filename);
    } catch (std::exception e) {
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
    unsigned int numVertices  = obj.m_positions.size();
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
    	for(int j = 0; j < obj.m_faces[i].m_vertices.size(); j++){
    		triangles(i, j) = obj.m_faces[i].m_vertices[j].m_p-1;
    	}
    }

    mesh.setData(vertices, triangles);
	return mesh;
}

void Application::drawScene() {
    // Calculate the aspect ratio of the viewport;
    // width / height
    float aspectRatio = m_viewportSize.x / m_viewportSize.y;
    // Calculate the projection matrix with a field-of-view of 45 degrees
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

    // Set the projection matrix
    m_program.setProjectionMatrix(projectionMatrix);

    glm::mat4 modelTransform(1.0f);

    /************************************************************
     * 3. Manual Transforms                                     *
     *                                                          *
     * Use `m_translation`, `m_scale`, and `m_rotationMatrix`   *
     * to create the  `modelTransform` matrix.                  *
     * The following glm functions will be useful:              *
     *    `glm::translate`                                      *
     *    `glm::scale`                                          *
     ************************************************************/

    modelTransform = glm::scale(modelTransform, glm::vec3(m_scale));
    modelTransform = m_rotationMatrix * modelTransform;
    modelTransform = glm::translate(modelTransform, m_translation);

	// Terrain drawing.
	for (int i = 0; i < m_terrain_meshes.size(); i++) {
		if (m_terrain_meshes[i].null) {
			continue;
		}
		m_program.setModelMatrix(modelTransform);
		glm::vec3 colour = m_meshGenerator.region_to_colour.at(i);
		m_program.setColor(colour);
		m_terrain_meshes[i].draw();
	}

	for (glm::vec3 coord : m_foliage_coords) {
		m_program.setModelMatrix(glm::translate(modelTransform, coord));
		drawFoliage();
	}

	m_program.setModelMatrix(modelTransform);
	m_program.setColor(glm::vec3(0.16, 0.5, 0.6));
	m_meshGenerator.generateWaterMesh(glfwGetTime()).draw();

	m_program.setColor(glm::vec3(1, 0, 0));
	//m_mesh.draw();

}

void Application::drawFoliage() {
	m_program.setColor(glm::vec3(0.61, 0.77, 0.18));
	foliage.draw();
}

void Application::doGUI() {
    ImGui::SetNextWindowSize(ImVec2(250, 250), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Map Generator");

    /************************************************************
     *                                                          *
     * 2. Load an .obj file                                     *
     *                                                          *
     * Add an input for a filename.                             *
     *                                                          *
     ************************************************************
     *                                                          *
     * 3. Manual Transforms                                     *
     *                                                          *
     * Create inputs for controlling translation, scale and     *
     * rotation.                                                *
     *                                                          *
     ************************************************************
     *                                                          *
     * 5. Add a checkbox for rendering the object in wireframe  *
     *  mode.                                                   *
     ************************************************************/

	static float frequency = 0.35;
	static float persistence = 0.1;
	static float lacunarity = 2.0;
	static int octave = 6;
	static int seed = 0;
	static float height = 5;
	static float redistribution_factor = 2.0;
	static int foliage_sparseness = 15;
	static int lake_size = 28;

	// Change the frequency.
	if (ImGui::SliderFloat("Distance between peaks \n(Frequency)", &frequency, 0.0f, 2.0f, "%.3f")) {
		m_terrain_meshes = m_meshGenerator.generateMeshes(PerlinNoise(seed, persistence, frequency, lacunarity, octave),
			height, redistribution_factor, lake_size);
		m_foliage_coords = m_meshGenerator.getFoliagePlacementCoordinates(PerlinNoise(0, 0.1, 0.1, 2.0, 6), foliage_sparseness);
	}

	// Change the persistence (controls the roughness of the Perlin noise)
	if (ImGui::SliderFloat("Mountain roughness \n(Persistence) ", &persistence, 0.0f, 1.0f, "%.3f")) {
		m_terrain_meshes = m_meshGenerator.generateMeshes(PerlinNoise(seed, persistence, frequency, lacunarity, octave),
			height, redistribution_factor, lake_size);
		m_foliage_coords = m_meshGenerator.getFoliagePlacementCoordinates(PerlinNoise(0, 0.1, 0.1, 2.0, 6), foliage_sparseness);
	}

	// Change the lacunarity (frequency multiplier between successive octaves)
	if (ImGui::SliderFloat("Rate of frequency changes \n(Lacunarity)", &lacunarity, 0.0f, 4.0f, "%.3f")) {
		m_terrain_meshes = m_meshGenerator.generateMeshes(PerlinNoise(seed, persistence, frequency, lacunarity, octave),
			height, redistribution_factor, lake_size);
		m_foliage_coords = m_meshGenerator.getFoliagePlacementCoordinates(PerlinNoise(0, 0.1, 0.1, 2.0, 6), foliage_sparseness);
	}

	// Change the height.
	if (ImGui::SliderFloat("Height", &height, 0.0f, 50.0f, "%.3f")) {
		m_terrain_meshes = m_meshGenerator.generateMeshes(PerlinNoise(seed, persistence, frequency, lacunarity, octave),
			height, redistribution_factor, lake_size);
		m_foliage_coords = m_meshGenerator.getFoliagePlacementCoordinates(PerlinNoise(0, 0.1, 0.1, 2.0, 6), foliage_sparseness);
	}

	// Change the redistribution factor.
	if (ImGui::SliderFloat("Redistribution factor", &redistribution_factor, 0.0f, 10.0f, "%.3f")) {
		m_terrain_meshes = m_meshGenerator.generateMeshes(PerlinNoise(seed, persistence, frequency, lacunarity, octave),
			height, redistribution_factor, lake_size);
		m_foliage_coords = m_meshGenerator.getFoliagePlacementCoordinates(PerlinNoise(0, 0.1, 0.1, 2.0, 6), foliage_sparseness);
	}


	if (ImGui::SliderInt("Foliage Sparseness Factor \n", &foliage_sparseness, 2, 100, "%f")) {
		m_terrain_meshes = m_meshGenerator.generateMeshes(PerlinNoise(seed, persistence, frequency, lacunarity, octave),
			height, redistribution_factor, lake_size);
		m_foliage_coords = m_meshGenerator.getFoliagePlacementCoordinates(PerlinNoise(0, 0.1, 0.1, 2.0, 6), foliage_sparseness);
	}

	// Adjust the size of the lake.
	if (ImGui::SliderInt("Lake size \n", &lake_size, 0, 30, "%f")) {
		m_terrain_meshes = m_meshGenerator.generateMeshes(PerlinNoise(seed, persistence, frequency, lacunarity, octave),
			height, redistribution_factor, lake_size);
		m_foliage_coords = m_meshGenerator.getFoliagePlacementCoordinates(PerlinNoise(0, 0.1, 0.1, 2.0, 6), foliage_sparseness);
	}

	// Change the seed.
	if (ImGui::InputInt("Regenerate (Seed)", &seed)) {
		m_terrain_meshes = m_meshGenerator.generateMeshes(PerlinNoise(seed, persistence, frequency, lacunarity, octave),
			height, redistribution_factor, lake_size);
		m_foliage_coords = m_meshGenerator.getFoliagePlacementCoordinates(PerlinNoise(seed, 0.1, 0.1, 2.0, 6), foliage_sparseness);
	}

    ImGui::End();
}


// Input Handlers

void Application::onMouseButton(int button, int action, int) {
    if (button >=0 && button < 3) {
        // Set the 'down' state for the appropriate mouse button
        m_mouseButtonDown[button] = action == GLFW_PRESS;
    }
}

void Application::onCursorPos(double xpos, double ypos) {
    // Make a vec2 with the current mouse position
    glm::vec2 currentMousePosition(xpos, ypos);

    // Get the difference from the previous mouse position
    glm::vec2 mousePositionDelta = currentMousePosition - m_mousePosition;

    if (m_mouseButtonDown[GLFW_MOUSE_BUTTON_LEFT]) {
		// Rotate around the y-axis.
		float delta = glm::length(mousePositionDelta) / 100.0f;
		delta = mousePositionDelta.x < 0 ? -delta : delta;
		m_angle += delta;
    } else if (m_mouseButtonDown[GLFW_MOUSE_BUTTON_MIDDLE]) {
		// Adjust the y position of the camera.
    	glm::vec2 pos = mousePositionDelta / glm::vec2(20);
    	pos.y = -pos.y;
		y_look_at += pos.y;

    } else if (m_mouseButtonDown[GLFW_MOUSE_BUTTON_RIGHT]) {
		// Zoom into the scene.
    	float scale = currentMousePosition.y/(m_viewportSize.y/2);
    	if(scale > 0){
    		m_scale = scale;
    	}
    }

	// Set the view matrix.
	float radius = 25.0f;
	float camX = sin(m_angle) * radius;
	float camZ = cos(m_angle) * radius;
	glm::mat4 view;
	view = glm::lookAt(glm::vec3(camX, y_look_at, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	m_program.setViewMatrix(view);

    // Update the mouse position to the current one
    m_mousePosition = currentMousePosition;

}

void Application::onKey(int key, int scancode, int action, int mods) {
    // `(void)foo` suppresses unused variable warnings
    (void)key;
    (void)scancode;
    (void)action;
    (void)mods;
}

void Application::onScroll(double xoffset, double yoffset) {
    // `(void)foo` suppresses unused variable warnings
    (void)xoffset;
    (void)yoffset;
}
