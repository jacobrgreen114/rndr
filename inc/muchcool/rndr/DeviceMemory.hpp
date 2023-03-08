
// Copyright (c) 2022. Jacob R. Green
// All Rights Reserved.

#pragma once

#include "GraphicsContext.hpp"

namespace Rndr {

enum class MemoryType { Device, Staging };

class DeviceMemory final : public GraphicsObject {
  vk::DeviceMemory _memory;

  uintn _size;
  bool _isMapped = false;
  void* _mappedAddress = null;

public:
  DeviceMemory(GraphicsContext* context, uintn size, MemoryType type);
  ~DeviceMemory() override;

  inline uintn GetSize() const { return _size; }
  inline bool IsMapped() const { return _isMapped; }
  inline void* GetMappedAddr() const;

  inline operator const vk::DeviceMemory&() const { return _memory; }

  void* Map();
  void UnMap();
};

} // namespace Rndr