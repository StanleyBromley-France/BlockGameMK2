#ifndef GLFWHelper_H
#define GLFWHelper_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace GLFWHelper {

    // settings
    extern const unsigned int SCR_WIDTH;
    extern const unsigned int SCR_HEIGHT;

    extern bool isConfirmQuitActive;
    // camera
    extern glm::vec3 cameraPos;
    extern glm::vec3 cameraFront;
    extern glm::vec3 cameraUp;

    extern bool firstMouse;
    extern float yaw;
    extern float pitch;
    extern float lastX;
    extern float lastY;
    extern float fov;

    // timing
    extern float deltaTime;
    extern float lastFrame;

    // Function declarations
    GLFWwindow* initialiseAndConfigure();
    glm::mat4 LookAt();
    glm::mat4 Projection();
    void UpdateTime();
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void processInput(GLFWwindow* window);
    void handleMovementInput(GLFWwindow* window);
}

#endif // GLFWHelper_H