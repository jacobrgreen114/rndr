
// Copyright (c) 2022-2023 Jacob R. Green
// All Rights Reserved.

#include "muchcool/rndr/Shader.hpp"

namespace rndr {

Shader::Shader(GraphicsContext *context, ArrayProxy<uint8> code)
    : GraphicsObject(context) {
  auto &device = GetGraphicsContext()->GetDevice();

  auto createInfo = vk::ShaderModuleCreateInfo(
      {}, code.size(), reinterpret_cast<const uint32 *>(code.data()));

  _shader = device.createShaderModule(createInfo);
}

Shader::~Shader() {
  auto &device = GetGraphicsContext()->GetDevice();
  device.destroy(_shader);
}

Pointer<Shader> Shader::LoadFromFile(GraphicsContext *context,
                                     const char *path) {
  auto fileStream = std::ifstream(path, std::ios::binary | std::ios::ate);
  auto fileSize = fileStream.tellg();
  fileStream.seekg(0);

  if (fileSize < 0)
    throw std::exception();

  auto codeBuffer = std::vector<uint8>(fileSize);
  fileStream.read(reinterpret_cast<char *>(codeBuffer.data()),
                  codeBuffer.size());

  return new Shader(context, codeBuffer);
}

} // namespace rndr