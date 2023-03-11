
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "muchcool/rndr/descriptor_set.hpp"

namespace muchcool::rndr {

/*
 * DescriptorSetLayout
 */

DescriptorSetLayout::DescriptorSetLayout(
    Shared<GraphicsContext> context_,
    ArrayProxy<const DecriptorSetLayoutBinding> bindings)
    : GraphicsObject(std::move(context_)) {
  auto& device = context()->device();
  auto createInfo = vk::DescriptorSetLayoutCreateInfo({}, bindings);
  _descriptor_set_layout = device.createDescriptorSetLayout(createInfo);
}

/*
 * DescriptorPool
 */

DescriptorPool::DescriptorPool(Shared<GraphicsContext> context_, uword maxSets,
                               ArrayProxy<const DescriptorPoolSize> poolSizes)
    : GraphicsObject(std::move(context_)) {
  auto& device = context()->device();

  auto createInfo = vk::DescriptorPoolCreateInfo(
      vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, maxSets, poolSizes);
  _descriptor_pool = device.createDescriptorPool(createInfo);
}

DescriptorPool::~DescriptorPool() {}

Shared<DescriptorSet> DescriptorPool::allocate(
    const DescriptorSetLayout& layout) {
  auto& device = context()->device();

  auto layouts = std::array<vk::DescriptorSetLayout, 1>{layout};
  auto allocInfo = vk::DescriptorSetAllocateInfo(_descriptor_pool, layouts);
  auto descriptorSets = device.allocateDescriptorSets(allocInfo);

  return Shared{new DescriptorSet(Shared{this}, descriptorSets[0])};
}

DescriptorSet::DescriptorSet(Shared<DescriptorPool> pool, vk::DescriptorSet set)
    : _descriptor_pool(std::move(pool)), _descriptor_set(set) {}

DescriptorSet::~DescriptorSet() {
  auto& device = _descriptor_pool->context()->device();
  device.freeDescriptorSets(*_descriptor_pool, _descriptor_set);
}

void DescriptorSet::update_uniform(uword binding, Buffer& buffer) {
  auto& device = _descriptor_pool->context()->device();

  auto bufferInfo = vk::DescriptorBufferInfo(buffer, 0, buffer.GetSize());

  auto writes = std::array<vk::WriteDescriptorSet, 1>{vk::WriteDescriptorSet(
      _descriptor_set, binding, 0, 1, vk::DescriptorType::eUniformBuffer, {},
      &bufferInfo)};

  device.updateDescriptorSets(writes, {});
}

void DescriptorSet::update_sampler(uword binding, Texture& texture) {
  auto& device = _descriptor_pool->context()->device();

  auto imageInfo =
      vk::DescriptorImageInfo(texture.GetSampler(), texture.GetView(),
                              vk::ImageLayout::eShaderReadOnlyOptimal);

  auto writes = std::array<vk::WriteDescriptorSet, 1>{vk::WriteDescriptorSet(
      _descriptor_set, binding, 0, 1, vk::DescriptorType::eCombinedImageSampler,
      &imageInfo)};

  device.updateDescriptorSets(writes, {});
}

}  // namespace muchcool::rndr