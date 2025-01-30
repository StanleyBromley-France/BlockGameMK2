#ifndef PIG_HPP
#define PIG_HPP

#include <glm/glm.hpp>
#include "../Model.h"
#include "../Shader.h"

class Pig {
public:
    Pig(const glm::vec3& position);

    void Render();

    glm::vec3 GetPosition();

private:
    glm::vec3 position;

    static ImportedModel* importedModel;
    static Shader* shader;

    static void LoadResources();
};

#endif
