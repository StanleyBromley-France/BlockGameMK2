# BlockGame

This repository contains the source code for a terrain-based prototype inspired by Minecraft, where users can fly around the scene.

## Video Walkthrough

https://youtu.be/Hk4ZPrVGvnc

## Dependencies

This project uses the following dependencies:
- **Assimp**: For model loading and handling.
- **GLEW**: OpenGL Extension Wrangler, used for managing OpenGL extensions.
- **GLAD**: OpenGL loader, used to manage OpenGL functions.
- **GLFW**: For handling window creation, input processing, and context management.
- **STB**: A library for image loading.
- **GLM**: A math library for graphics programming, used for transformations and vector mathematics.

Ensure these dependencies are installed before running the project.

## Game Controls

- WASD to move around
- Mouse movement to look around
- Hold shift to speed up movement
- Space bar moves up
- Left control moves down
- Scroll to zoom in/out

## Game Mechanics

The game allows the user to fly around a procedurally generated terrain, similar to the mechanics of Minecraft. The core functionality focuses on:
- **User Movement**: The player can freely fly around the scene to explore the terrain.
- **Terrain Generation**: The terrain is procedurally generated with a fixed map size, similar to Minecraft's world generation approach.

The mechanics are coded using standard game programming techniques, focusing on rendering and user input handling.

## Performance vs Good Practice

During development, a key performance trade-off was made:
- **Instancing**: To lower the amount of draw calls, I grouped blocks in a chunk using instancing. This created a limitation of all the blocks in that chunk having the same texure. Despite this, it was worth it to ipliment as it massivly improved performance (around 30 fps to 3000+)

## Further Details

- **Terrain Generation**: The game loads terrain similar to Minecraft, with a limited map size to avoid excessive memory usage.
- The project primarily focuses on visual representation and scene exploration.

## Evaluation & Reflection

Overall, I am happy with the progress of the prototype. The terrain generation works as expected, and the user can explore the scene smoothly. However, if I had more time, I would have focused on refining the terrain generation process to make it more dynamic and varied.
