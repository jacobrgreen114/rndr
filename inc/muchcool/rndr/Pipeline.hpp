
#pragma once

#include "DescriptorSet.hpp"

#include "RenderPass.hpp"
#include "Shader.hpp"

namespace rndr {

using ShaderStage = vk::ShaderStageFlagBits;
using PipelineBindPoint = vk::PipelineBindPoint;
using ShaderStageCreateInfo = vk::PipelineShaderStageCreateInfo;
using VertexInputBinding = vk::VertexInputBindingDescription;
using VertexInputAttribute = vk::VertexInputAttributeDescription;
using PrimitiveTopology = vk::PrimitiveTopology;

class PipelineLayout : public GraphicsObject {
  std::vector<Pointer<DescriptorSetLayout>> _descriptorSets;

  vk::PipelineLayout _pipelineLayout;

public:
  PipelineLayout(
      GraphicsContext* context,
      ArrayProxy<const Pointer<DescriptorSetLayout>> descriptorSets = {},
      ArrayProxy<vk::PushConstantRange> pushConstants = {});

  ~PipelineLayout() override;

  inline operator vk::PipelineLayout() const { return _pipelineLayout; }
};

class GraphicsPipeline : public GraphicsObject {
  Pointer<PipelineLayout> _pipelineLayout;
  vk::Pipeline _pipeline;

  Pointer<RenderPass> _renderPass;

  Pointer<Shader> _vertexShader;
  Pointer<Shader> _fragmentShader;

public:
  GraphicsPipeline(
      GraphicsContext* context, RenderPass* renderPass, PipelineLayout* layout,
      Shader* vertexShader = {}, Shader* fragmentShader = {},
      ArrayProxy<VertexInputBinding> vertexBindings = {},
      ArrayProxy<VertexInputAttribute> vertexAttributes = {},
      PrimitiveTopology topology = PrimitiveTopology::eTriangleList);

  ~GraphicsPipeline() override;

  inline operator const vk::Pipeline&() const { return _pipeline; }

  inline const auto& GetPipelineLayout() const { return _pipelineLayout; }
};

} // namespace Rndr