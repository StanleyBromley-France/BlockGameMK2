#version 330 core

// Input vertex attributes
layout (location = 0) in vec3 vertexPosition;   // Position of the vertex (x, y, z)
layout (location = 1) in vec2 textureCoordinates; // Texture coordinates (u, v)
layout (location = 3) in mat4 instanceTransformationMatrix; // Transformation matrix for instanced rendering

// Output variables to pass to fragment shader
out vec2 TexCoord;  // Output texture coordinates for fragment shader
flat out float alpha;  // Output alpha value for transparency (0 = fully transparent, 1 = fully opaque)

// Uniforms for camera view and projection matrices
uniform mat4 viewMatrix;     // View transformation matrix (camera space)
uniform mat4 projectionMatrix; // Projection transformation matrix (perspective/orthogonal)

void main()
{
    // Calculate the world position of the vertex by applying the instance transformation matrix
    vec4 worldPosition = instanceTransformationMatrix * vec4(vertexPosition, 1.0);

    // Apply the view transformation to the world position to get the position in camera space
    vec4 viewPosition = viewMatrix * worldPosition;

    // Calculate the distance from the camera (view position)
    float distanceFromCamera = length(viewPosition.xyz);

    // Define the start and end distance for the fade effect
    float fadeStartDistance = 208.0;  // Fade effect starts when the object is 3 chunks away from the max render distance
    float fadeEndDistance = 224.0;    // Fade effect finishes when the object is 16 units further

    // If the object is closer than the fade start distance, it's fully visible
    if (distanceFromCamera <= fadeStartDistance)
        alpha = 1.0;  // Fully opaque
    else
        // Gradually fade out based on distance
        alpha = 1.0 - (distanceFromCamera - fadeStartDistance) / (fadeEndDistance - fadeStartDistance);

    // Calculate the final position of the vertex in clip space by applying the projection transformation
    gl_Position = projectionMatrix * viewPosition;

    // Pass the texture coordinates to the fragment shader
    TexCoord = textureCoordinates;
}
