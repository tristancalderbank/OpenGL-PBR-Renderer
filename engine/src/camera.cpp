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
Camera::setWindowDimensions(const WindowSize &windowSize) {
    windowDimensions[0] = windowSize.width;
    windowDimensions[1] = windowSize.height;
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
Camera::processActions(float frameTimeDelta, const std::set<KeymapAction>& actions) {
    float normalizedSpeed = speed * frameTimeDelta;

    if (actions.find(KeymapAction::MOVE_FORWARD) != actions.end()) {
        position += normalizedSpeed * getDirection();
    }

    if (actions.find(KeymapAction::MOVE_BACKWARD) != actions.end()) {
        position -= normalizedSpeed * getDirection();
    }

    if (actions.find(KeymapAction::MOVE_LEFT) != actions.end()) {
        position -= normalizedSpeed * glm::cross(getDirection(), up);
    }

    if (actions.find(KeymapAction::MOVE_RIGHT) != actions.end()) {
        position += normalizedSpeed * glm::cross(getDirection(), up);
    }

    if (actions.find(KeymapAction::MOVE_UP) != actions.end()) {
        position += normalizedSpeed * up;
    }

    if (actions.find(KeymapAction::MOVE_DOWN) != actions.end()) {
        position -= normalizedSpeed * up;
    }
}

void
Camera::processMouse(const MousePosition &mousePosition) {

    if (!initialMousePositionSet) {
        lastMouseX = mousePosition.x;
        lastMouseY = mousePosition.y;
        initialMousePositionSet = true;
    }

    float dX = lastMouseX - mousePosition.x;
    float dY = lastMouseY - mousePosition.y;

    lastMouseX = mousePosition.x;
    lastMouseY = mousePosition.y;

    yaw += dX * sensitivity;
    pitch += dY * sensitivity;

    pitch = std::min(pitch, 89.0f); // can't go to 90 because of lookAt up vector won't work
    pitch = std::max(pitch, -89.0f);
}

void
Camera::resetMouse() {
    initialMousePositionSet = false;
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
