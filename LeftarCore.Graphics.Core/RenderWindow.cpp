#include "RenderWindow.h"
#include "RenderingEngine.h"

namespace lc {
	RenderWindow::RenderWindow(int width, int height, std::string title, bool isFullscreen) {
		size = { width, height };

		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		GLFWmonitor* monitor = nullptr;
		if (isFullscreen) {
			monitor = glfwGetPrimaryMonitor();
		}

		handle = glfwCreateWindow(width, height, title.c_str(), monitor, NULL);
	}

	bool RenderWindow::isOpen() {
		return !glfwWindowShouldClose(handle);
	}

	void RenderWindow::cleanup(VkInstance instance, VkDevice device) {
		for (auto imageView : swapchainImageViews) {
			vkDestroyImageView(device, imageView, nullptr);
		}

		vkDestroySwapchainKHR(device, swapchain, nullptr);
		vkDestroySurfaceKHR(instance, surface, nullptr);
		glfwDestroyWindow(handle);
	}

	RenderWindow::~RenderWindow() {}

	void RenderWindow::frameReady(VkSemaphore signalSemaphores[], VkQueue presentQueue) {
		VkPresentInfoKHR presentInfo {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;
		VkSwapchainKHR swapChains[] = { swapchain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = (uint32_t*)&currentFrame;

		vkQueuePresentKHR(presentQueue, &presentInfo);
		vkQueueWaitIdle(presentQueue);
	}

	int RenderWindow::getSwapchainImageIndex(VkDevice device, VkSemaphore semaphore) {
		uint32_t imageIndex;
		vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, semaphore, VK_NULL_HANDLE, &imageIndex);

		return imageIndex;
	}

	void RenderWindow::createSurface(VkInstance instance) {
		VkWin32SurfaceCreateInfoKHR createInfo {};
		createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		createInfo.hwnd = glfwGetWin32Window(handle);
		createInfo.hinstance = GetModuleHandle(nullptr);

		if (vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}

		if (glfwCreateWindowSurface(instance, handle, nullptr, &surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
	}

	void RenderWindow::initSwapchain(VkPhysicalDevice physicalDevice, VkDevice device,
																	 QueueFamilyIndices indices) {
		SwapChainSupportDetails swapChainSupport =
				VulkanUtilities::querySwapChainSupport(physicalDevice, surface);

		VkSurfaceFormatKHR surfaceFormat =
				VulkanUtilities::chooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode =
				VulkanUtilities::chooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent =
				VulkanUtilities::chooseSwapExtent(swapChainSupport.capabilities, size.x, size.y);

		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

		if (swapChainSupport.capabilities.maxImageCount > 0 &&
				imageCount > swapChainSupport.capabilities.maxImageCount) {
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		uint32_t uniqueQueueFamilies[] = { indices.graphicsFamily.value(),
																			 indices.presentFamily.value() };

		VkSwapchainCreateInfoKHR createSwapchainInfo {};
		createSwapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createSwapchainInfo.surface = surface;
		createSwapchainInfo.minImageCount = imageCount;
		createSwapchainInfo.imageFormat = surfaceFormat.format;
		createSwapchainInfo.imageColorSpace = surfaceFormat.colorSpace;
		createSwapchainInfo.imageExtent = extent;
		createSwapchainInfo.imageArrayLayers = 1;
		createSwapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		if (indices.graphicsFamily != indices.presentFamily) {
			createSwapchainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createSwapchainInfo.queueFamilyIndexCount = 2;
			createSwapchainInfo.pQueueFamilyIndices = uniqueQueueFamilies;
		} else {
			createSwapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}

		createSwapchainInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createSwapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createSwapchainInfo.presentMode = presentMode;
		createSwapchainInfo.clipped = VK_TRUE;
		createSwapchainInfo.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(device, &createSwapchainInfo, nullptr, &swapchain) != VK_SUCCESS) {
			throw std::runtime_error("failed to create swap chain!");
		}

		swapchainImageFormat = surfaceFormat.format;
		swapchainExtent = extent;

		createSwapchainImages(device, imageCount);
	}

	void RenderWindow::createSwapchainImages(VkDevice device, uint32_t imageCount) {
		vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr);
		swapchainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(device, swapchain, &imageCount, swapchainImages.data());

		swapchainImageViews.resize(swapchainImages.size());

		for (size_t i = 0; i < swapchainImages.size(); i++) {
			VkImageViewCreateInfo createImageViewInfo {};
			createImageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createImageViewInfo.image = swapchainImages[i];
			createImageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createImageViewInfo.format = swapchainImageFormat;

			createImageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createImageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createImageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createImageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

			createImageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createImageViewInfo.subresourceRange.baseMipLevel = 0;
			createImageViewInfo.subresourceRange.levelCount = 1;
			createImageViewInfo.subresourceRange.baseArrayLayer = 0;
			createImageViewInfo.subresourceRange.layerCount = 1;

			if (vkCreateImageView(device, &createImageViewInfo, nullptr, &swapchainImageViews[i]) !=
					VK_SUCCESS) {
				throw std::runtime_error("failed to create image views!");
			}
		}
	}

	void RenderWindow::initFramebuffers(std::shared_ptr<RenderPass> renderPass) {
		for (size_t i = 0; i < swapchainImageViews.size(); i++) {
			std::vector<VkImageView> attachments = { swapchainImageViews[i] };
			framebuffers.push_back(std::shared_ptr<Framebuffer>(new Framebuffer(
					{ swapchainExtent.width, swapchainExtent.height }, attachments, renderPass)));
		}
	}

	GLFWwindow* RenderWindow::getHandle() const {
		return handle;
	}

	void RenderWindow::pollEvents() {
		glfwPollEvents();
	}
} // namespace lc
