#ifndef CUBE_MESH_H
#define CUBE_MESH_H

#include "Shader.h"
#include "GladHelper.h"

class CubeMesh
{
public:
    CubeMesh();

    enum class Texture
    {
        SAND,
        DIRT,
        STONE
    };

    Shader* GetShader(Texture texture);
    GladHelper::MeshBuffers& GetMeshBuffers();


    void SwitchTexture(Texture texture);

private:
    static Shader* ShaderSingle;
    static Shader* ShaderMixed;
    GladHelper::MeshBuffers MeshBuffers;

    static unsigned int sandTexture;
    static unsigned int dirtTexture;
    static unsigned int stoneTexture;

};
#endif