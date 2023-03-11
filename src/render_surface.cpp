
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "muchcool/rndr/render_surface.hpp"

namespace muchcool::rndr {

RenderSurface::RenderSurface(Shared<GraphicsContext> context_,
                             HWND windowHandle)
    : GraphicsObject(std::move(context_)) {
  auto& instance = context()->instance();
  auto& physicalDevice = context()->physical_device();
  auto& device = context()->device();

  auto surfaceCreateInfo =
      vk::Win32SurfaceCreateInfoKHR({}, GetModuleHandle(null), windowHandle);
  _surface = instance.createWin32SurfaceKHR(surfaceCreateInfo);

  if (!physicalDevice.getSurfaceSupportKHR(context()->queue_family_index(),
                                           _surface)) {
    throw std::exception("Physical device does not support this surface.");
  }

  _surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(_surface);

  auto surfaceFormats = physicalDevice.getSurfaceFormatsKHR(_surface);
  _surfaceFormat = surfaceFormats[0];

  auto presentModes = physicalDevice.getSurfacePresentModesKHR(_surface);
  _presentMode = presentModes[0];

  _renderPass = Shared{new RenderPass(context(), _surfaceFormat.format)};

  _inFlightFence = device.createFence({});

  CreateSwapchain();
}

Shared<RenderSurface> RenderSurface::New(Shared<GraphicsContext> context_,
                                         HWND windowHandle) {
  return Shared{new RenderSurface(std::move(context_), windowHandle)};
}

void RenderSurface::Destroy() {
  auto& graphicContext = *context();
  auto& instance = graphicContext.instance();
  auto& device = graphicContext.device();

  DestroySwapchain();
  device.destroy(_inFlightFence);
  instance.destroy(_surface);
}

void RenderSurface::CreateSwapchain() {
  auto& device = context()->device();

  auto swapchainCreateInfo = vk::SwapchainCreateInfoKHR(
      {}, _surface, 3, _surfaceFormat.format, _surfaceFormat.colorSpace,
      _surfaceCapabilities.currentExtent, 1,
      vk::ImageUsageFlagBits::eColorAttachment);
  swapchainCreateInfo.clipped = true;
  swapchainCreateInfo.presentMode = _presentMode;

  _swapchain = device.createSwapchainKHR(swapchainCreateInfo);
  _swapchainImages = device.getSwapchainImagesKHR(_swapchain);
  _swapchainImageViews = std::vector<vk::ImageView>(_swapchainImages.size());

  std::transform(_swapchainImages.begin(), _swapchainImages.end(),
                 _swapchainImageViews.begin(),
                 [this, device](auto& image) -> vk::ImageView {
                   auto createInfo = vk::ImageViewCreateInfo(
                       {}, image, vk::ImageViewType::e2D, _surfaceFormat.format,
                       {},
                       vk::ImageSubresourceRange(
                           vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));
                   return device.createImageView(createInfo);
                 });

  _frameBuffers = std::vector<vk::Framebuffer>(_swapchainImageViews.size());

  std::transform(_swapchainImageViews.begin(), _swapchainImageViews.end(),
                 _frameBuffers.begin(),
                 [this, device](auto& view) -> vk::Framebuffer {
                   auto createInfo = vk::FramebufferCreateInfo(
                       {}, *_renderPass, view, GetCurrentExtent().width,
                       GetCurrentExtent().height, 1);
                   return device.createFramebuffer(createInfo);
                 });

  _viewport =
      vk::Viewport(0, 0, _surfaceCapabilities.currentExtent.width,
                   _surfaceCapabilities.currentExtent.height, 0.0f, 1.0f);
  _scissor = vk::Rect2D({}, _surfaceCapabilities.currentExtent);
}

void RenderSurface::DestroySwapchain() {
  auto& device = context()->device();

  for (auto& fb : _frameBuffers) device.destroy(fb);
  _frameBuffers.clear();

  for (auto& view : _swapchainImageViews) device.destroy(view);
  _swapchainImageViews.clear();

  _swapchainImages.clear();

  device.destroy(_swapchain);
  _swapchain = null;
}

void RenderSurface::RecreateSwapchain() {
  DestroySwapchain();
  CreateSwapchain();
}

void RenderSurface::UpdateSwapchain() {
  auto renderLock = LockRenderMutex();

  _surfaceCapabilities =
      context()->physical_device().getSurfaceCapabilitiesKHR(_surface);
  RecreateSwapchain();
}

}  // namespace muchcool::rndr
