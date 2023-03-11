
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "muchcool/rndr/shader.hpp"

namespace muchcool::rndr {

Shader::Shader(Shared<GraphicsContext> context_, ArrayProxy<const uint8> code)
    : GraphicsObject(std::move(context_)) {
  auto& device = context()->device();

  auto createInfo = vk::ShaderModuleCreateInfo(
      {}, code.size(), reinterpret_cast<const uint32*>(code.data()));

  _shader = device.createShaderModule(createInfo);
}

Shader::~Shader() {
  auto& device = context()->device();
  device.destroy(_shader);
}

Shared<Shader> Shader::FromData(Shared<GraphicsContext> context,
                                const ArrayProxy<const uint8> data) {
  return Shared{new Shader(std::move(context), data)};
}

Shared<Shader> Shader::LoadFromFile(Shared<GraphicsContext> context,
                                    const char* path) {
  auto fileStream = std::ifstream(path, std::ios::binary | std::ios::ate);
  auto fileSize = fileStream.tellg();
  fileStream.seekg(0);

  if (fileSize < 0) throw std::exception();

  auto codeBuffer = std::vector<uint8>(fileSize);
  fileStream.read(reinterpret_cast<char*>(codeBuffer.data()),
                  codeBuffer.size());

  return Shared{new Shader(std::move(context), codeBuffer)};
}

}  // namespace muchcool::rndr