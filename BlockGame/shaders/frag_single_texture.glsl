#version 400 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

flat in double col;

uniform sampler2D texture1;

void main()
{


    FragColor = texture(texture1, TexCoord) + vec4(col,col,col, 1);
}