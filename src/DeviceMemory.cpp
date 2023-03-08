
// Copyright (c) 2022-2023 Jacob R. Green
// All Rights Reserved.

#include "muchcool/rndr/DeviceMemory.hpp"

namespace Rndr {

DeviceMemory::DeviceMemory(GraphicsContext *context, uintn size,
                           MemoryType type)
    : GraphicsObject(context), _size(size) {
  auto &device = GetGraphicsContext()->GetDevice();

  auto memoryTypeIndex = UINT32_MAX;

  switch (type) {
  case MemoryType::Device:
    memoryTypeIndex = GetGraphicsContext()->GetAllocationMemoryType();
    break;
  case MemoryType::Staging:
    memoryTypeIndex = GetGraphicsContext()->GetStagingMemoryType();
    break;
  }

  auto allocInfo = vk::MemoryAllocateInfo(size, memoryTypeIndex);
  _memory = device.allocateMemory(allocInfo);
}

void *DeviceMemory::Map() {
  if (_isMapped)
    throw std::exception("Buffer already mapped.");
  auto &device = GetGraphicsContext()->GetDevice();

  auto hostAddr = device.mapMemory(_memory, 0, GetSize());

  _isMapped = true;
  return _mappedAddress = hostAddr;
}

void DeviceMemory::UnMap() {
  auto &device = GetGraphicsContext()->GetDevice();
  device.unmapMemory(_memory);
  _isMapped = false;
  _mappedAddress = null;
}

void *DeviceMemory::GetMappedAddr() const {
  if (!IsMapped())
    throw std::exception("Buffer is not mapped.");

  return _mappedAddress;
}

DeviceMemory::~DeviceMemory() {
  auto &device = GetGraphicsContext()->GetDevice();
  device.free(_memory);
}

} // namespace Rndr