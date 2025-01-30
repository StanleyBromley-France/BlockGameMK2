#version 400 core
out vec4 FragColor;

in vec2 TexCoord;
flat in float col; // Match type with vertex shader

uniform sampler2D texture1;

void main()
{
    vec4 texColor = texture(texture1, TexCoord);

    // Apply the calculated alpha (col increases with distance)
    FragColor = vec4(texColor.rgb, col);
}
