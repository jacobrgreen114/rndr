
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "muchcool/rndr/CommandPool.hpp"

namespace rndr {

CommandPool::CommandPool(GraphicsContext *context) : GraphicsObject(context) {
  auto &device = GetGraphicsContext()->GetDevice();

  auto createInfo = vk::CommandPoolCreateInfo(
      vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
      context->GetQueueFamilyIndex());
  _commandPool = device.createCommandPool(createInfo);
}

CommandPool::~CommandPool() {
  auto &device = GetGraphicsContext()->GetDevice();
  device.destroy(_commandPool);
}

Pointer<CommandBuffer> CommandPool::AllocateBuffer(CommandBufferLevel level) {
  auto &device = GetGraphicsContext()->GetDevice();

  auto allocInfo = vk::CommandBufferAllocateInfo(_commandPool, level, 1);
  auto commandBuffers = device.allocateCommandBuffers(allocInfo);
  return new CommandBuffer(this, commandBuffers[0]);
}

std::vector<vk::CommandBuffer>
CommandPool::AllocateBuffers(uintn count, CommandBufferLevel level) {
  auto &device = GetGraphicsContext()->GetDevice();

  auto allocInfo = vk::CommandBufferAllocateInfo(_commandPool, level, count);
  auto commandBuffers = device.allocateCommandBuffers(allocInfo);
  return commandBuffers;
}

CommandBuffer::CommandBuffer(CommandPool *pool, vk::CommandBuffer buffer)
    : _commandPool(pool), _commandBuffer(buffer) {}

} // namespace rndr
