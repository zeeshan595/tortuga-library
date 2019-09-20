# tortuga-library

Tortuga is a open source Game Engine built in C++ 17. Tortuga ONLY uses vulkan for its graphics API with NO plans of using any other graphics API. Tortuga is built for Linux (Wayland) and currently does NOT have support for any other platforms. For editing the source code & building the project, a `Linux` system is recommended with a copy of `vs code` or `gnome-builder`.

**Key Features**

* Multi-Threaded Rendering
* Data Oriented Design
* Entity Component System
* Event Based Input System
* Ray Marching (Signed Distance) _[NOT IMPLIMENTED]_
* Multi GPU Support (Split Screen) _[NOT IMPLIMENTED]_

**Required Resources**

* vulkan https://www.khronos.org/vulkan/
* vulkan-validation-layers (This is an extra vulkan package for debugging)
* shaderc https://github.com/google/shaderc
* glm https://github.com/g-truc/glm
* meson https://mesonbuild.com/

*Linux*

Use your package manager to install `libglm`, `libvulkan` and `meson`

You can download `shaderc` from github and place the extracted folders (`bin`, `include`, `lib`, `share`) inside `/usr/`

**Building the library**

1. Download the project `git clone https://github.com/zeeshan595/tortuga-library`
2. Use meson to build the project `meson Build && cd Build && ninja`
3. Copy Shaders folder to build directory `cp -r ../Shaders/ ./`
4. Copy Models folder to build directory `cp -r ../Models/ ./`
5. Run engine `./tortuga`