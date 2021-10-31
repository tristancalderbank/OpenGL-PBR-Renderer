#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "imgui/imgui.h"

class Camera {

public:
    Camera(
        glm::vec3 up,
        glm::vec3 position,
        float yaw,
        float pitch,
        int windowWidth,
        int windowHeight
    );

    void setWindowDimensions(int width, int height);
    glm::vec3 getPosition();
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
    void processKeyboard(GLFWwindow* window, float frameTimeDelta);
    void processMouseMovement(GLFWwindow* window, double xPos, double yPos);
    void drawDebugPanel();

private:
    glm::vec3 getDirection();
    float getAspectRatio();

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
    int windowDimensions[2] = { 800, 600 }; 
    float zNear = 0.1f;
    float zFar = 100.0f;
};