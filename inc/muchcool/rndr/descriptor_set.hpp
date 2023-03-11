
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#include "buffer.hpp"
#include "texture.hpp"

namespace muchcool::rndr {

class DescriptorSet;

using DecriptorSetLayoutBinding = vk::DescriptorSetLayoutBinding;
using DescriptorPoolSize = vk::DescriptorPoolSize;

class DescriptorSetLayout : public GraphicsObject {
  vk::DescriptorSetLayout _descriptor_set_layout;

 public:
  DescriptorSetLayout(Shared<GraphicsContext> context,
                      ArrayProxy<const DecriptorSetLayoutBinding> bindings);

  operator vk::DescriptorSetLayout() const { return _descriptor_set_layout; }
};

class DescriptorPool : public GraphicsObject {
  vk::DescriptorPool _descriptor_pool;

 public:
  DescriptorPool(Shared<GraphicsContext> context, uword maxSets,
                 ArrayProxy<const DescriptorPoolSize> poolSizes);
  ~DescriptorPool() override;

  operator vk::DescriptorPool() const { return _descriptor_pool; }

  Shared<DescriptorSet> allocate(const DescriptorSetLayout& layout);
};

class DescriptorSet : public virtual Object {
  Shared<DescriptorPool> _descriptor_pool;
  vk::DescriptorSet _descriptor_set;

 public:
  DescriptorSet(Shared<DescriptorPool>, vk::DescriptorSet set);
  ~DescriptorSet() override;

  operator vk::DescriptorSet() const { return _descriptor_set; }

  void update_uniform(uword binding, Buffer& buffer);
  void update_sampler(uword binding, Texture& texture);
};

}  // namespace muchcool::rndr