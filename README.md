# minigl

OpenGL library to provide common functionality for my 3D school projects. The library covers three different aspects:

* A (varyingly) thin wrapper over the core OpenGL API to provide a more modern and C++-like interface. This comprises abstractions over render commands, buffer objects, etc.
* An application class that uses GLFW to create a window and manage the OpenGL context, input, and camera. This class is meant to be derived from to create custom applications.
* Utility classes for loading and managing shaders and models, as well as some math functions and geometry wrapping over GLM.

The name comes not from the size of the library, but because it provides a minimal set of functionality that is useful for my projects. As such, it is not meant to be full featured or general purpose, and is at the moment still very much a work in progress.