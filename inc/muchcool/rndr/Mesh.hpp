
#pragma once

#include "Buffer.hpp"
#include "Platform.hpp"

namespace rndr {

using Index16 = uint16;
using Index32 = uint32;

class Vertex {};

class Vertex2D : public Vertex {
public:
  glm::vec2 Pos;
};

class Vertex2DTextured : public Vertex2D {
public:
  glm::vec2 UV;
};

class BaseMesh : public GraphicsObject {
public:
  BaseMesh(uintn vertexSize, uintn vertexCount, void* vertexData,
           uintn indexSize, uintn indexCount, void* indexData);

  template <typename Vtx, typename Idx>
  BaseMesh(ArrayProxy<Vtx> vertices, ArrayProxy<Vtx> indices)
      : BaseMesh(sizeof(Vtx), vertices.size(), vertices.data(), sizeof(Idx),
                 indices.size(), indices.data()) {}
};

template <typename Vtx, typename Idx> class Mesh : public GraphicsObject {
  Pointer<Buffer> _vertexBuffer;
  Pointer<Buffer> _indexBuffer;

public:
  Mesh(GraphicsContext* context, ArrayProxy<Vtx> vertices,
       ArrayProxy<Idx> indices)
      : GraphicsObject(context) {
    _vertexBuffer = new VertexBuffer(context, sizeof(Vtx) * vertices.size());
    _indexBuffer = new IndexBuffer(context, sizeof(Idx) * indices.size());

    UpdateVertices(vertices);
    UpdateIndices(indices);
  }

  void UpdateVertices(ArrayProxy<Vtx> vertices) {
    _vertexBuffer->template CopyFrom(vertices);
  }

  void UpdateIndices(ArrayProxy<Idx> indices) {
    _indexBuffer->template CopyFrom(indices);
  }
};

class Mesh2D : public Mesh<Vertex2D, Index16> {};

class TexturedMesh2D : public Mesh<Vertex2DTextured, Index16> {};

} // namespace Rndr