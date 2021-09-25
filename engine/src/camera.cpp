#include "camera.h"

#include <algorithm>

glm::vec3
Camera::getDirection() {
    float directionY = sin(pitch);
    float directionX = cos(pitch) * cos(yaw);
    float directionZ = cos(pitch) * sin(yaw);

    return glm::normalize(glm::vec3(directionX, directionY, -directionZ));
}

float
Camera::getAspectRatio() {
    return (float) windowDimensions[0] / windowDimensions[1];
}
    
Camera::Camera(
    glm::vec3 up,
    glm::vec3 position,
    float yaw,
    float pitch,
    int windowWidth,
    int windowHeight
) : up(up), position(position), yaw(yaw), pitch(pitch) {
    windowDimensions[0] = windowWidth;
    windowDimensions[1] = windowHeight;
}

void
Camera::setWindowDimensions(int width, int height) {
    windowDimensions[0] = width;
    windowDimensions[1] = height;
}

glm::vec3
Camera::getPosition() {
    return position;
}

glm::mat4
Camera::getViewMatrix() {
    return glm::lookAt(position, position + getDirection(), up);
}

glm::mat4
Camera::getProjectionMatrix() {
    glm::mat4 projection = glm::perspective(glm::radians(fov), getAspectRatio(), zNear, zFar);
    return projection;
}

void
Camera::processKeyboard(GLFWwindow* window, float frameTimeDelta) {
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

void
Camera::processMouseMovement(GLFWwindow* window, double xPos, double yPos) {

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

void
Camera::drawDebugPanel() {
    if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Text("View");
        ImGui::InputFloat3("up", &up.x);
        ImGui::InputFloat3("position", &position.x);

        float yawDegrees = glm::degrees(yaw);
        float pitchDegrees = glm::degrees(pitch);
        float aspectRatio = getAspectRatio();

        ImGui::InputFloat("yaw (degrees)", &yawDegrees);
        ImGui::InputFloat("pitch (degrees)", &pitchDegrees);

        ImGui::Separator();

        ImGui::Text("Projection");
        ImGui::SliderFloat("fov (degrees)", &fov, 0.0f, 180.0f);
        ImGui::InputInt2("window", windowDimensions);
        ImGui::InputFloat("aspect ratio", &aspectRatio);
        ImGui::DragFloat("z-plane near", &zNear, 0.01f, 0.0f, 1000.0f);
        ImGui::DragFloat("z-plane far", &zFar, 0.01f, 0.0f, 1000.0f);
    }
}
