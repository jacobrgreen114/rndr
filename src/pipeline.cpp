
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "muchcool/rndr/pipeline.hpp"

namespace muchcool::rndr {

/*
 * Pipeline Layout
 */

vk::DescriptorSetLayout ToVulkanType(
    const Shared<DescriptorSetLayout>& layout) {
  return *layout;
}

std::vector<vk::DescriptorSetLayout> ToVulkanType(
    const ArrayProxy<const Shared<DescriptorSetLayout>>& descriptorSets) {
  auto descriptorHandles =
      std::vector<vk::DescriptorSetLayout>(descriptorSets.size());
  std::transform(descriptorSets.begin(), descriptorSets.end(),
                 descriptorHandles.begin(),
                 [](auto& pDescriptor) { return ToVulkanType(pDescriptor); });
  return descriptorHandles;
}

PipelineLayout::PipelineLayout(
    Shared<GraphicsContext> context_,
    ArrayProxy<const Shared<DescriptorSetLayout>> descriptorSets,
    ArrayProxy<vk::PushConstantRange> pushConstants)
    : GraphicsObject(std::move(context_)),
      _descriptorSets(descriptorSets.begin(), descriptorSets.end()) {
  auto& device = context()->device();

  auto descriptorHandles = ToVulkanType(_descriptorSets);

  auto createInfo =
      vk::PipelineLayoutCreateInfo({}, descriptorHandles, pushConstants);
  _pipelineLayout = device.createPipelineLayout(createInfo);
}

PipelineLayout::~PipelineLayout() {
  auto& device = context()->device();
  device.destroy(_pipelineLayout);
}

/*
 * GraphicsPipeline
 */

GraphicsPipeline::GraphicsPipeline(
    Shared<GraphicsContext> context_, Shared<RenderPass> render_pass,
    Shared<PipelineLayout> layout, Shared<Shader> vertex_shader,
    Shared<Shader> fragment_shader,
    ArrayProxy<VertexInputBinding> vertex_bindings,
    ArrayProxy<VertexInputAttribute> vertex_attributes,
    PrimitiveTopology topology)
    : GraphicsObject(std::move(context_)),
      _render_pass(std::move(render_pass)),
      _vertex_shader(std::move(vertex_shader)),
      _fragment_shader(std::move(fragment_shader)),
      _pipeline_layout(std::move(layout)) {
  auto& device = context()->device();

  auto shaderStages = std::array<vk::PipelineShaderStageCreateInfo, 2>{
      vk::PipelineShaderStageCreateInfo({}, vk::ShaderStageFlagBits::eVertex,
                                        *_vertex_shader, "main"),
      vk::PipelineShaderStageCreateInfo({}, vk::ShaderStageFlagBits::eFragment,
                                        *_fragment_shader, "main"),
  };

  auto inputStateInfo = vk::PipelineVertexInputStateCreateInfo(
      {}, vertex_bindings, vertex_attributes);

  auto inputAssemblyInfo =
      vk::PipelineInputAssemblyStateCreateInfo({}, topology, VK_FALSE);

  auto viewport = vk::Viewport();
  auto scissor = vk::Rect2D();
  auto viewportState =
      vk::PipelineViewportStateCreateInfo({}, viewport, scissor);

  auto rasterizationState = vk::PipelineRasterizationStateCreateInfo();
  rasterizationState.frontFace = vk::FrontFace::eCounterClockwise;
  rasterizationState.cullMode = vk::CullModeFlagBits::eNone;
  rasterizationState.lineWidth = 1.0f;

  auto multisampleState = vk::PipelineMultisampleStateCreateInfo();
  multisampleState.setMinSampleShading(1.0f);
  // multisampleState.setRasterizationSamples(vk::SampleCountFlagBits::e1);

  auto colorBlendAttachment = vk::PipelineColorBlendAttachmentState();
  colorBlendAttachment.blendEnable = VK_TRUE;
  colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
  colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
  colorBlendAttachment.colorBlendOp = vk::BlendOp::eAdd;
  colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;
  colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eOne;
  colorBlendAttachment.alphaBlendOp = vk::BlendOp::eAdd;
  colorBlendAttachment.colorWriteMask =
      vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
      vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;

  auto colorBlendAttachmentState = vk::PipelineColorBlendStateCreateInfo(
      {}, {}, {}, 1, &colorBlendAttachment);

  auto dynamicStates = std::array<vk::DynamicState, 2>{
      vk::DynamicState::eViewport, vk::DynamicState::eScissor};
  auto dynamicStateInfo = vk::PipelineDynamicStateCreateInfo({}, dynamicStates);

  auto pipelineCreateInfo = vk::GraphicsPipelineCreateInfo(
      {}, shaderStages, &inputStateInfo, &inputAssemblyInfo, null,
      &viewportState, &rasterizationState, &multisampleState, null,
      &colorBlendAttachmentState, &dynamicStateInfo, *_pipeline_layout,
      *_render_pass, 0);

  _pipeline = device.createGraphicsPipeline(null, pipelineCreateInfo).value;
}

GraphicsPipeline::~GraphicsPipeline() {
  auto& device = context()->device();
  device.destroy(_pipeline);
}

}  // namespace muchcool::rndr