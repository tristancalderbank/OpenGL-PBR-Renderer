#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
private:
    float sensitivity = 0.001;
    float speed = 5.0f;

	glm::vec3 up;
	glm::vec3 position;
    float yaw;
    float pitch;

    bool initialMousePositionSet = false;
    double lastMouseX;
    double lastMouseY;

    glm::vec3 getDirection() {
        float directionY = sin(pitch);
        float directionX = cos(pitch) * cos(yaw);
        float directionZ = cos(pitch) * sin(yaw);

        return glm::normalize(glm::vec3(directionX, directionY, -directionZ));
    }
    
public:
	Camera(glm::vec3 up, glm::vec3 position, float yaw, float pitch) : up(up), position(position), yaw(yaw), pitch(pitch) {}

	glm::mat4 getViewportMatrix() {
		return glm::lookAt(position, position + getDirection(), up);
	}

    void processKeyboard(GLFWwindow* window, float frameTimeDelta) {
        float normalizedSpeed = speed * frameTimeDelta;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            position -= normalizedSpeed * getDirection();
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            position += normalizedSpeed * getDirection();
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            position += normalizedSpeed * glm::cross(getDirection(), up);
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            position -= normalizedSpeed * glm::cross(getDirection(), up);
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            position -= normalizedSpeed * up;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            position += normalizedSpeed * up;
        }
    }

    void processMouseMovement(GLFWwindow* window, double xPos, double yPos) {

        if (!initialMousePositionSet) {
            lastMouseX = xPos;
            lastMouseY = yPos;
            initialMousePositionSet = true;
        }

        float dX = lastMouseX - xPos;
        float dY = lastMouseY - yPos;

        lastMouseX = xPos;
        lastMouseY = yPos;

        yaw += dX * sensitivity;
        pitch += dY * sensitivity;

        pitch = std::min(pitch, 89.0f); // can't go to 90 because of lookAt up vector won't work
        pitch = std::max(pitch, -89.0f);
    }
};