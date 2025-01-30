#version 400 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 3) in mat4 instanceMatrix;

out vec2 TexCoord;
flat out float col; // Use float instead of double

uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 worldPosition = instanceMatrix * vec4(aPos, 1.0);
    vec4 viewPosition = view * worldPosition;

    float distance = length(viewPosition.xyz);
    
    float fadeStart = 208.0;  // Fade starts 3 chunks before the max render distance
    float fadeEnd = 224.0;    // Fade lasts for 16 units

    if (distance <= fadeStart)
        col = 1.0;  // Fully visible
    else
        col = 1.0 - clamp((distance - fadeStart) / (fadeEnd - fadeStart), 0.0, 1.0);

    gl_Position = projection * viewPosition;
    TexCoord = aTexCoord;
}