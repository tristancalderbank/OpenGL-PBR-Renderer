#pragma once

#include <string>

#include <glm/glm.hpp>

class Shader {
public: 
	Shader(const char* vertexPath, const char* fragmentPath);

	// activate the shader
	void use();

	// uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec3(const std::string& name, glm::vec3& value) const;
	void setVec3Array(const std::string& name, std::vector<glm::vec3> values) const;
	void setMat4(const std::string& name, glm::mat4 &value) const;

	void setModelViewProjectionMatrices(glm::mat4& model, glm::mat4& view, glm::mat4& projection);

private:
    // program ID
    unsigned int id;
};