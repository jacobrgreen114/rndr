
// Copyright (c) 2022-2023 Jacob R. Green
// All Rights Reserved.

#include "muchcool/rndr/Buffer.hpp"

namespace rndr {

Buffer::Buffer(GraphicsContext *context, BufferUsage bufferUsage, uintn size)
    : GraphicsObject(context), _size(size) {
  auto &device = GetGraphicsContext()->GetDevice();

  auto createInfo = vk::BufferCreateInfo({}, size, bufferUsage);

  auto allocCreateInfo = VmaAllocationCreateInfo{
      .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
               VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT,
      .usage = VMA_MEMORY_USAGE_AUTO};

  if (auto result = vmaCreateBuffer(
          context->GetAllocator(),
          &static_cast<const VkBufferCreateInfo &>(createInfo),
          &allocCreateInfo, &reinterpret_cast<VkBuffer &>(_buffer),
          &_allocation, &_allocationInfo);
      result != VK_SUCCESS) {
    throw;
  }
}

Buffer::~Buffer() {
  auto &context = GetGraphicsContext();
  auto &device = context->GetDevice();
  device.destroy(_buffer);
  vmaFreeMemory(context->GetAllocator(), _allocation);
}

/*
 * ArrayBuffer
 */

ArrayBuffer::ArrayBuffer(GraphicsContext *context, BufferUsage bufferUsage,
                         uintn elementSize, uintn elementCount)
    : Buffer(context, bufferUsage, elementSize * elementCount) {}

/*
 * Vertex Buffer
 */

VertexBuffer::VertexBuffer(GraphicsContext *context, uintn size)
    : Buffer(context, vk::BufferUsageFlagBits::eVertexBuffer, size) {}

IndexBuffer::IndexBuffer(GraphicsContext *context, uintn size)
    : Buffer(context, vk::BufferUsageFlagBits::eIndexBuffer, size) {}

} // namespace rndr
