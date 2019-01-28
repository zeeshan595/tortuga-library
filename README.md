Tortuga
===

Tortuga is a open source Game Engine built in C++. Tortuga ONLY uses vulkan for its graphics API with NO plans of using any other graphics API. Tortuga is built in Linux but it should be cross platform compatible if you wish to build it for a different platform. For editing the source code & building the project, a `Linux` system is recomended with a copy of `vs code`.

**Building the Project**

Before building Tortuga, you must install the required dependencies. Below is a list of required dependencies for Tortuga to function. The project expects to find all of the dependencies in `/usr/lib` and `/usr/include` folders.
* pthread
* vulkan (1.1.92)
* SDL2 (with vulkan compatibility)
* sndio

If using `vs code` open the `Project.code-workspace` and you can debug The project `Sandbox` from there. Otherwise build the project manually:

1. Go into Tortuga sub-folder and run `make`
2. Navigate to Sandbox sub-folder and run `make`
3. Open the application inside `Sandbox/Build/Application`
