#pragma once
#define GLFW_INCLUDE_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_EXPOSE_NATIVE_WIN32
#include "IRenderSurface.h"
#include <GLFW/glfw3native.h>

namespace lc {
	class RenderWindow : public IRenderSurface {
		const int MAX_FRAMES_IN_FLIGHT = 2;
		GLFWwindow* handle;

	public:
		RenderWindow(int width, int height, std::string title, bool isFullscreen = false);
		~RenderWindow();
		virtual void frameReady(VkSemaphore signalSemaphores[], VkQueue presentQueue);
		virtual int getSwapchainImageIndex(VkDevice device, VkSemaphore semaphore);
		virtual void createSurface(VkInstance instance);
		virtual void initSwapchain(VkPhysicalDevice physicalDevice, VkDevice device,
															 QueueFamilyIndices indices);
		virtual void createSwapchainImages(VkDevice device, uint32_t imageCount);
		virtual void initFramebuffers(std::shared_ptr<RenderPass> renderPass);
		virtual GLFWwindow* getHandle() const;
		virtual void pollEvents();
		virtual bool isOpen();
		virtual void cleanup(VkInstance instance, VkDevice device);
	};
} // namespace lc
