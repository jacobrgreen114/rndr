
// Copyright (c) 2022-2023 Jacob R. Green
// All Rights Reserved.

#include "muchcool/rndr/DescriptorSet.hpp"

namespace rndr {

/*
 * DescriptorSetLayout
 */

DescriptorSetLayout::DescriptorSetLayout(
    GraphicsContext *context,
    ArrayProxy<const DecriptorSetLayoutBinding> bindings)
    : GraphicsObject(context) {
  auto &device = GetGraphicsContext()->GetDevice();
  auto createInfo = vk::DescriptorSetLayoutCreateInfo({}, bindings);
  _descriptorSetLayout = device.createDescriptorSetLayout(createInfo);
}

/*
 * DescriptorPool
 */

DescriptorPool::DescriptorPool(GraphicsContext *context, uintn maxSets,
                               ArrayProxy<const DescriptorPoolSize> poolSizes)
    : GraphicsObject(context) {
  auto &device = GetGraphicsContext()->GetDevice();

  auto createInfo = vk::DescriptorPoolCreateInfo(
      vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, maxSets, poolSizes);
  _descriptorPool = device.createDescriptorPool(createInfo);
}

DescriptorPool::~DescriptorPool() {}

Pointer<DescriptorSet> DescriptorPool::Allocate(DescriptorSetLayout *layout) {
  auto &device = GetGraphicsContext()->GetDevice();

  auto layouts = std::array<vk::DescriptorSetLayout, 1>{*layout};
  auto allocInfo = vk::DescriptorSetAllocateInfo(_descriptorPool, layouts);
  auto descriptorSets = device.allocateDescriptorSets(allocInfo);

  return new DescriptorSet(this, descriptorSets[0]);
}

DescriptorSet::DescriptorSet(DescriptorPool *pool, vk::DescriptorSet set)
    : _descriptorPool(pool), _descriptorSet(set) {}

DescriptorSet::~DescriptorSet() {
  auto &device = _descriptorPool->GetGraphicsContext()->GetDevice();
  device.freeDescriptorSets(*_descriptorPool, _descriptorSet);
}

void DescriptorSet::UpdateUniform(uintn binding, Buffer *buffer) {
  auto &device = _descriptorPool->GetGraphicsContext()->GetDevice();

  auto bufferInfo = vk::DescriptorBufferInfo(*buffer, 0, buffer->GetSize());

  auto writes = std::array<vk::WriteDescriptorSet, 1>{vk::WriteDescriptorSet(
      _descriptorSet, binding, 0, 1, vk::DescriptorType::eUniformBuffer, {},
      &bufferInfo)};

  device.updateDescriptorSets(writes, {});
}

void DescriptorSet::UpdateSampler(uintn binding, Texture *texture) {
  auto &device = _descriptorPool->GetGraphicsContext()->GetDevice();

  auto imageInfo =
      vk::DescriptorImageInfo(texture->GetSampler(), texture->GetView(),
                              vk::ImageLayout::eShaderReadOnlyOptimal);

  auto writes = std::array<vk::WriteDescriptorSet, 1>{vk::WriteDescriptorSet(
      _descriptorSet, binding, 0, 1, vk::DescriptorType::eCombinedImageSampler,
      &imageInfo)};

  device.updateDescriptorSets(writes, {});
}

} // namespace rndr