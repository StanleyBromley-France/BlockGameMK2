#version 400 core


// the final color output of the fragment shader
// 'out' declares a variable that stores the final color of this fragment
out vec4 FragColor;

// texture coordinates passed from the vertex shader
// these define how a texture is mapped to the fragment
// 'in' declares variables that receive data from the vertex shader
in vec2 TexCoord;

flat in float alpha;

// 'uniform' declares variables that stay constant for all fragments during a draw call
// these are set from the application side and used in the shader
// a sampler2D is a special type of uniform variable used to fetch colors from a 2D texture
// it represents a texture image and allows us to retrieve pixel colors
// texture1 is assinged a place in CubeMesh.cpp with single shader at position 0
// it is switched when using 
// glActiveTexture(GL_TEXTURE0);
// glBindTexture(GL_TEXTURE_2D, desiredTexture);
uniform sampler2D texture1;

uniform bool u_isGrayscale;

void main()
{
    vec4 texColor = texture(texture1, TexCoord);

    // apply the calculated alpha (alpha increases with distance)
    if (u_isGrayscale) {
        // Use red channel for all RGB components
        FragColor = vec4(texColor.rrr, alpha);
    } else {
        // Use original RGB values
        FragColor = vec4(texColor.rgb, alpha);
    }
}
