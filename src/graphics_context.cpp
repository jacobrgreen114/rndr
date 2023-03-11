
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "muchcool/rndr/graphics_context.hpp"

// TODO : Diagnose why upgrading version past 1.0 fixed imageview blending issue
#define VULKAN_API_VERSION VK_API_VERSION_1_3

namespace muchcool::rndr {

#ifdef NDEBUG
std::array<const char*, 0> instanceLayers = {};
#else
std::array<const char*, 1> instanceLayers = {"VK_LAYER_KHRONOS_validation"};
#endif

std::array<const char*, 2> instanceExtensions = {
    VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME};

std::array<const char*, 0> deviceLayers = {};

std::array<const char*, 1> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

/*
 * GraphicsContext
 */

GraphicsContext::GraphicsContext() {
  auto application_info = vk::ApplicationInfo();
  application_info.apiVersion = VULKAN_API_VERSION;

  const auto instance_create_info = vk::InstanceCreateInfo(
      {}, &application_info, instanceLayers, instanceExtensions);

  _instance = vk::createInstance(instance_create_info);
  const auto physical_devices = _instance.enumeratePhysicalDevices();
  // TODO : Implement device select logic
  _physical_device = physical_devices[0];
  _physical_device_properties = _physical_device.getProperties();
  _physical_device_features = _physical_device.getFeatures();

  _memory_properties = _physical_device.getMemoryProperties();

  // TODO : Implement queue family select logic
  _queue_family_index = 0;
  auto queue_priorities = std::array<float, 1>{1.0f};

  auto queue_create_info = std::array<vk::DeviceQueueCreateInfo, 1>{
      vk::DeviceQueueCreateInfo({}, _queue_family_index, queue_priorities)};

  const auto device_create_info =
      vk::DeviceCreateInfo({}, queue_create_info, deviceLayers,
                           deviceExtensions, &_physical_device_features);

  _device = _physical_device.createDevice(device_create_info);
  _queue = _device.getQueue(0, 0);

  const auto allocatorCreateInfo =
      VmaAllocatorCreateInfo{.physicalDevice = _physical_device,
                             .device = _device,
                             .instance = _instance,
                             .vulkanApiVersion = VULKAN_API_VERSION};

  if (const auto result = vmaCreateAllocator(&allocatorCreateInfo, &_allocator);
      result != VK_SUCCESS) {
    throw;
  }
}

GraphicsContext::~GraphicsContext() noexcept {
  vmaDestroyAllocator(_allocator);
}

Shared<GraphicsContext> GraphicsContext::create() {
  return Shared{new GraphicsContext()};
}

void GraphicsContext::InstantExecute(InstantExecuteCallback& callback) {
  const auto command_pool_create_info = vk::CommandPoolCreateInfo(
      vk::CommandPoolCreateFlagBits::eTransient, queue_family_index());

  const auto command_pool = _device.createCommandPool(command_pool_create_info);

  const auto cmd_buf_alloc_info = vk::CommandBufferAllocateInfo(
      command_pool, vk::CommandBufferLevel::ePrimary, 1);

  auto command_buffer = _device.allocateCommandBuffers(cmd_buf_alloc_info)[0];

  const auto begin_info = vk::CommandBufferBeginInfo();
  command_buffer.begin(begin_info);

  callback(command_buffer);

  command_buffer.end();

  const auto submit_info = vk::SubmitInfo({}, {}, command_buffer);
  _queue.submit(submit_info);
  _queue.waitIdle();

  _device.free(command_pool, command_buffer);
  _device.destroy(command_pool);
}

/*
 * GraphicsObject
 */

}  // namespace muchcool::rndr