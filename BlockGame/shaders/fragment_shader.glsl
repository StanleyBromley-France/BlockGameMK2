#version 400 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

flat in double col;


uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(TexCoord.x, TexCoord.y)), 0.3);
    FragColor += vec4(col, col, col, 1);

}