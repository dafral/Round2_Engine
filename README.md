# Round2_Engine

Engine developed from scratch for 3D Engines subject using C++ language. 


## Usage
A 3D engine that allows you to see 3D models and try different textures on them. 


## How to use it
In order to load a fbx file, drag and drop it on the engine window. You can do the same with textures. Remember that the only files supported by the engines are FBX files for the 3D models and DDS/PNG files for the textures.

You can find some example files inside the "Assets" folder.


### Camera Controls 
- **"WASD":** fps-like movement
- **"QE":** Up & down
- **Shift:** Accelerate camera
- **Mouse wheel:** zoom in and out
- **Right click + Move Mouse:** orbital camera

## Clarifications
- There is no resource manager.
- When you load the scene, the camera goes to the position 0,0,0.

## Innovation
- Bounding Volume (GameObjects don't have only AABBs, they have also spheres around them).
- Octree instead of KDtree


## Libraries used
	- SDL 2.0.4

	- OpenGL 

	- Glew 2.1.0

	- Assimp 3.1.1

	- Devil 1.8.0

	- ImGui 1.65 
	
	- MathGeoLib 
	
	- ImguiDock 1.0


## Contact us
Github repository:

- https://github.com/dafral/Round2_Engine

Download the latest version:

- https://github.com/dafral/Round2_Engine/releases

Report a bug

- https://github.com/dafral/Round2_Engine/issues/new

### Project contributors
Marc de Pedro (https://github.com/marcdp1997)

David Franco (https://github.com/dafral)




MIT License

Copyright (c) 2017 