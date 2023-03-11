
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#include "common.hpp"

#include "mesh.hpp"

namespace muchcool::rndr {

class Renderable : public GraphicsObject {
  Shared<BaseMesh> _mesh;

 public:
  Renderable(Shared<GraphicsContext>&& context, BaseMesh* mesh);
};

}  // namespace muchcool::rndr