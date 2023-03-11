
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#include "render_surface.hpp"

namespace muchcool::rndr {

class Scene : public virtual Object {};

class Renderer : public virtual Object {
 public:
  Renderer();

  void Render(RenderSurface* surface, Scene* scene);
};

}  // namespace muchcool::rndr