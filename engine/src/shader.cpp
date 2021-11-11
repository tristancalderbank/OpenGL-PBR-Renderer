
#include "shader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <glad/glad.h>

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
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
        std::cout << "Error: failed to read shader file: " << vertexPath << std::endl;
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
        std::cout << "Error: vertex shader compilation failed for: " << vertexPath << std::endl;
        std::cout << infoLog << std::endl;
    }

    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "Error: fragment shader compilation failed for: " << fragmentPath << std::endl;
        std::cout << infoLog << std::endl;
    }

    // create the shader program
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        std::cout << "Error: shader program linking failed" << std::endl;
        std::cout << vertexPath << std::endl;
        std::cout << fragmentPath << std::endl;
        std::cout << infoLog << std::endl;
    }

    // delete the shaders now that they are linked into the program
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() {
    glUseProgram(id);
};

void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
};

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
};

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
};

void Shader::setVec2(const std::string& name, glm::vec2& value) const {
    glUniform2f(glGetUniformLocation(id, name.c_str()), value[0], value[1]);
}

void Shader::setVec3(const std::string& name, glm::vec3& value) const {
    glUniform3f(glGetUniformLocation(id, name.c_str()), value[0], value[1], value[2]);
}

void Shader::setVec3Array(const std::string& name, std::vector<glm::vec3> values) const {
    glUniform3fv(glGetUniformLocation(id, name.c_str()), values.size(), &values[0][0]);
}

void Shader::setMat4(const std::string& name, glm::mat4 &value) const {
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setModelViewProjectionMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    setMat4("model", model);
    setMat4("view", view);
    setMat4("projection", projection);
}