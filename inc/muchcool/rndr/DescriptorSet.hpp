
// Copyright (c) 2022. Jacob R. Green
// All Rights Reserved.

#pragma once

#include "Buffer.hpp"
#include "Texture.hpp"

namespace rndr {

class DescriptorSet;

using DecriptorSetLayoutBinding = vk::DescriptorSetLayoutBinding;
using DescriptorPoolSize = vk::DescriptorPoolSize;

class DescriptorSetLayout : public GraphicsObject {
  vk::DescriptorSetLayout _descriptorSetLayout;

public:
  DescriptorSetLayout(GraphicsContext* context,
                      ArrayProxy<const DecriptorSetLayoutBinding> bindings);

  inline operator vk::DescriptorSetLayout() const {
    return _descriptorSetLayout;
  }
};

class DescriptorPool : public GraphicsObject {
  vk::DescriptorPool _descriptorPool;

public:
  DescriptorPool(GraphicsContext* context, uintn maxSets,
                 ArrayProxy<const DescriptorPoolSize> poolSizes);
  ~DescriptorPool() override;

  inline operator vk::DescriptorPool() const { return _descriptorPool; }

  Pointer<DescriptorSet> Allocate(DescriptorSetLayout* layout);
};

class DescriptorSet : public virtual Object {
  Pointer<DescriptorPool> _descriptorPool;
  vk::DescriptorSet _descriptorSet;

public:
  DescriptorSet(DescriptorPool* pool, vk::DescriptorSet set);
  ~DescriptorSet() override;

  inline operator vk::DescriptorSet() const { return _descriptorSet; }

  void UpdateUniform(uintn binding, Buffer* buffer);
  void UpdateSampler(uintn binding, Texture* texture);
};

} // namespace Rndr