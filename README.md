# tortuga-library

Tortuga is a open source Game Engine built in C++ 11. Tortuga ONLY uses vulkan for its graphics API with NO plans of using any other graphics API. 
Tortuga is built in Linux but it should be cross platform compatible if you wish to build it for a different platform. 
For editing the source code & building the project, a `Linux` system is recommended with a copy of `vs code`/`gnome-builder`.

Tortuga Editor UI: https://github.com/zeeshan595/tortuga-editor

**Key Features**

* Multi-Threaded Rendering
* Ray Marching
* Data Oriented Design
* Multi GPU Support (Split Screen) _[NOT IMPLIMENTED]_

**Required Resources**

* vulkan https://www.khronos.org/vulkan/
* vulkan-validation-layers (This is an extra vulkan package for debugging)
* shaderc https://github.com/google/shaderc
* sdl2 https://www.libsdl.org/download-2.0.php
* glm https://github.com/g-truc/glm
* meson https://mesonbuild.com/

*Linux*

Use your package manager to install `libglm`, `libvulkan`, `libsdl2` and `meson`

You can download `shaderc` from github and place the extracted folders (`bin`, `include`, `lib`, `share`) inside `/usr/`

*Mac OSX*

Use brew to install `glm`, `sdl2`, `meson`, `ninja`

Download install vulkan from khronos

You can download `shaderc` from github and place the extracted folders (`bin`, `include`, `lib`, `share`) inside `/usr/


**Building the library**

1. Download the project `git clone https://github.com/zeeshan595/tortuga-library`
2. Use meson to build the project `meson build && cd build && ninja`
3. The library will be located in `build/libtortuga.so`
4. Run engine `cd build && ./tortuga`