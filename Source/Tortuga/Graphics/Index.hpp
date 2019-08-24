#ifndef _GRAPHICS_INDEX
#define _GRAPHICS_INDEX

#include <fstream>

namespace Tortuga {
  namespace Graphics {
    struct Index {
      uint32_t Vertex;
      uint32_t Normal;
      uint32_t Texture;
    };
  }
}

#endif