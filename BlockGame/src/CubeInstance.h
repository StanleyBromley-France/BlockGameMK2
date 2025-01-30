#ifndef CUBE_INSTANCE_H
#define CUBE_INSTANCE_H

#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "GladHelper.h"
#include "GLFWHelper.h"

#include "CubeMesh.h"


class CubeInstance
{
public:
    CubeInstance(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f)); // Constructor to initialize the cube
    void Render();       // Render the cube
    void DeAllocate();
private:
    glm::vec3 position;
};

#endif
