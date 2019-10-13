#include "./AcceleratedMesh.hpp"

#include "../Core/Console.hpp"

namespace Tortuga
{
namespace Graphics
{
AcceleratedMesh::AcceleratedMesh()
{
}
AcceleratedMesh::AcceleratedMesh(
    std::vector<glm::vec3> positions,
    std::vector<glm::vec2> textures,
    std::vector<glm::vec3> normals,
    std::vector<IndexStruct> indices)
{
  Positions = positions;
  Textures = textures;
  Normals = normals;
  Indices = indices;
  const auto data = AcceleratedMeshRecalculateVolumes((*this));
  Faces = data.Faces;
  Nodes = data.Nodes;
  MeshDataByteSize = 0;
  MeshDataByteSize += sizeof(glm::vec3) * Positions.size();
  MeshDataByteSize += sizeof(glm::vec2) * Textures.size();
  MeshDataByteSize += sizeof(glm::vec3) * Normals.size();
}
void FindMinNodes(std::vector<AcceleratedMesh::Node> nodes, uint32_t &left, uint32_t &right)
{
  std::vector<uint32_t> data(2);
  uint32_t dist = std::numeric_limits<uint32_t>::max();
  for (uint32_t i = 0; i < nodes.size(); i++)
  {
    const auto leftNode = nodes[i];
    for (uint32_t j = 0; j < nodes.size(); j++)
    {
      if (i == j)
        continue;

      const auto rightNode = nodes[j];
      const auto newDist = glm::distance(leftNode.Center, rightNode.Center);
      if (newDist < dist)
      {
        left = i;
        right = j;
        dist = newDist;
      }
    }
  }
}

AcceleratedMesh AcceleratedMeshRecalculateVolumes(AcceleratedMesh data)
{
  std::vector<AcceleratedMesh::Node> nodesToProcesses;
  for (uint32_t i = 0; i < data.Indices.size(); i += 3)
  {
    //setup faces
    auto face = AcceleratedMesh::Face();
    face.Indices[0] = i + 0;
    face.Indices[1] = i + 1;
    face.Indices[2] = i + 2;
    face.Indices[3] = 0;
    const auto v0 = data.Positions[data.Indices[face.Indices[0]].Position];
    const auto v1 = data.Positions[data.Indices[face.Indices[1]].Position];
    const auto v2 = data.Positions[data.Indices[face.Indices[2]].Position];
    face.Center = glm::vec4((v0 + v1 + v2) / 3.0f, 1.0f);
    data.Faces.push_back(face);

    //setup nodes
    auto node = AcceleratedMesh::Node();
    node.Index = data.Faces.size() - 1;
    node.Center = face.Center;
    node.Radius = std::numeric_limits<uint32_t>::max();
    for (const auto p : face.Indices)
    {
      const auto d = glm::distance(face.Center, glm::vec4(data.Positions[data.Indices[p].Position], 1.0f));
      if (node.Radius > d)
        node.Radius = d;
    }
    nodesToProcesses.push_back(node);
  }

  while (nodesToProcesses.size() > 1)
  {
    uint32_t left, right;
    FindMinNodes(nodesToProcesses, left, right);
    data.Nodes.push_back(nodesToProcesses[left]);
    data.Nodes.push_back(nodesToProcesses[right]);

    //create new node to combine the 2 nodes
    auto node = AcceleratedMesh::Node();
    node.Center = (nodesToProcesses[left].Center + nodesToProcesses[right].Center) / 2.0f;
    const auto dist = glm::distance(nodesToProcesses[left].Center, nodesToProcesses[right].Center);
    node.Radius = dist + glm::max(glm::distance(nodesToProcesses[left].Center, node.Center), glm::distance(nodesToProcesses[right].Center, node.Center));
    node.Index = -1;
    node.Left = data.Nodes.size() - 2;
    node.Right = data.Nodes.size() - 1;
    const auto leftItr = nodesToProcesses.begin() + left;
    const auto rightItr = nodesToProcesses.begin() + right;
    if (left > right)
    {
      nodesToProcesses.erase(leftItr);
      nodesToProcesses.erase(rightItr);
    }
    else
    {
      nodesToProcesses.erase(rightItr);
      nodesToProcesses.erase(leftItr);
    }
    nodesToProcesses.push_back(node);
  }
  data.Nodes.push_back(nodesToProcesses[0]);
  data.MeshDataByteSize = 0;
  data.MeshDataByteSize += sizeof(glm::vec3) * data.Positions.size();
  data.MeshDataByteSize += sizeof(glm::vec2) * data.Textures.size();
  data.MeshDataByteSize += sizeof(glm::vec3) * data.Normals.size();
  return data;
}
} // namespace Graphics
} // namespace Tortuga