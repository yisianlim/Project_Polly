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
					glm::vec3(4, 3, 10), 
					glm::vec3(0, 0, 0), 
					glm::vec3(0, 1, 0));
    m_program.setViewMatrix(viewMatrix);

	// Generate the terrain
	m_meshGenerator = MeshGenerator(15, 15, 5);
	m_mesh = m_meshGenerator.generate();
	m_terrain_meshes = m_meshGenerator.generateMeshes();
}

void Application::loadObj(const char *filename) {
    cgra::Wavefront obj;
    // Wrap the loading in a try..catch block
    try {
        obj = cgra::Wavefront::load(filename);
    } catch (std::exception e) {
        std::cerr << "Couldn't load file: '" << e.what() << "'" << std::endl;
        return;
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

    m_mesh.setData(vertices, triangles);
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

	//for (int i = 0; i < m_terrain_meshes.size(); i++) {
	//	m_program.setModelMatrix(modelTransform);
	//	glm::vec3 colour = m_meshGenerator.region_to_colour.at(i);
	//	m_program.setColor(colour);
	//	m_terrain_meshes[i].draw();
	//}

	m_program.setModelMatrix(modelTransform);
	m_program.setColor(glm::vec3(1, 0, 1));
	m_mesh.draw();


}

void Application::doGUI() {
    ImGui::SetNextWindowSize(ImVec2(250, 250), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Shapes");

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

    // Interface for manual transforms.
    static glm::vec3 rotation(0, 0, 0);
    if (ImGui::InputFloat3("Rotation", &rotation[0])) {
    	m_rotationMatrix = glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z);
    }

    static glm::vec3 translation(0, 0, 0);
    if (ImGui::InputFloat3("Translation", &translation[0])) {
        m_translation = translation;
    }

    static float scale;
    if (ImGui::InputFloat("Scale", &scale)) {
        m_scale = scale;
    }

    // Interface to load .obj file.
    static std::string filename;
    filename.resize(200);
    ImGui::InputText(".obj File", &filename[0], filename.size());

   ImGui::SameLine();
    if(ImGui::Button("Load")){
    	loadObj(&filename[0]);
    }

    // Checkbox for wireframe rendering.
    static bool checked = false;
    ImGui::Checkbox("Draw wireframe", &checked);

    if(checked){
    	m_mesh.setDrawWireframe(true);
    } else {
    	m_mesh.setDrawWireframe(false);
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
    /**********************************************************
     * 4. Interactive Transforms                              *
     *                                                        *
     * Change `translation`, `scale` and `rotation` based on  *
     * `mousePositionDelta`.                                  *
     **********************************************************/

    // Make a vec2 with the current mouse position
    glm::vec2 currentMousePosition(xpos, ypos);

    // Get the difference from the previous mouse position
    glm::vec2 mousePositionDelta = currentMousePosition - m_mousePosition;

    if (m_mouseButtonDown[GLFW_MOUSE_BUTTON_LEFT]) {
    	// Get the pixel coordinates and convert them to viewport coordinates.
    	// p1 is the previous position.
    	// p2 is the current position.
    	float p1_x = (m_mousePosition.x - ((float)m_viewportSize.x/2.0)) / ((float)m_viewportSize.x/2.0);
    	float p1_y = (m_mousePosition.y - ((float)m_viewportSize.y/2.0)) / ((float)m_viewportSize.y/2.0);
    	float p2_x = (currentMousePosition.x - ((float)m_viewportSize.x/2.0)) / ((float)m_viewportSize.x/2.0);
    	float p2_y = (currentMousePosition.y - ((float)m_viewportSize.y/2.0)) / ((float)m_viewportSize.y/2.0);
    	glm::vec2 prevPoint (p1_x, p1_y);
    	glm::vec2 currentPoint (p2_x, p2_y);

    	// Convert them into 3D vectors.
    	glm::vec3 prevVector = turnTo3DVector(prevPoint);
    	glm::vec3 currentVector= turnTo3DVector(currentPoint);

    	// Calculate the angle between the two vectors.
    	float angle = std::acos(glm::dot(prevVector, currentVector));

    	// Calculate the axis.
    	glm::vec3 axis = glm::cross(prevVector, currentVector);
    	axis.y = -axis.y;

    	// Form the rotation matrix.
    	if(glm::length(axis) > 0){
    		glm::mat4 rot = glm::rotate(glm::mat4(1), angle, axis);
    		m_rotationMatrix = glm::inverse(rot) * m_rotationMatrix;
    	}
    } else if (m_mouseButtonDown[GLFW_MOUSE_BUTTON_MIDDLE]) {
    	glm::vec3 translation((mousePositionDelta.x/(m_viewportSize.x/2))-1, -((mousePositionDelta.y/(m_viewportSize.y/2))-1), 0);
    	glm::vec2 pos = mousePositionDelta / m_viewportSize;
    	pos.y = -pos.y;
    	m_translation += glm::vec3(pos, 0);

    } else if (m_mouseButtonDown[GLFW_MOUSE_BUTTON_RIGHT]) {
    	float scale = currentMousePosition.y/(m_viewportSize.y/2);
    	if(scale > 0){
    		m_scale = scale;
    	}
    }

    // Update the mouse position to the current one
    m_mousePosition = currentMousePosition;

}

glm::vec3 Application::turnTo3DVector(glm::vec2 vector){
	float len = glm::length(vector);
	glm::vec3 vec_3d;
	if(len > 1){
		glm::vec2 vec_normalized = glm::normalize(vector);
		vec_3d = glm::vec3(vec_normalized.x, vec_normalized.y, 0);
	} else {
		float z = glm::sqrt(1-(len*len)); // square root of 1 - x^2 + y^2
		vec_3d = glm::vec3(vector.x, vector.y, z);
	}
	return vec_3d;
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
