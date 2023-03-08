
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "muchcool/rndr/Renderable.hpp"

namespace rndr {

Renderable::Renderable(GraphicsContext *context, BaseMesh *mesh)
    : GraphicsObject(context), _mesh(mesh) {}

} // namespace rndr
