#include "IRenderSurface.h"

namespace lc {
	std::vector<VkImage> IRenderSurface::getSwapchainImages() {
		return swapchainImages;
	}
	VkSurfaceKHR IRenderSurface::getSurface() {
		return surface;
	}

	VkSwapchainKHR IRenderSurface::getSwapchain() {
		return swapchain;
	}

	VkFormat IRenderSurface::getSwapchainImageFormat() {
		return swapchainImageFormat;
	}

	std::vector<std::shared_ptr<Framebuffer>> IRenderSurface::getFramebuffers() {
		return framebuffers;
	}
	VkExtent2D IRenderSurface::getExtent() {
		return swapchainExtent;
	}

	int IRenderSurface::getCurrentFrame() {
		return currentFrame;
	}

	void IRenderSurface::nextFrame() {
		currentFrame = (currentFrame + 1) % maxFrames;
	}
} // namespace lc
