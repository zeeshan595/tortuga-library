# tortuga-library

Tortuga is a open source Game Engine built in C++ 11. Tortuga ONLY uses vulkan for its graphics API with NO plans of using any other graphics API. 
Tortuga is built in Linux but it should be cross platform compatible if you wish to build it for a different platform. 
For editing the source code & building the project, a `Linux` system is recommended with a copy of `vs code`/`gnome-builder`.

Tortuga Editor UI: https://github.com/zeeshan595/tortuga-editor

**Building the library**

1. Download the project `git clone https://github.com/zeeshan595/tortuga-library`
2. Use meson to build the project `meson build && cd build && ninja`
3. The library will be located in `build/libtortuga.so`

**Key Features**

* Multi GPU Support (Split Screen)
* Multi-Threaded Rendering
* Ray Marching
* Data Oriented Design

**Supported Formats**

Image: `PNG, JPG, BMP, GIF, ICO, TIF, CUR, LBM, PCX, PNM, WEBP, XCF, XV`
