
// Copyright (c) 2022-2023 Jacob R. Green
// All Rights Reserved.

#include "muchcool/rndr/Pipeline.hpp"

namespace rndr {

/*
 * Pipeline Layout
 */

vk::DescriptorSetLayout
ToVulkanType(const Pointer<DescriptorSetLayout> &layout) {
  return *layout;
}

std::vector<vk::DescriptorSetLayout> ToVulkanType(
    const ArrayProxy<const Pointer<DescriptorSetLayout>> &descriptorSets) {
  auto descriptorHandles =
      std::vector<vk::DescriptorSetLayout>(descriptorSets.size());
  std::transform(descriptorSets.begin(), descriptorSets.end(),
                 descriptorHandles.begin(),
                 [](auto &pDescriptor) { return ToVulkanType(pDescriptor); });
  return descriptorHandles;
}

PipelineLayout::PipelineLayout(
    GraphicsContext *context,
    ArrayProxy<const Pointer<DescriptorSetLayout>> descriptorSets,
    ArrayProxy<vk::PushConstantRange> pushConstants)
    : GraphicsObject(context),
      _descriptorSets(descriptorSets.begin(), descriptorSets.end()) {
  auto &device = GetGraphicsContext()->GetDevice();

  auto descriptorHandles = ToVulkanType(_descriptorSets);

  auto createInfo =
      vk::PipelineLayoutCreateInfo({}, descriptorHandles, pushConstants);
  _pipelineLayout = device.createPipelineLayout(createInfo);
}

PipelineLayout::~PipelineLayout() {
  auto &device = GetGraphicsContext()->GetDevice();
  device.destroy(_pipelineLayout);
}

/*
 * GraphicsPipeline
 */

GraphicsPipeline::GraphicsPipeline(
    GraphicsContext *context, RenderPass *renderPass,
    PipelineLayout *pipelineLayout, Shader *vertexShader,
    Shader *fragmentShader, ArrayProxy<VertexInputBinding> vertexBindings,
    ArrayProxy<VertexInputAttribute> vertexAttributes,
    PrimitiveTopology topology)
    : GraphicsObject(context), _renderPass(renderPass),
      _vertexShader(vertexShader), _fragmentShader(fragmentShader),
      _pipelineLayout(pipelineLayout) {

  auto &device = GetGraphicsContext()->GetDevice();

  auto shaderStages = std::array<vk::PipelineShaderStageCreateInfo, 2>{
      vk::PipelineShaderStageCreateInfo({}, vk::ShaderStageFlagBits::eVertex,
                                        *_vertexShader, "main"),
      vk::PipelineShaderStageCreateInfo({}, vk::ShaderStageFlagBits::eFragment,
                                        *_fragmentShader, "main"),
  };

  auto inputStateInfo = vk::PipelineVertexInputStateCreateInfo(
      {}, vertexBindings, vertexAttributes);

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
      &colorBlendAttachmentState, &dynamicStateInfo, *_pipelineLayout,
      *_renderPass, 0);

  _pipeline = device.createGraphicsPipeline(null, pipelineCreateInfo).value;
}

GraphicsPipeline::~GraphicsPipeline() {
  auto &device = GetGraphicsContext()->GetDevice();
  device.destroy(_pipeline);
}

} // namespace rndr