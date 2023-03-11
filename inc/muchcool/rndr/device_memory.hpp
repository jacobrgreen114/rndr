
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#include "graphics_context.hpp"

namespace muchcool::rndr {

enum class MemoryType { Device, Staging };

class DeviceMemory final : public GraphicsObject {
  vk::DeviceMemory _memory;

  uintn _size;
  bool _isMapped = false;
  void* _mappedAddress = null;

 public:
  DeviceMemory(Shared<GraphicsContext> context, uintn size, MemoryType type);
  ~DeviceMemory() override;

  uintn GetSize() const { return _size; }
  bool IsMapped() const { return _isMapped; }
  void* GetMappedAddr() const;

  operator const vk::DeviceMemory&() const { return _memory; }

  void* Map();
  void UnMap();
};

}  // namespace muchcool::rndr