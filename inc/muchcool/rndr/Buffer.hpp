
// Copyright (c) 2022. Jacob R. Green
// All Rights Reserved.

#pragma once

#include "GraphicsContext.hpp"

namespace rndr {

using BufferUsage = vk::BufferUsageFlagBits;

class Buffer : public GraphicsObject {
  uintn _size;
  vk::Buffer _buffer;

protected:
  VmaAllocation _allocation;
  VmaAllocationInfo _allocationInfo;

public:
  Buffer(GraphicsContext* context, BufferUsage bufferUsage, uintn size);
  ~Buffer() override;

  inline auto GetSize() const { return _size; }
  inline operator vk::Buffer() const { return _buffer; }

  template <typename T> void CopyFrom(ArrayProxy<T> array) {
    if (sizeof(T) * array.size() > GetSize())
      throw std::exception("Buffer is not large enough");

    auto& allocator = GetGraphicsContext()->GetAllocator();

    void* dest = nullptr;
    if (auto result = vmaMapMemory(allocator, _allocation, &dest);
        result != VK_SUCCESS) {
      throw;
    }

    auto errorCode =
        memcpy_s(dest, _size, array.data(), sizeof(T) * array.size());
    if (errorCode)
      throw std::exception("Failed to copy array.");

    vmaUnmapMemory(allocator, _allocation);
  }
};

class ArrayBuffer : public Buffer {

public:
  ArrayBuffer(GraphicsContext* context, BufferUsage bufferUsage,
              uintn elementSize, uintn elementCount);
};

class VertexBuffer : public Buffer {
public:
  VertexBuffer(GraphicsContext* context, uintn size);
};

class IndexBuffer : public Buffer {
public:
  IndexBuffer(GraphicsContext* context, uintn size);
};

template <typename T> class UniformBuffer : public Buffer {
public:
  UniformBuffer(GraphicsContext* context)
      : Buffer(context, vk::BufferUsageFlagBits::eUniformBuffer, sizeof(T)) {}

  void Update(const T& data) {
    auto& allocator = GetGraphicsContext()->GetAllocator();

    void* dest = nullptr;
    if (auto result = vmaMapMemory(allocator, _allocation, &dest);
        result != VK_SUCCESS) {
      throw;
    }

    memcpy(dest, &data, sizeof(T));

    vmaUnmapMemory(allocator, _allocation);
  }

  UniformBuffer(GraphicsContext* context, const T& initialData)
      : UniformBuffer(context) {
    Update(initialData);
  }
};

} // namespace rndr