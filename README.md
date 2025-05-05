# BlockGame

This repository contains the source code for a terrain-based prototype inspired by Minecraft, where users can fly around the scene.

## Dependencies

This project uses the following dependencies:
- **GLAD**: OpenGL loader, used to manage OpenGL functions.
- **GLFW**: For handling window creation, input processing, and context management.
- **STB**: A library for image loading.
- **GLM**: A math library for graphics programming, used for transformations and vector mathematics.
- **FAST NOISE LITE**: This is used to generate noise maps that the terrain use.

## Game Controls

- WASD to move around
- Mouse movement to look around
- Hold shift to speed up movement
- Space bar moves up
- Left control moves down
- Scroll to zoom in/out

## Project Details

- **Terrain Generation**: The game loads terrain similar to Minecraft, with a limited map size to avoid excessive memory usage.
-  **Instancing**: To lower the amount of draw calls, I grouped blocks in a chunk using instancing. This created a limitation of all the blocks in that chunk having the same texure. Despite this, it was worth it to ipliment as it massivly improved performance (around 30 fps to 3000+).

## Future Development

- **Improved Terrain Generation**: Converting the terrain generation to be seemless throught the chunks
- **Unlimited Map Size**: Storing unloaded chunks in a file. Currently all chunks are kept in memory permentantly, even when not being rendered. This brings the limitation of the map size, and I have plans to change this.

