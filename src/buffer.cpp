
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "muchcool/rndr/buffer.hpp"

namespace muchcool::rndr {

Buffer::Buffer(Shared<GraphicsContext> context_, BufferUsage bufferUsage,
               uword size)
    : GraphicsObject(std::move(context_)), _size(size) {
  auto& device = context()->device();

  auto createInfo = vk::BufferCreateInfo({}, size, bufferUsage);

  auto allocCreateInfo = VmaAllocationCreateInfo{
      .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
               VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT,
      .usage = VMA_MEMORY_USAGE_AUTO};

  if (auto result = vmaCreateBuffer(
          context()->allocator(),
          &static_cast<const VkBufferCreateInfo&>(createInfo), &allocCreateInfo,
          &reinterpret_cast<VkBuffer&>(_buffer), &_allocation,
          &_allocationInfo);
      result != VK_SUCCESS) {
    throw;
  }
}

Buffer::~Buffer() {
  auto& device = context()->device();
  device.destroy(_buffer);
  vmaFreeMemory(context()->allocator(), _allocation);
}

/*
 * ArrayBuffer
 */

ArrayBuffer::ArrayBuffer(Shared<GraphicsContext> context,
                         BufferUsage bufferUsage, uword elementSize,
                         uword elementCount)
    : Buffer(std::move(context), bufferUsage, elementSize * elementCount) {}

/*
 * Vertex Buffer
 */

VertexBuffer::VertexBuffer(Shared<GraphicsContext> context, uword size)
    : Buffer(std::move(context), vk::BufferUsageFlagBits::eVertexBuffer, size) {
}

IndexBuffer::IndexBuffer(Shared<GraphicsContext> context, uword size)
    : Buffer(std::move(context), vk::BufferUsageFlagBits::eIndexBuffer, size) {}

}  // namespace muchcool::rndr
