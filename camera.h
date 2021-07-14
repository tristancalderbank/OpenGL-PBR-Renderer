#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "lib/imgui/imgui.h"

class Camera {
private:
    float sensitivity = 0.001;
    float speed = 5.0f;
    bool initialMousePositionSet = false;
    double lastMouseX;
    double lastMouseY;

    // view matrix stuff
	glm::vec3 up;
	glm::vec3 position;
    float yaw;
    float pitch;

    // projection matrix stuff
    float fov = 45.0f;
    float aspect[2] = { 800.0f, 600.0f };
    float zNear = 0.1f;
    float zFar = 100.0f;

    glm::vec3 getDirection() {
        float directionY = sin(pitch);
        float directionX = cos(pitch) * cos(yaw);
        float directionZ = cos(pitch) * sin(yaw);

        return glm::normalize(glm::vec3(directionX, directionY, -directionZ));
    }
    
public:
	Camera(glm::vec3 up, glm::vec3 position, float yaw, float pitch) : up(up), position(position), yaw(yaw), pitch(pitch) {}

    glm::vec3 getPosition() {
        return position;
    }

	glm::mat4 getViewMatrix() {
		return glm::lookAt(position, position + getDirection(), up);
	}

    glm::mat4 getProjectionMatrix() {
        glm::mat4 projection = glm::perspective(glm::radians(fov), aspect[0] / aspect[1], zNear, zFar);
        return projection;
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

    void drawDebugPanel() {
        if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("View");
            ImGui::InputFloat3("up", &up.x);
            ImGui::InputFloat3("position", &position.x);

            float yawDegrees = glm::degrees(yaw);
            float pitchDegrees = glm::degrees(pitch);

            ImGui::InputFloat("yaw (degrees)", &yawDegrees);
            ImGui::InputFloat("pitch (degrees)", &pitchDegrees);

            ImGui::Separator();

            ImGui::Text("Projection");
            ImGui::SliderFloat("fov (degrees)", &fov, 0.0f, 180.0f);
            ImGui::DragFloat2("aspect ratio", aspect, 1.0f, 100.0f, 4000.0f);
            ImGui::DragFloat("z-plane near", &zNear, 0.01f, 0.0f, 1000.0f);
            ImGui::DragFloat("z-plane far", &zFar, 0.01f, 0.0f, 1000.0f);
        }
    }

};