#ifndef GLAD_HELPER_H
#define GLAD_HELPER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Shader.h"

#include "STB/stb_image.h"

namespace GladHelper {

    bool initialiseAndConfigure();
    unsigned int loadTexture(const char* imagePath, bool flipVertically = true);
    void ClearScreen();

    class MeshBuffers {
    public:
        void SetupMeshBuffers(const float* vertices, size_t vertexCount, const unsigned int* indices, size_t indexCount);
        unsigned int getVAO();
        void DeAllocate();
    private:
        unsigned int VBO = 0, VAO = 0, EBO = 0;
    };
}

#endif