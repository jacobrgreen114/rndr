// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "muchcool/rndr/texture.hpp"

#include "muchcool/rndr/command_pool.hpp"

namespace muchcool::rndr {

Texture::Texture(Shared<GraphicsContext> context_, uint32 width, uint32 height,
                 vk::Format format, uword buffer_size, void* buffer,
                 vk::Filter filtering, vk::SamplerAddressMode address_mode,
                 float anisotropy)
    : GraphicsObject(std::move(context_)) {
  auto& device = context()->device();
  auto& allocator = context()->allocator();

  auto image_create_info = vk::ImageCreateInfo(
      {}, vk::ImageType::e2D, format, {width, height, 1}, 1, 1,
      vk::SampleCountFlagBits::e1, vk::ImageTiling::eOptimal,
      vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
      vk::SharingMode::eExclusive, {}, vk::ImageLayout::eUndefined);

  auto allocation_create_info =
      VmaAllocationCreateInfo{.usage = VmaMemoryUsage::VMA_MEMORY_USAGE_AUTO};

  if (auto result = vmaCreateImage(
          allocator, &static_cast<const VkImageCreateInfo&>(image_create_info),
          &allocation_create_info, &reinterpret_cast<VkImage&>(_image),
          &_allocation, &_allocationInfo);
      result != VK_SUCCESS) {
    throw;
  }

  auto staging_buffer_create_info = vk::BufferCreateInfo(
      {}, _allocationInfo.size, vk::BufferUsageFlagBits::eTransferSrc);
  auto staging_alloc_create_info = VmaAllocationCreateInfo{
      .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
      .usage = VmaMemoryUsage::VMA_MEMORY_USAGE_AUTO};

  auto staging_buffer = VkBuffer{};
  auto staging_allocation = VmaAllocation{};
  auto staging_alloc_info = VmaAllocationInfo{};

  if (auto result = vmaCreateBuffer(
          allocator,
          &static_cast<const VkBufferCreateInfo&>(staging_buffer_create_info),
          &staging_alloc_create_info, &staging_buffer, &staging_allocation,
          &staging_alloc_info);
      result != VK_SUCCESS) {
    throw;
  }

  void* mapped_addr = nullptr;
  if (auto result = vmaMapMemory(allocator, staging_allocation, &mapped_addr);
      result != VK_SUCCESS) {
    throw;
  }

  auto copy_error =
      memcpy_s(mapped_addr, staging_alloc_info.size, buffer, buffer_size);

  vmaUnmapMemory(allocator, staging_allocation);
  if (copy_error) throw std::exception();

  auto command_pool = new CommandPool(context());
  auto command_buffer = command_pool->AllocateBuffer();

  auto begin_info = vk::CommandBufferBeginInfo();
  command_buffer->operator vk::CommandBuffer().begin(begin_info);

  auto image_memory_barrier = vk::ImageMemoryBarrier(
      vk::AccessFlagBits::eNone, vk::AccessFlagBits::eShaderRead,
      vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal,
      VK_QUEUE_FAMILY_IGNORED, VK_QUEUE_FAMILY_IGNORED, _image,
      {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1});

  command_buffer->operator vk::CommandBuffer().pipelineBarrier(
      vk::PipelineStageFlagBits::eTopOfPipe,
      vk::PipelineStageFlagBits::eFragmentShader, {}, {}, {},
      image_memory_barrier);

  auto copies = std::array<vk::BufferImageCopy, 1>{vk::BufferImageCopy(
      0, 0, 0,
      vk::ImageSubresourceLayers(vk::ImageAspectFlagBits::eColor, 0, 0, 1),
      {0, 0, 0}, {width, height, 1})};

  command_buffer->operator vk::CommandBuffer().copyBufferToImage(
      staging_buffer, _image, vk::ImageLayout::eTransferDstOptimal, copies);

  image_memory_barrier = vk::ImageMemoryBarrier(
      vk::AccessFlagBits::eNone, vk::AccessFlagBits::eShaderRead,
      vk::ImageLayout::eTransferDstOptimal,
      vk::ImageLayout::eShaderReadOnlyOptimal, VK_QUEUE_FAMILY_IGNORED,
      VK_QUEUE_FAMILY_IGNORED, _image,
      {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1});

  command_buffer->operator vk::CommandBuffer().pipelineBarrier(
      vk::PipelineStageFlagBits::eTopOfPipe,
      vk::PipelineStageFlagBits::eFragmentShader, {}, {}, {},
      image_memory_barrier);

  command_buffer->operator vk::CommandBuffer().end();

  auto& queue = context()->queue();

  auto command_buffers = std::array<vk::CommandBuffer, 1>{*command_buffer};

  auto submit_info = vk::SubmitInfo({}, {}, command_buffers);
  queue.submit(submit_info);
  queue.waitIdle();

  auto view_create_info = vk::ImageViewCreateInfo(
      {}, _image, vk::ImageViewType::e2D, format,
      vk::ComponentMapping(vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG,
                           vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA),
      vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));

  _view = device.createImageView(view_create_info);

  auto sampler_create_info = vk::SamplerCreateInfo(
      {}, filtering, filtering, vk::SamplerMipmapMode::eNearest, address_mode,
      address_mode, address_mode, {}, anisotropy > 0.0f, anisotropy);

  _sampler = device.createSampler(sampler_create_info);

  vmaDestroyBuffer(allocator, staging_buffer, staging_allocation);
}

Texture::~Texture() {
  auto& device = context()->device();
  device.destroy(_sampler);
  device.destroy(_view);
  device.destroy(_image);
}

}  // namespace muchcool::rndr
