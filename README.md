# tortuga-library

Tortuga is a open source Game Engine built in C++ 11. Tortuga ONLY uses vulkan for its graphics API with NO plans of using any other graphics API. Tortuga is built in Linux but it should be cross platform compatible if you wish to build it for a different platform. For editing the source code & building the project, a `Linux` system is recommended with a copy of `vs code`/`gnome-builder`.

**Key Features**

* Multi-Threaded Rendering
* Data Oriented Design
* Entity Component System
* Ray Marching (Signed Distance) _[NOT IMPLIMENTED]_
* Multi GPU Support (Split Screen) _[NOT IMPLIMENTED]_

**Required Resources**

* vulkan https://www.khronos.org/vulkan/
* vulkan-validation-layers (This is an extra vulkan package for debugging)
* shaderc https://github.com/google/shaderc
* glfw https://www.glfw.org/
* glm https://github.com/g-truc/glm
* meson https://mesonbuild.com/

*Linux*

Use your package manager to install `libglm`, `libvulkan`, `libglfw3` and `meson`

You can download `shaderc` from github and place the extracted folders (`bin`, `include`, `lib`, `share`) inside `/usr/`

*Mac OSX*

Use brew to install `glm`, `glfw`, `meson`, `ninja`

Download install vulkan from khronos

You can download `shaderc` from github and place the extracted folders (`bin`, `include`, `lib`, `share`) inside `/usr/


**Building the library**

1. Download the project `git clone https://github.com/zeeshan595/tortuga-library`
2. Use meson to build the project `meson Build && cd Build && ninja`
3. Copy Shaders folder to build directory `cp -r ../Shaders/ ./`
4. Copy Models folder to build directory `cp -r ../Models/ ./`
5. Run engine `./tortuga`