
#pragma once

#include "RenderPass.hpp"

namespace rndr {

class RenderSurface final : public GraphicsObject {
private:
  vk::SurfaceKHR _surface;
  vk::SurfaceCapabilitiesKHR _surfaceCapabilities;
  vk::SurfaceFormatKHR _surfaceFormat;
  vk::PresentModeKHR _presentMode;

  vk::SwapchainKHR _swapchain;
  std::vector<vk::Image> _swapchainImages;
  std::vector<vk::ImageView> _swapchainImageViews;

  Pointer<rndr::RenderPass> _renderPass;
  std::vector<vk::Framebuffer> _frameBuffers;
  vk::Fence _inFlightFence;

  vk::Viewport _viewport;
  vk::Rect2D _scissor;

  std::mutex _renderMutex;

public:
  RenderSurface(GraphicsContext* graphicsContext, HWND windowHandle);

  static Pointer<RenderSurface> New(GraphicsContext* graphicsContext,
                                    HWND windowHandle);

  void UpdateSwapchain();
  void Destroy();

  inline const vk::SurfaceKHR& GetSurface() const { return _surface; }
  inline const vk::Extent2D& GetCurrentExtent() const {
    return _surfaceCapabilities.currentExtent;
  }
  inline const vk::SurfaceFormatKHR& GetSurfaceFormat() const {
    return _surfaceFormat;
  }
  inline const vk::PresentModeKHR& GetSurfacePresentMode() const {
    return _presentMode;
  }

  inline const vk::SwapchainKHR& GetSwapchain() const { return _swapchain; }
  inline const std::vector<vk::Image>& GetSwapchainImages() const {
    return _swapchainImages;
  }
  inline const std::vector<vk::ImageView>& GetSwapchainImageViews() const {
    return _swapchainImageViews;
  }

  inline const Pointer<rndr::RenderPass>& GetRenderPass() const {
    return _renderPass;
  }

  inline const std::vector<vk::Framebuffer>& GetFrameBuffers() const {
    return _frameBuffers;
  }

  inline const vk::Viewport& GetViewport() const { return _viewport; }
  inline const vk::Rect2D& GetScissor() const { return _scissor; }

  inline const vk::Fence& GetInFlightFence() const { return _inFlightFence; }

  inline auto LockRenderMutex() { return std::lock_guard(_renderMutex); }

private:
  void CreateSwapchain();
  void DestroySwapchain();
  void RecreateSwapchain();
};

} // namespace Rndr
