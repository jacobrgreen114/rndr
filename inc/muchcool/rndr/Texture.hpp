// Copyright (c) 2022. Jacob R. Green
// All Rights Reserved.

#pragma once

#include "Buffer.hpp"

namespace rndr {

class Texture : public GraphicsObject {
  vk::Image _image;
  vk::ImageView _view;
  vk::Sampler _sampler;

  VmaAllocation _allocation;
  VmaAllocationInfo _allocationInfo;

public:
  Texture(GraphicsContext* context, uint32 width, uint32 height,
          vk::Format format, uintn buffer_size, void* buffer,
          vk::Filter filtering = vk::Filter::eLinear,
          vk::SamplerAddressMode address_mode = vk::SamplerAddressMode::eRepeat,
          float anisotropy = 0.0f);

  ~Texture() override;

  inline auto GetImage() const { return _image; }
  inline auto GetView() const { return _view; }
  inline auto GetSampler() const { return _sampler; }

private:
  void ImageCopy(vk::CommandBuffer commandBuffer);
};

} // namespace rndr