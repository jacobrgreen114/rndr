
#pragma once

#include "Platform.hpp"

#include "Mesh.hpp"

namespace rndr {

class Renderable : public GraphicsObject {
  Pointer<BaseMesh> _mesh;

public:
  Renderable(GraphicsContext* context, BaseMesh* mesh);
};

} // namespace Rndr