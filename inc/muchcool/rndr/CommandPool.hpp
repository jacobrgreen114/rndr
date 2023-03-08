
#pragma once

#include "GraphicsContext.hpp"

namespace rndr {

using CommandBufferLevel = vk::CommandBufferLevel;

class CommandBuffer;

class CommandPool : public GraphicsObject {
  vk::CommandPool _commandPool;

public:
  CommandPool(GraphicsContext* context);
  ~CommandPool() override;

  inline operator vk::CommandPool() const { return _commandPool; }

  Pointer<CommandBuffer>
  AllocateBuffer(CommandBufferLevel level = CommandBufferLevel::ePrimary);
  std::vector<vk::CommandBuffer>
  AllocateBuffers(uintn count,
                  CommandBufferLevel level = CommandBufferLevel::ePrimary);
};

class CommandBuffer : public virtual Object {
  Pointer<CommandPool> _commandPool;
  vk::CommandBuffer _commandBuffer;

public:
  CommandBuffer(CommandPool* pool, vk::CommandBuffer buffer);

  inline auto& GetCommandPool() const { return _commandPool; }
  inline operator vk::CommandBuffer() const { return _commandBuffer; }
};

} // namespace Rndr