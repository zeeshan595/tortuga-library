Tortuga
===

Tortuga is a open source Game Engine built in C++. Tortuga ONLY uses vulkan for its graphics API with NO plans of using any other graphics API. Tortuga is built in Linux but it should be cross platform compatible if you wish to build it for a different platform. For editing the source code & building the project, a `Linux` system is recommended with a copy of `vs code`.

**Building the Project**

Before building Tortuga, you must install the required dependencies. Below is a list of required dependencies for Tortuga to function.
* vulkan (1.1.92)
* SDL2 (with vulkan compatibility)
* SDL_Image
* GLM

You can follow the instructions to install these dependencies in Ubuntu

1. `sudo apt install libvulkan1 libvulkan-dev libsdl2-2.0-0 libsdl2-dev libsdl2-image-2.0-0 libsdl2-image-dev libglm-dev vulkan-utils`
2. SDL2 might not come with vulkan support. But you can compile SDL2 yourself from `https://www.libsdl.org/download-2.0.php` Replace the compiled SDL2 with the system SDL2. `/usr/include/SDL2/` & `/usr/lib/x86_64-linux-gnu/`
3. Download glslang Validator for compiling shaders from `https://github.com/KhronosGroup/glslang/releases`

If using `vs code` open the `Project.code-workspace` and you can debug The project `Sandbox` from there. Otherwise build the project manually:

1. Go into Tortuga sub-folder and run `make build`
2. Navigate to Sandbox sub-folder and run `make build`
3. Open the application inside `Sandbox/Build/Application`

**Key Features**

* Multi GPU Support (Split Screen)
* Multi-Threaded Rendering
* Ray Marching
* Data Oriented Design

**Supported Formats**

Image: `PNG, JPG, BMP, GIF, ICO, TIF, CUR, LBM, PCX, PNM, WEBP, XCF, XV`
