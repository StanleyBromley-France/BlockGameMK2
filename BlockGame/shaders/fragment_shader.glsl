#version 330 core

// the final color output of the fragment shader
// 'out' declares a variable that stores the final color of this fragment
out vec4 OutputColor;

// texture coordinates passed from the vertex shader
// these define how a texture is mapped to the fragment
// 'in' declares variables that receive data from the vertex shader
in vec2 TextureCoordinates;

// 'uniform' declares variables that stay constant for all fragments during a draw call
// these are set from the application side and used in the shader
// a sampler2D is a special type of uniform variable used to fetch colors from a 2D texture
// it represents a texture image and allows us to retrieve pixel colors 
uniform sampler2D FirstTexture;
uniform sampler2D SecondTexture;

void main()
{
    // samples the first texture at the given texture coordinates
    vec4 firstTextureColor = texture(FirstTexture, TextureCoordinates);
    
    // samples the second texture at the same coordinates
    vec4 secondTextureColor = texture(SecondTexture, TextureCoordinates);

    // blends the two textures together using a mix ratio of 0.3 (30% second texture, 70% first texture)
    OutputColor = mix(firstTextureColor, secondTextureColor, 0.3);
}