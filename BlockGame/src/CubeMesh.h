#ifndef CUBE_MESH_H
#define CUBE_MESH_H

#include "Shader.h"
#include "GladHelper.h"

class CubeMesh
{
public:
    static CubeMesh& GetInstance();


    enum class Texture
    {
        SAND,
        DIRT,
        STONE
    };

    Shader& GetShader(Texture texture);
    GladHelper::MeshBuffers& GetMeshBuffers();


    void SwitchTexture(Texture texture);

private:
    CubeMesh();
    // prevents creating another instance of this class in code
    CubeMesh(const CubeMesh&) = delete;
    CubeMesh& operator=(const CubeMesh&) = delete;

    Shader Shader;
    GladHelper::MeshBuffers MeshBuffers;

    unsigned int sandTexture;
    unsigned int dirtTexture;
    unsigned int stoneTexture;

};
#endif