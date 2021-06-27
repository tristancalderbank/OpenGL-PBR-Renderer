#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
public: 
	// program ID
	unsigned int id;

	Shader(const char* vertexPath, const char* fragmentPath) {
		// retrieve vertex/fragment source 
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vertexShaderFile;
		std::ifstream fragmentShaderFile;

		// make sure ifstream objects can throw exceptions
		vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			// open the files
			vertexShaderFile.open(vertexPath);
			fragmentShaderFile.open(fragmentPath);

			std::stringstream vertexShaderStream, fragmentShaderStream;

			// read file buffers into streams
			vertexShaderStream << vertexShaderFile.rdbuf();
			fragmentShaderStream << fragmentShaderFile.rdbuf();

			// close the files
			vertexShaderFile.close();
			fragmentShaderFile.close();

			// convert streams into string
			vertexCode = vertexShaderStream.str();
			fragmentCode = fragmentShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n" << std::endl;
		}

		const char* vertexShaderCode = vertexCode.c_str();
		const char* fragmentShaderCode = fragmentCode.c_str();

		// compile the shaders
		unsigned int vertex, fragment;
		int success;
		char infoLog[512];

		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexShaderCode, NULL);
		glCompileShader(vertex);

		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// fragment shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// create the shader program
		id = glCreateProgram();
		glAttachShader(id, vertex);
		glAttachShader(id, fragment);
		glLinkProgram(id);

		glGetProgramiv(id, GL_LINK_STATUS, &success);

		if (!success) {
			glGetProgramInfoLog(id, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		// delete the shaders now that they are linked into the program
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	// activate the shader
	void use() {
		glUseProgram(id);
	};

	// uniform functions
	void setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
	};

	void setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(id, name.c_str()), value);
	};

	void setFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(id, name.c_str()), value);
	};

	void setMat4(const std::string& name, glm::mat4 &value) const {
		glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &value[0][0]);
	}

	void setModelViewProjectionMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
		setMat4("model", model);
		setMat4("view", view);
		setMat4("projection", projection);
	}
};