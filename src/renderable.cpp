
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "muchcool/rndr/renderable.hpp"

namespace muchcool::rndr {

Renderable::Renderable(Shared<GraphicsContext>&& context, BaseMesh* mesh)
    : GraphicsObject(std::forward<Shared<GraphicsContext>>(context)),
      _mesh(mesh) {}

}  // namespace muchcool::rndr
