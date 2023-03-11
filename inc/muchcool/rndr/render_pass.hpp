
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#include "graphics_context.hpp"

namespace muchcool::rndr {

class RenderPass final : public GraphicsObject {
 private:
  vk::RenderPass _renderPass;

 public:
  RenderPass(Shared<GraphicsContext> context, vk::Format surfaceFormat);
  ~RenderPass();

  operator const vk::RenderPass&() const { return _renderPass; }
};

}  // namespace muchcool::rndr