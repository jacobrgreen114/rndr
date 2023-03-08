
#pragma once

#include "GraphicsContext.hpp"

namespace rndr {

class RenderPass final : public GraphicsObject {
private:
  vk::RenderPass _renderPass;

public:
  RenderPass(GraphicsContext* context, vk::Format surfaceFormat);
  ~RenderPass();

  inline operator const vk::RenderPass&() const { return _renderPass; }
};

} // namespace Rndr