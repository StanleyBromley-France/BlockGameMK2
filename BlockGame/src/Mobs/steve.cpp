#include "steve.h"

#include "../GLFWHelper.h"

ImportedModel* Steve::importedModel = nullptr;
Shader* Steve::shader = nullptr;

// Constructor
Steve::Steve(const glm::vec3& position) : position(position) {
    if (!importedModel || !shader) {
        LoadResources(); // loads the model and shader if not already loaded
    }
}

// Render the zombie
void Steve::Render() {
    shader->use();

    // pass projection matrix to shader
    shader->SetMat4("projection", GLFWHelper::Projection());

    // camera/view transformation
    glm::mat4 view = glm::mat4(1.0f);
    view = GLFWHelper::LookAt();
    shader->SetMat4("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position); // translates model to given position
    model = glm::scale(model, glm::vec3(.10f, .10f, .10f));	// resizes model
    shader->SetMat4("model",model);

    importedModel->Draw(*shader);
}

// getter for position
glm::vec3 Steve::GetPosition() {
    return position;
}

// loads the static model and shader resources
void Steve::LoadResources() {
    if (!importedModel) {
        importedModel = new ImportedModel("objects/steve/steve.obj", true);
    }
    if (!shader) {
        shader = new Shader("shaders/model_import_vertex.glsl", "shaders/model_import_frag.glsl");
    }
}