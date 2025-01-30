#include "pig.h"

#include "../GLFWHelper.h"

ImportedModel* Pig::importedModel = nullptr;
Shader* Pig::shader = nullptr;

// Constructor
Pig::Pig(const glm::vec3& position) : position(position) {
    if (!importedModel || !shader) {
        LoadResources(); // loads the model and shader if not already loaded
    }
}

// Render the zombie
void Pig::Render() {
    shader->use();

    // Pass projection matrix to shader
    shader->SetMat4("projection", GLFWHelper::Projection());

    // Camera/view transformation
    glm::mat4 view = glm::mat4(1.0f);
    view = GLFWHelper::LookAt();
    shader->SetMat4("view", view);

    // Model transformation
    glm::mat4 model = glm::mat4(1.0f);

    // Apply rotation based on time for spinning effect
    float time = static_cast<float>(glfwGetTime()); // Get elapsed time in seconds
    model = glm::rotate(model, glm::radians(time * 100.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Spins around the Y-axis

    model = glm::translate(model, position); // Translate model to given position
    model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f)); // Resize model

    shader->SetMat4("model", model);

    importedModel->Draw(*shader);
}



// getter for position
glm::vec3 Pig::GetPosition() {
    return position;
}

// loads the static model and shader resources
void Pig::LoadResources() {
    if (!importedModel) {
        importedModel = new ImportedModel("objects/pig/pig.glb");
    }
    if (!shader) {
        shader = new Shader("shaders/model_import_vertex.glsl", "shaders/model_import_frag.glsl");
    }
}