
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#include "descriptor_set.hpp"

#include "render_pass.hpp"
#include "shader.hpp"

namespace muchcool::rndr {

using ShaderStage = vk::ShaderStageFlagBits;
using PipelineBindPoint = vk::PipelineBindPoint;
using ShaderStageCreateInfo = vk::PipelineShaderStageCreateInfo;
using VertexInputBinding = vk::VertexInputBindingDescription;
using VertexInputAttribute = vk::VertexInputAttributeDescription;
using PrimitiveTopology = vk::PrimitiveTopology;

class PipelineLayout : public GraphicsObject {
  std::vector<Shared<DescriptorSetLayout>> _descriptorSets;

  vk::PipelineLayout _pipelineLayout;

 public:
  PipelineLayout(
      Shared<GraphicsContext> context,
      ArrayProxy<const Shared<DescriptorSetLayout>> descriptorSets = {},
      ArrayProxy<vk::PushConstantRange> pushConstants = {});

  ~PipelineLayout() override;

  operator vk::PipelineLayout() const { return _pipelineLayout; }
};

class GraphicsPipeline : public GraphicsObject {
  Shared<PipelineLayout> _pipeline_layout;
  vk::Pipeline _pipeline;

  Shared<RenderPass> _render_pass;

  Shared<Shader> _vertex_shader;
  Shared<Shader> _fragment_shader;

 public:
  GraphicsPipeline(
      Shared<GraphicsContext> context, Shared<RenderPass> render_pass,
      Shared<PipelineLayout> layout, Shared<Shader> vertex_shader = {},
      Shared<Shader> fragment_shader = {},
      ArrayProxy<VertexInputBinding> vertex_bindings = {},
      ArrayProxy<VertexInputAttribute> vertex_attributes = {},
      PrimitiveTopology topology = PrimitiveTopology::eTriangleList);

  ~GraphicsPipeline() override;

  operator const vk::Pipeline&() const { return _pipeline; }

  const auto& pipeline_layout() const { return _pipeline_layout; }
};

}  // namespace muchcool::rndr