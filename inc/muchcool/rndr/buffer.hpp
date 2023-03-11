
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#include "graphics_context.hpp"

namespace muchcool::rndr {

using BufferUsage = vk::BufferUsageFlagBits;

class Buffer : public GraphicsObject {
  uword _size;
  vk::Buffer _buffer;

 protected:
  VmaAllocation _allocation;
  VmaAllocationInfo _allocationInfo;

 public:
  Buffer(Shared<GraphicsContext> context, BufferUsage bufferUsage, uword size);
  ~Buffer() override;

  auto GetSize() const { return _size; }
  operator vk::Buffer() const { return _buffer; }

  template <typename T>
  void CopyFrom(ArrayProxy<T> array) {
    if (sizeof(T) * array.size() > GetSize())
      throw std::exception("Buffer is not large enough");

    auto& allocator = context()->allocator();

    void* dest = nullptr;
    if (auto result = vmaMapMemory(allocator, _allocation, &dest);
        result != VK_SUCCESS) {
      throw;
    }

    auto errorCode =
        memcpy_s(dest, _size, array.data(), sizeof(T) * array.size());
    if (errorCode) throw std::exception("Failed to copy array.");

    vmaUnmapMemory(allocator, _allocation);
  }
};

class ArrayBuffer : public Buffer {
 public:
  ArrayBuffer(Shared<GraphicsContext> context, BufferUsage bufferUsage,
              uword elementSize, uword elementCount);
};

class VertexBuffer : public Buffer {
 public:
  VertexBuffer(Shared<GraphicsContext> context, uword size);
};

class IndexBuffer : public Buffer {
 public:
  IndexBuffer(Shared<GraphicsContext> context, uword size);
};

template <typename T>
class UniformBuffer : public Buffer {
 public:
  UniformBuffer(Shared<GraphicsContext> context)
      : Buffer(std::move(context), vk::BufferUsageFlagBits::eUniformBuffer,
               sizeof(T)) {}

  void Update(const T& data) {
    auto& allocator = context()->allocator();

    void* dest = nullptr;
    if (auto result = vmaMapMemory(allocator, _allocation, &dest);
        result != VK_SUCCESS) {
      throw;
    }

    memcpy(dest, &data, sizeof(T));

    vmaUnmapMemory(allocator, _allocation);
  }

  UniformBuffer(Shared<GraphicsContext> context, const T& initialData)
      : UniformBuffer(std::move(context)) {
    Update(initialData);
  }
};

}  // namespace muchcool::rndr