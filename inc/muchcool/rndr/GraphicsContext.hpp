
// Copyright (c) 2022. Jacob R. Green
// All Rights Reserved.

#pragma once

#include "Platform.hpp"

namespace rndr {

using InstantExecuteCallback = Delegate<void, vk::CommandBuffer>;

class GraphicsContext final : public virtual Object {
  vk::Instance _instance;
  vk::PhysicalDevice _physical_device;
  vk::PhysicalDeviceProperties _physical_device_properties;
  vk::PhysicalDeviceFeatures _physical_device_features;
  vk::PhysicalDeviceMemoryProperties _memory_properties;

  uintn _queue_family_index;
  vk::Device _device;
  vk::Queue _queue;

  VmaAllocator _allocator;

public:
  GraphicsContext();
  ~GraphicsContext() noexcept override;
  static Pointer<GraphicsContext> New();

  const vk::Instance& GetInstance() const { return _instance; }
  const vk::PhysicalDevice& GetPhysicalDevice() const {
    return _physical_device;
  }

  uintn GetQueueFamilyIndex() const { return _queue_family_index; }

  const vk::Device& GetDevice() const { return _device; }
  const vk::Queue& GetQueue() const { return _queue; }

  const VmaAllocator& GetAllocator() const { return _allocator; }

  void InstantExecute(InstantExecuteCallback& callback);
};

class GraphicsObject : public virtual Object {
private:
  Pointer<GraphicsContext> _graphicsContext;

public:
  GraphicsObject(GraphicsContext* graphics_context);
  ~GraphicsObject() override = default;

  const Pointer<GraphicsContext>& GetGraphicsContext() const {
    return _graphicsContext;
  }
};

} // namespace rndr
