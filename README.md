# procedural_dungeon
 A procedural dungeon builder written in C++ using SDL2 and OpenGL.

# Compiling & Running
Make sure you have SDL2, OpenGL, GLEW, glm, and assimp installed and accessible through your CMAKE PATH variable. Then, run:

`cmake CMakeLists.txt`

And after that, run:

`cmake --build .`

In addition to this, you will need to manually import the .dlls, shader files, and 3D models to the output folder. For the shader & 3D models, copy them from the engine directory over to your output build. You will need to download & add the .dlls yourself for SDL2, GLEW, and assimp. GLM should be baked into the project.
