
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#include "render_pass.hpp"

namespace muchcool::rndr {

class RenderSurface final : public GraphicsObject {
 private:
  vk::SurfaceKHR _surface;
  vk::SurfaceCapabilitiesKHR _surfaceCapabilities;
  vk::SurfaceFormatKHR _surfaceFormat;
  vk::PresentModeKHR _presentMode;

  vk::SwapchainKHR _swapchain;
  std::vector<vk::Image> _swapchainImages;
  std::vector<vk::ImageView> _swapchainImageViews;

  Shared<rndr::RenderPass> _renderPass;
  std::vector<vk::Framebuffer> _frameBuffers;
  vk::Fence _inFlightFence;

  vk::Viewport _viewport;
  vk::Rect2D _scissor;

  std::mutex _renderMutex;

 public:
  RenderSurface(Shared<GraphicsContext> context, HWND windowHandle);

  static Shared<RenderSurface> New(Shared<GraphicsContext> context,
                                   HWND windowHandle);

  void UpdateSwapchain();
  void Destroy();

  const vk::SurfaceKHR& GetSurface() const { return _surface; }
  const vk::Extent2D& GetCurrentExtent() const {
    return _surfaceCapabilities.currentExtent;
  }
  const vk::SurfaceFormatKHR& GetSurfaceFormat() const {
    return _surfaceFormat;
  }
  const vk::PresentModeKHR& GetSurfacePresentMode() const {
    return _presentMode;
  }

  const vk::SwapchainKHR& GetSwapchain() const { return _swapchain; }
  const std::vector<vk::Image>& GetSwapchainImages() const {
    return _swapchainImages;
  }
  const std::vector<vk::ImageView>& GetSwapchainImageViews() const {
    return _swapchainImageViews;
  }

  const Shared<rndr::RenderPass>& GetRenderPass() const {
    return _renderPass;
  }

  const std::vector<vk::Framebuffer>& GetFrameBuffers() const {
    return _frameBuffers;
  }

  const vk::Viewport& GetViewport() const { return _viewport; }
  const vk::Rect2D& GetScissor() const { return _scissor; }

  const vk::Fence& GetInFlightFence() const { return _inFlightFence; }

  auto LockRenderMutex() { return std::lock_guard(_renderMutex); }

 private:
  void CreateSwapchain();
  void DestroySwapchain();
  void RecreateSwapchain();
};

}  // namespace muchcool::rndr
