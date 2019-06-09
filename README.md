Tortuga
===

Tortuga is a open source Game Engine built in C++ 11. Tortuga ONLY uses vulkan for its graphics API with NO plans of using any other graphics API. Tortuga is built in Linux but it should be cross platform compatible if you wish to build it for a different platform. For editing the source code & building the project, a `Linux` system is recommended with a copy of `vs code`.

**Building the Project**

Before building Tortuga, you must install the required dependencies. Below is a list of required dependencies for Tortuga to function.
* vulkan (1.1.92)
* SDL2 (with vulkan compatibility)
* GLM
* NPM

If using vs code after step 4 you can use the debug menu to build and run the application.

1. Make sure npm is installed `sudo apt install npm`
2. Download the project `git clone git@github.com:zeeshan595/Tortuga.git`
3. Change to the project directory `cd Tortuga`
4. Install dependencies `npm install`
5. Build the project `npm run build`
6. Change to build directory `cd Editor/Build`
7. Launch the application `./Application`

**Key Features**

* Multi GPU Support (Split Screen)
* Multi-Threaded Rendering
* Ray Marching
* Data Oriented Design

**Supported Formats**

Image: `PNG, JPG, BMP, GIF, ICO, TIF, CUR, LBM, PCX, PNM, WEBP, XCF, XV`
