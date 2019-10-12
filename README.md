# tortuga-library

Tortuga is a open source Game Engine built in C++ 17. Tortuga ONLY uses vulkan for its graphics API with NO plans of using any other graphics API. Tortuga is built for linux but should be compatible on Mac OS X and Windows.

## Key Features

* Multi-Threaded Rendering
* Data Oriented Design
* Entity Component System
* Event Based Input System
* Ray Marching (Signed Distance)
* Garbage Collection
* Multi GPU Support (Split Screen) _[NOT IMPLIMENTED]_

## Building the library

##### With Makefile

1. Download the project `git clone https://github.com/zeeshan595/tortuga-library`
2. Change Directory `cd tortuga-library`
3. Setup required dependencies `make init`
4. Build project `make`

##### With VS Code

1. Download the project `git clone https://github.com/zeeshan595/tortuga-library`
2. Open project with vs code
3. Run 'init' task to get and build submodules
4. Launch project