#version 400 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 3) in mat4 instanceMatrix;

out vec2 TexCoord;
flat out double col;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec4 newaPos = vec4(aPos, 1);


	gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0f);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);

	col = (length(gl_Position))/ (16*16*2);
}