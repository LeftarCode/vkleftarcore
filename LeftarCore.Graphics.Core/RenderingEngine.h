#pragma once
#define GLFW_INCLUDE_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_EXPOSE_NATIVE_WIN32
#include <stdexcept>
#include <vector>
#include <iostream>
#include "VkImport.h"
#include <map>
#include <set>
#include "VulkanUtilities.h"
#include "RenderWindow.h"
#include "RenderPipeline.h"
#include "../LeftarCore.Core/lcpch.h"
#include "../LeftarCore.Core/Vertex.h"
#include "Buffer.h"
#include <GLFW/glfw3native.h>
#include <GLFW/glfw3.h>

namespace lc {
	enum EngineOperatingMode { eNoneMode, eEditor, eGame, MODE_COUNT };

	struct RenderEngineCreateInfo {
		EngineOperatingMode engineMode;
		glm::vec2 size;
		std::string title;
	};

	class RenderingEngine {
		int MAX_FRAMES_IN_FLIGHT = 2;
		RenderEngineCreateInfo createInfo;
		static std::shared_ptr<RenderingEngine> _instance;
#ifdef _DEBUG
		const bool enableValidationLayers = true;
#else
		const bool enableValidationLayers = false;
#endif
		std::vector<const char*> getRequiredExtensions();
		std::vector<const char*> getValidationLayers();
		std::shared_ptr<IRenderSurface> renderSurface;
		VkInstance instance {};
		VkDebugUtilsMessengerEXT debugMessenger;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice device = VK_NULL_HANDLE;
#pragma region Present Purpose Members
		VkQueue graphicsQueue = VK_NULL_HANDLE;
		VkQueue presentQueue = VK_NULL_HANDLE;
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		std::vector<VkFence> imagesInFlight;
		std::vector<VkCommandBuffer> commandBuffers;
		VkCommandPool commandPool;
#pragma endregion
		std::vector<const char*> validationLayers { "VK_LAYER_KHRONOS_validation" };
		std::vector<const char*> enabledExtensions { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		void initSurface();
		RenderingEngine(RenderEngineCreateInfo createInfo);

	public:
		static std::shared_ptr<RenderingEngine> create(RenderEngineCreateInfo createInfo);
		static std::shared_ptr<RenderingEngine> getInstance();
		void setRenderSurface(IRenderSurface* renderSurface);
		void createVkInstance();
		void init();
		void init(VkPhysicalDevice physicalDevice, VkDevice device, VkQueue graphicsQueue,
							VkCommandPool commandPool);
		std::shared_ptr<IRenderSurface> getWindow();
		VkInstance getVkInstance();
		VkDevice getDeviceHandle();
		VkCommandPool getCommandPool();
		VkQueue getGraphicsQueue();
		void initSwapchain();
		void initCommandBuffers(std::shared_ptr<RenderPass> renderPass,
														std::shared_ptr<RenderPipeline> renderPipeline,
														std::shared_ptr<Buffer> vertexBuffer,
														std::shared_ptr<Buffer> indexBuffer);
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		void drawEditor(VkCommandBuffer commandBuffer, VkFramebuffer framebuffer,
										std::shared_ptr<RenderPass> renderPass,
										std::shared_ptr<RenderPipeline> renderPipeline,
										std::shared_ptr<Buffer> vertexBuffer, std::shared_ptr<Buffer> indexBuffer);
		void draw();
		~RenderingEngine();
	};
} // namespace lc
