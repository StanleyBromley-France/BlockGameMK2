#include "include/gladHelper.h"
#include "STB/stb_image.h"

namespace GladHelper {

    bool initialiseAndConfigure() {
        // glad: load all OpenGL function pointers
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            return false;

        glEnable(GL_DEPTH_TEST); // faces unseen by cam arent rendered

        // culls backfaces
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW); 
        glCullFace(GL_BACK);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //glDepthMask(GL_FALSE);  // Disable depth writing

        glEnable(GL_MULTISAMPLE);

        return true;
    }

    unsigned int loadTexture(const char* imagePath, bool flipVertically) {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // texture filtering parameters
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);  // No interpolation when minifying (scaling down)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  // No interpolation when magnifying (scaling up)

 
        float anisoValue = 16.0f; // Could be 16.0f maybe?
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoValue);



        // creates and sends image data
        int width, height, nrChannels;
        if (flipVertically) {
            stbi_set_flip_vertically_on_load(true); // Flip loaded textures on the y-axis
        }
        unsigned char* data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
        if (data) {
            GLenum format = GL_RGB;
            if (nrChannels == 1) {
                format = GL_RED;  // Grayscale image
            }
            else if (nrChannels == 4) {
                format = GL_RGBA;  // RGBA image
            }
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            

        }
        else {
            std::cerr << "Failed to load texture: " << imagePath << std::endl;
        }
        stbi_image_free(data);

        return textureID;
    }

    void ClearScreen() {
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //glClearColor(1.f, 1.f, 1.f, 1.0f);
        glClearColor(0.678f, 0.847f, 1.0f, 1.0f);  // Light blue color

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears both color and depth buffers
    }

    void MeshBuffers::SetupMeshBuffers(const float* vertices, size_t vertexCount, const unsigned int* indices, size_t indexCount) {

        // Generate and bind VAO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // Generate and bind VBO
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), vertices, GL_STATIC_DRAW);

        // Generate and bind EBO
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

        // Configure vertex attributes
        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // texture coordinates attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // unbinds VAO
        glBindVertexArray(0);
    }

    unsigned int MeshBuffers::getVAO() {
        return VAO;
    }

    void MeshBuffers::DeAllocate() {
        // de-allocates all resources once they've outlived their purpose
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
}