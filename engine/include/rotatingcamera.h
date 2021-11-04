#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "imgui/imgui.h"

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
    float time = 0.0f;
    float timePerCircle = 10;
    float radius = 4.0f;
    // view matrix stuff
	glm::vec3 up;
	glm::vec3 position = glm::vec3(3.0f, 1.0f, 3.0f);
    // projection matrix stuff
    float fov = 45.0f;
    int windowDimensions[2] = { 800, 600 }; 
    float zNear = 0.1f;
    float zFar = 100.0f;
};