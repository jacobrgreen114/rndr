// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#include "buffer.hpp"

namespace muchcool::rndr {

class Texture : public GraphicsObject {
  vk::Image _image;
  vk::ImageView _view;
  vk::Sampler _sampler;

  VmaAllocation _allocation;
  VmaAllocationInfo _allocationInfo;

 public:
  Texture(Shared<GraphicsContext> context, uint32 width, uint32 height,
          vk::Format format, uword buffer_size, void* buffer,
          vk::Filter filtering = vk::Filter::eLinear,
          vk::SamplerAddressMode address_mode = vk::SamplerAddressMode::eRepeat,
          float anisotropy = 0.0f);

  ~Texture() override;

  auto GetImage() const { return _image; }
  auto GetView() const { return _view; }
  auto GetSampler() const { return _sampler; }

 private:
  void ImageCopy(vk::CommandBuffer commandBuffer);
};

}  // namespace muchcool::rndr