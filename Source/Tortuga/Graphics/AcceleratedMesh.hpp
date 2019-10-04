#ifndef _ACCELERATED_MESH
#define _ACCELERATED_MESH

#include <vector>
#include <cstdint>
#include <glm/glm.hpp>

namespace Tortuga
{
namespace Graphics
{
struct AcceleratedMesh
{
public:
  struct Node
  {
    glm::vec4 Center;
    float Radius;
    uint32_t Left;
    uint32_t Right;
    uint32_t Index;
    int32_t IsEnd;
  };
  struct IndexStruct
  {
    uint32_t Position;
    uint32_t Texture;
    uint32_t Normal;
  };
  struct Face
  {
    glm::vec4 Center;
    uint32_t Indices[3];
  };
  //provided by user
  std::vector<glm::vec3> Positions;
  std::vector<glm::vec2> Textures;
  std::vector<glm::vec3> Normals;
  std::vector<IndexStruct> Indices;

  //generated
  std::vector<Node> Nodes;
  std::vector<Face> Faces;
  uint32_t MeshDataByteSize;

  AcceleratedMesh();
  AcceleratedMesh(std::vector<glm::vec3> positions, std::vector<glm::vec2> textures, std::vector<glm::vec3> normals, std::vector<IndexStruct> indices);
};
AcceleratedMesh AcceleratedMeshRecalculateVolumes(AcceleratedMesh data);
void FindMinNodes(std::vector<AcceleratedMesh::Node> nodes, uint32_t &left, uint32_t &right);
} // namespace Graphics
} // namespace Tortuga

#endif