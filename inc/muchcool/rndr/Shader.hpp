
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#include "GraphicsContext.hpp"

namespace rndr {

class Shader final : public GraphicsObject {
  vk::ShaderModule _shader;

 public:
  Shader(GraphicsContext* context, std::span<const uint8> code);
  ~Shader() override;

  inline operator const vk::ShaderModule&() const { return _shader; }

  static Pointer<Shader> FromData(GraphicsContext* context,
                                  std::span<const uint8> data);

  static Pointer<Shader> LoadFromFile(GraphicsContext* context,
                                      const char* path);
};

}  // namespace rndr