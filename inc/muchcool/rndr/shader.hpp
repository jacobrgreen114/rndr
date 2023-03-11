
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#include "graphics_context.hpp"

namespace muchcool::rndr {

class Shader final : public GraphicsObject {
  vk::ShaderModule _shader;

 public:
  Shader(Shared<GraphicsContext> context, ArrayProxy<const uint8> code);
  ~Shader() override;

  operator const vk::ShaderModule&() const { return _shader; }

  static Shared<Shader> FromData(Shared<GraphicsContext> context,
                                 ArrayProxy<const uint8> data);

  static Shared<Shader> LoadFromFile(Shared<GraphicsContext> context,
                                     const char* path);
};

}  // namespace muchcool::rndr