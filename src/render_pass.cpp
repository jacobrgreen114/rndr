
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "muchcool/rndr/render_pass.hpp"

namespace muchcool::rndr {

RenderPass::RenderPass(Shared<GraphicsContext> context_,
                       vk::Format surfaceFormat)
    : GraphicsObject(std::move(context_)) {
  auto& device = context()->device();

  auto attachmentDescriptions =
      std::array<vk::AttachmentDescription, 1>{vk::AttachmentDescription(
          {}, surfaceFormat, vk::SampleCountFlagBits::e1,
          vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore,
          vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare,
          vk::ImageLayout::eUndefined, vk::ImageLayout::ePresentSrcKHR)};

  auto inputAttachments = std::array<vk::AttachmentReference, 0>{};

  auto colorAttachments = std::array<vk::AttachmentReference, 1>{
      vk::AttachmentReference(0, vk::ImageLayout::eColorAttachmentOptimal)};

  auto subpassDescriptions = std::array<vk::SubpassDescription, 1>{
      vk::SubpassDescription({}, vk::PipelineBindPoint::eGraphics,
                             inputAttachments, colorAttachments)};

  auto renderPassCreateInfo =
      vk::RenderPassCreateInfo({}, attachmentDescriptions, subpassDescriptions);

  _renderPass = device.createRenderPass(renderPassCreateInfo);
}

RenderPass::~RenderPass() {
  auto& device = context()->device();
  device.destroy(_renderPass);
}

}  // namespace muchcool::rndr