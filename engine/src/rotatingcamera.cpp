#include "rotatingcamera.h"

#include "constants.h"

float
RotatingCamera::getAspectRatio() {
    return (float) mWindowDimensions[0] / mWindowDimensions[1];
}

RotatingCamera::RotatingCamera(
    glm::vec3 up,
    int windowWidth,
    int windowHeight
) : mUp(up) {
    mWindowDimensions[0] = windowWidth;
    mWindowDimensions[1] = windowHeight;
}

void
RotatingCamera::setWindowDimensions(int width, int height) {
    mWindowDimensions[0] = width;
    mWindowDimensions[1] = height;
}

glm::vec3
RotatingCamera::getPosition() {
    return mPosition;
}

glm::mat4
RotatingCamera::getViewMatrix() {
    return glm::lookAt(mPosition, constants::origin, mUp);
}

glm::mat4
RotatingCamera::getProjectionMatrix() {
    glm::mat4 projection = glm::perspective(glm::radians(mFov), getAspectRatio(), mZNear, mZFar);
    return projection;
}

void
RotatingCamera::update(float frameTimeDelta) {
    mTime += frameTimeDelta;

    float percentOfCircle = mTime / mTimePerCircle;
    float angle = percentOfCircle * 2.0 * constants::PI;

    mPosition.x = sin(angle) * mRadius;
    mPosition.z = cos(angle) * mRadius;
}

void
RotatingCamera::drawDebugPanel() {
    if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Text("View");
        ImGui::InputFloat3("up", &mUp.x);
        ImGui::InputFloat3("position", &mPosition.x);

        float aspectRatio = getAspectRatio();

        ImGui::Separator();

        ImGui::Text("Projection");
        ImGui::SliderFloat("fov (degrees)", &mFov, 0.0f, 180.0f);
        ImGui::InputInt2("window", mWindowDimensions);
        ImGui::InputFloat("aspect ratio", &aspectRatio);
        ImGui::DragFloat("z-plane near", &mZNear, 0.01f, 0.0f, 1000.0f);
        ImGui::DragFloat("z-plane far", &mZFar, 0.01f, 0.0f, 1000.0f);
    }
}
