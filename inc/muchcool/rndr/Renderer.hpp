
#pragma once

#include "RenderSurface.hpp"

namespace rndr {

class Scene : public virtual Object {};

class Renderer : public virtual Object {

public:
  Renderer();

  void Render(RenderSurface* surface, Scene* scene);
};

} // namespace Rndr