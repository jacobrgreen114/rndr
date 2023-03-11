
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#include "graphics_context.hpp"

namespace muchcool::rndr {

using CommandBufferLevel = vk::CommandBufferLevel;

class CommandBuffer;

class CommandPool : public GraphicsObject {
  vk::CommandPool _commandPool;

 public:
  CommandPool(Shared<GraphicsContext> context);
  ~CommandPool() override;

  operator vk::CommandPool() const { return _commandPool; }

  Shared<CommandBuffer> AllocateBuffer(
      CommandBufferLevel level = CommandBufferLevel::ePrimary);
  std::vector<vk::CommandBuffer> AllocateBuffers(
      uword count, CommandBufferLevel level = CommandBufferLevel::ePrimary);
};

class CommandBuffer : public virtual Object {
  Shared<CommandPool> _commandPool;
  vk::CommandBuffer _commandBuffer;

 public:
  CommandBuffer(Shared<CommandPool> pool, vk::CommandBuffer buffer);

  auto& GetCommandPool() const { return _commandPool; }
  operator vk::CommandBuffer() const { return _commandBuffer; }
};

}  // namespace muchcool::rndr