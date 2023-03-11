
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#include "common.hpp"

namespace muchcool::rndr {

using InstantExecuteCallback = Delegate<void, vk::CommandBuffer>;

class GraphicsContext final : public virtual Object {
  vk::Instance _instance;
  vk::PhysicalDevice _physical_device;
  vk::PhysicalDeviceProperties _physical_device_properties;
  vk::PhysicalDeviceFeatures _physical_device_features;
  vk::PhysicalDeviceMemoryProperties _memory_properties;

  uword _queue_family_index;
  vk::Device _device;
  vk::Queue _queue;

  VmaAllocator _allocator;

 public:
  GraphicsContext();
  ~GraphicsContext() noexcept override;
  static Shared<GraphicsContext> create();

  auto& instance() const { return _instance; }
  auto& physical_device() const { return _physical_device; }

  uword queue_family_index() const { return _queue_family_index; }
  auto& device() const { return _device; }
  auto& queue() const { return _queue; }

  auto& allocator() const { return _allocator; }

  void InstantExecute(InstantExecuteCallback& callback);
};

class GraphicsObject : public Object {
 private:
  Shared<GraphicsContext> _context;

 public:
  constexpr explicit GraphicsObject(Shared<GraphicsContext> context)
      : _context{std::move(context)} {
    assert(_context);
  }

  ~GraphicsObject() override = default;

  auto& context() const { return _context; }
};

}  // namespace muchcool::rndr
