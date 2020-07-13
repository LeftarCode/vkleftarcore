#pragma once
#define GLFW_INCLUDE_VULKAN
#include <stdexcept>
#include <vector>
#include <string>
#include <set>
#include "VulkanUtilities.h"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "Framebuffer.h"

namespace lc {
	class IRenderSurface {
	protected:
		int currentFrame = 0;
		int maxFrames = 2;
		glm::vec2 size;
		VkSurfaceKHR surface = VK_NULL_HANDLE;
		VkSwapchainKHR swapchain = VK_NULL_HANDLE;
		VkFormat swapchainImageFormat;
		VkExtent2D swapchainExtent;
		std::vector<std::shared_ptr<Framebuffer>> framebuffers;
		std::vector<VkImageView> swapchainImageViews;
		std::vector<VkImage> swapchainImages;

	public:
		virtual std::vector<VkImage> getSwapchainImages();
		virtual VkSurfaceKHR getSurface();
		virtual VkSwapchainKHR getSwapchain();
		virtual VkFormat getSwapchainImageFormat();
		virtual std::vector<std::shared_ptr<Framebuffer>> getFramebuffers();
		virtual VkExtent2D getExtent();
		virtual int getCurrentFrame();
		virtual void nextFrame();
		virtual void frameReady(VkSemaphore signalSemaphores[], VkQueue presentQueue) = 0;
		virtual int getSwapchainImageIndex(VkDevice device, VkSemaphore semaphore) = 0;
		virtual void createSurface(VkInstance instance) = 0;
		virtual void initSwapchain(VkPhysicalDevice physicalDevice, VkDevice device,
															 QueueFamilyIndices indices) = 0;
		virtual void createSwapchainImages(VkDevice device, uint32_t imageCount) = 0;
		virtual void initFramebuffers(std::shared_ptr<RenderPass> renderPass) = 0;
		virtual void pollEvents() = 0;
		virtual bool isOpen() = 0;
		virtual void cleanup(VkInstance instance, VkDevice device) = 0;
	};
} // namespace lc
