#include "rotatingcamera.h"

#include "constants.h"

float
RotatingCamera::getAspectRatio() {
    return (float) windowDimensions[0] / windowDimensions[1];
}

RotatingCamera::RotatingCamera(
    glm::vec3 up,
    int windowWidth,
    int windowHeight
) : up(up) {
    windowDimensions[0] = windowWidth;
    windowDimensions[1] = windowHeight;
}

void
RotatingCamera::setWindowDimensions(int width, int height) {
    windowDimensions[0] = width;
    windowDimensions[1] = height;
}

glm::vec3
RotatingCamera::getPosition() {
    return position;
}

glm::mat4
RotatingCamera::getViewMatrix() {
    return glm::lookAt(position, constants::origin, up);
}

glm::mat4
RotatingCamera::getProjectionMatrix() {
    glm::mat4 projection = glm::perspective(glm::radians(fov), getAspectRatio(), zNear, zFar);
    return projection;
}

void
RotatingCamera::update(float frameTimeDelta) {
    time += frameTimeDelta;

    float percentOfCircle = time / timePerCircle;
    float angle = percentOfCircle * 2.0 * constants::PI;

    position.x = sin(angle) * radius;
    position.z = cos(angle) * radius;
}

void
RotatingCamera::drawDebugPanel() {
    if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Text("View");
        ImGui::InputFloat3("up", &up.x);
        ImGui::InputFloat3("position", &position.x);

        float aspectRatio = getAspectRatio();

        ImGui::Separator();

        ImGui::Text("Projection");
        ImGui::SliderFloat("fov (degrees)", &fov, 0.0f, 180.0f);
        ImGui::InputInt2("window", windowDimensions);
        ImGui::InputFloat("aspect ratio", &aspectRatio);
        ImGui::DragFloat("z-plane near", &zNear, 0.01f, 0.0f, 1000.0f);
        ImGui::DragFloat("z-plane far", &zFar, 0.01f, 0.0f, 1000.0f);
    }
}
