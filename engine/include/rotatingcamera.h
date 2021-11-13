#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "imgui/imgui.h"

/**
 * A camera that rotates in a circle around a point.
 */
class RotatingCamera {
public:
    RotatingCamera(
        glm::vec3 up,
        int windowWidth,
        int windowHeight
    );

    void setWindowDimensions(int width, int height);
    glm::vec3 getPosition();
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
    void update(float frameTimeDelta);
    void drawDebugPanel();

private:
    glm::vec3 getDirection();
    float getAspectRatio();

private:
    float mTime = 0.0f;
    float mTimePerCircle = 10;
    float mRadius = 4.0f;
    // view matrix stuff
	glm::vec3 mUp;
	glm::vec3 mPosition = glm::vec3(3.0f, 1.0f, 3.0f);
    // projection matrix stuff
    float mFov = 45.0f;
    int mWindowDimensions[2] = { 800, 600 };
    float mZNear = 0.1f;
    float mZFar = 100.0f;
};