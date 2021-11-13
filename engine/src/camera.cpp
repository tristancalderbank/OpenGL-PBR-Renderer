#include "camera.h"

#include <algorithm>

glm::vec3
Camera::getDirection() {
    float directionY = sin(mPitch);
    float directionX = cos(mPitch) * cos(mYaw);
    float directionZ = cos(mPitch) * sin(mYaw);

    return glm::normalize(glm::vec3(directionX, directionY, -directionZ));
}

float
Camera::getAspectRatio() {
    return (float) mWindowDimensions[0] / mWindowDimensions[1];
}
    
Camera::Camera(
    glm::vec3 up,
    glm::vec3 position,
    float yaw,
    float pitch,
    int windowWidth,
    int windowHeight
) : mUp(up), mPosition(position), mYaw(yaw), mPitch(pitch) {
    mWindowDimensions[0] = windowWidth;
    mWindowDimensions[1] = windowHeight;
}

void
Camera::setWindowDimensions(const WindowSize &windowSize) {
    mWindowDimensions[0] = windowSize.width;
    mWindowDimensions[1] = windowSize.height;
}

glm::vec3
Camera::getPosition() {
    return mPosition;
}

glm::mat4
Camera::getViewMatrix() {
    return glm::lookAt(mPosition, mPosition + getDirection(), mUp);
}

glm::mat4
Camera::getProjectionMatrix() {
    glm::mat4 projection = glm::perspective(glm::radians(mFov), getAspectRatio(), mZNear, mZFar);
    return projection;
}

void
Camera::processActions(float frameTimeDelta, const std::set<KeymapAction>& actions) {
    float normalizedSpeed = mSpeed * frameTimeDelta;

    if (actions.find(KeymapAction::MOVE_FORWARD) != actions.end()) {
        mPosition += normalizedSpeed * getDirection();
    }

    if (actions.find(KeymapAction::MOVE_BACKWARD) != actions.end()) {
        mPosition -= normalizedSpeed * getDirection();
    }

    if (actions.find(KeymapAction::MOVE_LEFT) != actions.end()) {
        mPosition -= normalizedSpeed * glm::cross(getDirection(), mUp);
    }

    if (actions.find(KeymapAction::MOVE_RIGHT) != actions.end()) {
        mPosition += normalizedSpeed * glm::cross(getDirection(), mUp);
    }

    if (actions.find(KeymapAction::MOVE_UP) != actions.end()) {
        mPosition += normalizedSpeed * mUp;
    }

    if (actions.find(KeymapAction::MOVE_DOWN) != actions.end()) {
        mPosition -= normalizedSpeed * mUp;
    }
}

void
Camera::processMouse(const MousePosition &mousePosition) {

    if (!mInitialMousePositionSet) {
        mLastMouseX = mousePosition.x;
        mLastMouseY = mousePosition.y;
        mInitialMousePositionSet = true;
    }

    float dX = mLastMouseX - mousePosition.x;
    float dY = mLastMouseY - mousePosition.y;

    mLastMouseX = mousePosition.x;
    mLastMouseY = mousePosition.y;

    mYaw += dX * mSensitivity;
    mPitch += dY * mSensitivity;

    mPitch = std::min(mPitch, 89.0f); // can't go to 90 because of lookAt up vector won't work
    mPitch = std::max(mPitch, -89.0f);
}

void
Camera::resetMouse() {
    mInitialMousePositionSet = false;
}

void
Camera::drawDebugPanel() {
    if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Text("View");
        ImGui::InputFloat3("up", &mUp.x);
        ImGui::InputFloat3("position", &mPosition.x);

        float yawDegrees = glm::degrees(mYaw);
        float pitchDegrees = glm::degrees(mPitch);
        float aspectRatio = getAspectRatio();

        ImGui::InputFloat("yaw (degrees)", &yawDegrees);
        ImGui::InputFloat("pitch (degrees)", &pitchDegrees);

        ImGui::Separator();

        ImGui::Text("Projection");
        ImGui::SliderFloat("fov (degrees)", &mFov, 0.0f, 180.0f);
        ImGui::InputInt2("window", mWindowDimensions);
        ImGui::InputFloat("aspect ratio", &aspectRatio);
        ImGui::DragFloat("z-plane near", &mZNear, 0.01f, 0.0f, 1000.0f);
        ImGui::DragFloat("z-plane far", &mZFar, 0.01f, 0.0f, 1000.0f);
    }
}
