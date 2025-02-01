#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <STB/stb_image.h>
#include <FastNoiseLite.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "GLFWHelper.h"
#include "GladHelper.h"

#include <algorithm>
#include <iostream>
#include <vector>

#include "Biomes.h"
#include "map.h"

double lastTime = 0.0;
int frameCount = 0;
double frameTime = 0.0;

void calculateFPS(GLFWwindow* window) {
    double currentTime = glfwGetTime();
    frameTime = currentTime - lastTime;
    frameCount++;
    // Update FPS every second
    if (frameTime >= 1.0) {
        std::cout << "FPS: " << frameCount << std::endl;
        frameCount = 0;
        lastTime = currentTime;
    }
}

int main()
{
    // window creation
    GLFWwindow* window = GLFWHelper::initialiseAndConfigure();

    if (window == NULL) // makes sure window exists
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // glad creation
    if (!GladHelper::initialiseAndConfigure())
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    unsigned int randomSeed = static_cast<unsigned int>(std::time(nullptr));

    Map map = Map(32, 32, randomSeed);



    // render loop
    while (!glfwWindowShouldClose(window))
    {
        GLFWHelper::UpdateTime();

        // input
        GLFWHelper::processInput(window);

        GladHelper::ClearScreen();

        for (auto& chunk : map.chunks) {
            if (glm::distance(chunk.chunkPos, GLFWHelper::cameraPos) < (16.0f * 16)) {
                chunk.RenderChunk();
            }
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();

        calculateFPS(window);
    }

    for (auto& chunk : map.chunks) {
        chunk.Deallocate();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}