
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "muchcool/rndr/command_pool.hpp"

namespace muchcool::rndr {

CommandPool::CommandPool(Shared<GraphicsContext> context_)
    : GraphicsObject(std::move(context_)) {
  auto& device = context()->device();

  auto createInfo = vk::CommandPoolCreateInfo(
      vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
      context()->queue_family_index());
  _commandPool = device.createCommandPool(createInfo);
}

CommandPool::~CommandPool() {
  auto& device = context()->device();
  device.destroy(_commandPool);
}

Shared<CommandBuffer> CommandPool::AllocateBuffer(CommandBufferLevel level) {
  auto& device = context()->device();

  auto allocInfo = vk::CommandBufferAllocateInfo(_commandPool, level, 1);
  auto commandBuffers = device.allocateCommandBuffers(allocInfo);
  return Shared{new CommandBuffer(Shared{this}, commandBuffers[0])};
}

std::vector<vk::CommandBuffer> CommandPool::AllocateBuffers(
    uword count, CommandBufferLevel level) {
  auto& device = context()->device();

  auto allocInfo = vk::CommandBufferAllocateInfo(_commandPool, level, count);
  auto commandBuffers = device.allocateCommandBuffers(allocInfo);
  return commandBuffers;
}

CommandBuffer::CommandBuffer(Shared<CommandPool> pool, vk::CommandBuffer buffer)
    : _commandPool(std::move(pool)), _commandBuffer(buffer) {}

}  // namespace muchcool::rndr
