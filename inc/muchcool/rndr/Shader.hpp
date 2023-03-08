
#pragma once

#include "GraphicsContext.hpp"

namespace rndr {

class Shader final : public GraphicsObject {
  vk::ShaderModule _shader;

public:
  Shader(GraphicsContext* context, ArrayProxy<uint8> code);
  ~Shader() override;

  inline operator const vk::ShaderModule&() const { return _shader; }

  static Pointer<Shader> LoadFromFile(GraphicsContext* context,
                                      const char* path);
};

} // namespace Rndr