# tortuga-library

Tortuga is a open source Game Engine built in C++ 17. Tortuga ONLY uses vulkan for its graphics API with NO plans of using any other graphics API. Tortuga is built for linux but should be compatible on Mac OS X and Windows. For editing the source code & building the project a copy of `vs code` is recomended.

**Key Features**

* Multi-Threaded Rendering
* Data Oriented Design
* Entity Component System
* Event Based Input System
* Ray Marching (Signed Distance)
* Garbage Collection
* Multi GPU Support (Split Screen) _[NOT IMPLIMENTED]_

**Required Resources**

* vulkan https://www.khronos.org/vulkan/
* vulkan-validation-layers (This is an extra vulkan package for debugging)
* shaderc https://github.com/google/shaderc
* glfw https://www.glfw.org/
* glm https://github.com/g-truc/glm
* meson https://mesonbuild.com/

*Linux*

Use your package manager to install `libglm`, `libglfw`, `libvulkan` and `meson`

You can download `shaderc` from github and place the extracted folders (`bin`, `include`, `lib`, `share`) inside `/usr/`

**Building the library**

1. Download the project `git clone https://github.com/zeeshan595/tortuga-library`
2. Open project with vs code
3. Run Configure task
4. Launch project