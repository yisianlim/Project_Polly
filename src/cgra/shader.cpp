#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <stdexcept>

#include "shader.hpp"

namespace cgra {

    // Helper function for compiling a single shader stage
    static GLuint compileShader(const char *src, GLenum type);

    // Helper function for reading a file into a string
    static std::string read_all(std::ifstream &in) {
        std::stringstream sstr;
        sstr << in.rdbuf();
        return sstr.str();
    }

    // Helper function for reading a file into a string
    static std::string read_all(const char *filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::ostringstream msgStream;
            msgStream << "Cannot open file '" << filename << "'";
            throw std::runtime_error(msgStream.str());
        }
        return read_all(file);
    }

    Program Program::load_program(const char *vsFile, const char *fsFile) {
        // Read the contents of the vertex and fragment shader files into memory
        std::string vsSrc = read_all(vsFile);
        std::string fsSrc = read_all(fsFile);

        // Compile the shader stages.
        GLuint vs = compileShader(vsSrc.c_str(), GL_VERTEX_SHADER);
        GLuint fs = compileShader(fsSrc.c_str(), GL_FRAGMENT_SHADER);

        // Create a new program object
        GLuint program = glCreateProgram();

        // Attach the shader stages to the program
        glAttachShader(program, vs);
        glAttachShader(program, fs);

        // Set the attribute locations
        // 0 is the position.
        // 1 is the normal.
        glBindAttribLocation(program, 0, "vertPosition");
        glBindAttribLocation(program, 1, "vertNormal");

        // Link the stages together. This must be done
        // after setting the program
        glLinkProgram(program);

        // Check to see that the linking was successful
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE) {
            // Get the length of the error message
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            //The maxLength includes the NULL character
            std::string infoLog(maxLength, 0);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            // Print an error message
            std::cerr << "Failed to link program:\n" << infoLog << std::endl;

            //We don't need the shader anymore.
            glDeleteProgram(program);
            glDeleteShader(vs);
            glDeleteShader(fs);

            // Abort the process.
            abort();
        }

        // Link was successful, we can detach the shader stages from the
        // program now
        glDetachShader(program, vs);
        glDetachShader(program, fs);

        // We can also delete the shader objects, since we won't be using
        // them again
        glDeleteShader(vs);
        glDeleteShader(fs);

        auto prog = Program(program);

        // Set default values for the matrices
        prog.setModelMatrix(glm::mat4(1));
        prog.setViewMatrix(glm::mat4(1));
        prog.setProjectionMatrix(glm::mat4(1));

        return prog;
    }

    void Program::use() {
        if (m_program != 0) {
            glUseProgram(m_program);
        }
    }

    void Program::setModelMatrix(const glm::mat4 &mat) {
        if (m_program == 0) return;
        use();

        // Get the location of the "modelMat" uniform variable
        GLint modelLoc = glGetUniformLocation(m_program, "modelMat");
        // Set the value of "modelMat"
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mat[0][0]);
    }

    void Program::setViewMatrix(const glm::mat4 &mat) {
        if (m_program == 0) return;
        use();

        GLint viewLoc = glGetUniformLocation(m_program, "viewMat");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &mat[0][0]);
    }

    void Program::setProjectionMatrix(const glm::mat4 &mat) {
        if (m_program == 0) return;
        use();

        GLint projectionLoc = glGetUniformLocation(m_program, "projectionMat");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &mat[0][0]);
    }


	void Program::setColor(const glm::vec3 &col) {
		glUniform3fv(glGetUniformLocation(m_program, "uColor"), 1, glm::value_ptr(col));
	}

    static GLuint compileShader(const char *src, GLenum type) {
        // Create the shader object
        GLuint shader = glCreateShader(type);
        // Load the shader source into the object
        glShaderSource(shader, 1, &src, nullptr);
        // Compile the shader
        glCompileShader(shader);

        // Check to see if the compilation was successful
        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

        if (isCompiled == GL_FALSE) {
            // Get the length of the error message
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            //The maxLength includes the NULL character
            std::string infoLog(maxLength, 0);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

            // Print the error mesage
            std::cerr << "Failed to compile shader:\n" << infoLog << std::endl;

            //We don't need the shader anymore.
            glDeleteShader(shader);

            // Abort the process
            abort();
        }

        return shader;
    }
}
