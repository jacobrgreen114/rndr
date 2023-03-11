
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#include <muchcool/core.hpp>

// #if OS_WINDOWS
// #define GLFW_EXPOSE_NATIVE_WIN32
// #define VK_USE_PLATFORM_WIN32_KHR
// #elif OS_LINUX
// #define GLFW_EXPOSE_NATIVE_WAYLAND
// #define VK_USE_PLATFORM_WAYLAND_KHR
// #elif OS_MACOS
// #define GLFW_EXPOSE_NATIVE_COCOA
// #define VK_USE_PLATFORM_MACOS_MVK
// #endif

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.hpp>

// #define GLM_FORCE_AVX2
// #define GLM_FORCE_RADIANS
// #define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
// #define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/transform.hpp>

namespace muchcool {
template <typename T>
using ArrayProxy = vk::ArrayProxy<T>;
}