#include "RenderingEngine.h"

namespace lc {
	std::shared_ptr<RenderingEngine> RenderingEngine::_instance = NULL;

	std::vector<const char*> RenderingEngine::getRequiredExtensions() {
		if (createInfo.engineMode == eGame) {
			uint32_t glfwExtensionCount = 0;
			const char** glfwExtensions;
			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

			std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

			if (enableValidationLayers) {
				extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			}

			return extensions;
		} else if (createInfo.engineMode == eEditor) {
			std::vector<const char*> extensions;
			if (enableValidationLayers) {
				extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			}

			extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
			extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

			return extensions;
		}

		return std::vector<const char*>();
	}

	std::vector<const char*> RenderingEngine::getValidationLayers() {
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		std::vector<const char*> enabledValidationLayers;
		for (auto layer : validationLayers) {
			bool layerFound = false;
			for (auto available : availableLayers) {
				if (strcmp(available.layerName, layer) == 0) {
					layerFound = true;
					break;
				}
			}

			if (layerFound) {
				enabledValidationLayers.push_back(layer);
			}
		}

		return enabledValidationLayers;
	}

	void RenderingEngine::initSwapchain() {
		renderSurface->initSwapchain(physicalDevice, device, QueueFamilyIndices());
	}

	void RenderingEngine::setRenderSurface(IRenderSurface* renderSurface) {
		this->renderSurface.reset(renderSurface);
		this->initSurface();
	}

	void RenderingEngine::createVkInstance() {
		VkApplicationInfo appInfo {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "TechDemo";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "LeftarCore";
		appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
		appInfo.apiVersion = VK_API_VERSION_1_2;

		VkInstanceCreateInfo instanceCreateInfo {};
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.pApplicationInfo = &appInfo;

		std::vector<const char*> extesions = getRequiredExtensions();
		std::vector<const char*> enabledValidationLayers = getValidationLayers();

		instanceCreateInfo.enabledExtensionCount = extesions.size();
		instanceCreateInfo.ppEnabledExtensionNames = extesions.data();
		instanceCreateInfo.enabledLayerCount = 0;

		VkDebugUtilsMessengerCreateInfoEXT createDebugMessengerInfo {};
		if (enableValidationLayers) {
			instanceCreateInfo.ppEnabledLayerNames = enabledValidationLayers.data();
			instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(enabledValidationLayers.size());

			createDebugMessengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			createDebugMessengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
																								 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
																								 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			createDebugMessengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
																						 VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
																						 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			createDebugMessengerInfo.pfnUserCallback = vulkanDebugCallback;
			createDebugMessengerInfo.pUserData = nullptr;

			instanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&createDebugMessengerInfo;
		}

		if (vkCreateInstance(&instanceCreateInfo, nullptr, &instance) != VK_SUCCESS) {
			throw std::runtime_error("failed to create instance");
		}

		if (enableValidationLayers) {
			if (VkImport::createDebugUtilsMessenger(instance, &createDebugMessengerInfo, nullptr,
																							&debugMessenger) != VK_SUCCESS) {
				throw std::runtime_error("failed to set up debug messenger!");
			}
		}
	}

	void RenderingEngine::init() {
		std::vector<const char*> enabledValidationLayers = getValidationLayers();
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

		if (deviceCount == 0) {
			throw std::runtime_error("failed to find GPUs with Vulkan support!");
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

		physicalDevice = VulkanUtilities::pickPhysicalDevice(devices, renderSurface->getSurface(),
																												 enabledExtensions);
		QueueFamilyIndices indices =
				VulkanUtilities::findQueueFamilies(physicalDevice, renderSurface->getSurface());

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(),
																							 indices.presentFamily.value() };

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures {};

		VkDeviceCreateInfo createDeviceInfo {};
		createDeviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createDeviceInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createDeviceInfo.pQueueCreateInfos = queueCreateInfos.data();
		createDeviceInfo.pEnabledFeatures = &deviceFeatures;
		createDeviceInfo.enabledExtensionCount = enabledExtensions.size();
		createDeviceInfo.ppEnabledExtensionNames = enabledExtensions.data();

		if (enableValidationLayers) {
			createDeviceInfo.enabledLayerCount = static_cast<uint32_t>(enabledValidationLayers.size());
			createDeviceInfo.ppEnabledLayerNames = enabledValidationLayers.data();
		} else {
			createDeviceInfo.enabledLayerCount = 0;
		}

		if (vkCreateDevice(physicalDevice, &createDeviceInfo, nullptr, &device) != VK_SUCCESS) {
			throw std::runtime_error("failed to create logical device!");
		}

		vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
		vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);

		renderSurface->initSwapchain(physicalDevice, device, indices);

		VkCommandPoolCreateInfo poolInfo {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = indices.graphicsFamily.value();
		poolInfo.flags = 0;

		if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
			throw std::runtime_error("failed to create command pool!");
		}

		imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
		imagesInFlight.resize(renderSurface->getSwapchainImages().size(), VK_NULL_HANDLE);

		VkSemaphoreCreateInfo semaphoreInfo {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) !=
							VK_SUCCESS ||
					vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) !=
							VK_SUCCESS ||
					vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to create synchronization objects for a frame!");
			}
		}
	}

	void RenderingEngine::initSurface() {
		createVkInstance();

		renderSurface->createSurface(instance);
	}

	void RenderingEngine::init(VkPhysicalDevice physicalDevice, VkDevice device,
														 VkQueue graphicsQueue, VkCommandPool commandPool) {
		this->physicalDevice = physicalDevice;
		this->device = device;
		this->graphicsQueue = graphicsQueue;
		this->commandPool = commandPool;

		renderSurface->initSwapchain(physicalDevice, device, lc::QueueFamilyIndices());
	}

	VkInstance RenderingEngine::getVkInstance() {
		return instance;
	}

	std::shared_ptr<IRenderSurface> RenderingEngine::getWindow() {
		return renderSurface;
	}

	VkDevice RenderingEngine::getDeviceHandle() {
		return device;
	}

	VkCommandPool RenderingEngine::getCommandPool() {
		return commandPool;
	}

	VkQueue RenderingEngine::getGraphicsQueue() {
		return graphicsQueue;
	}

	void RenderingEngine::initCommandBuffers(std::shared_ptr<RenderPass> renderPass,
																					 std::shared_ptr<RenderPipeline> renderPipeline,
																					 std::shared_ptr<Buffer> vertexBuffer,
																					 std::shared_ptr<Buffer> indexBuffer) {
		commandBuffers.resize(renderSurface->getFramebuffers().size());

		VkCommandBufferAllocateInfo allocInfo {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

		if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers!");
		}

		for (size_t i = 0; i < commandBuffers.size(); i++) {
			VkCommandBufferBeginInfo beginInfo {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
				throw std::runtime_error("failed to begin recording command buffer!");
			}

			VkRenderPassBeginInfo renderPassInfo {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = renderPass->getHandle();
			renderPassInfo.framebuffer = renderSurface->getFramebuffers()[i]->getHandle();
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = renderSurface->getExtent();

			VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;

			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
												renderPipeline->getHandle());

			VkBuffer vertexBuffers[] = { vertexBuffer->getHandle() };
			VkDeviceSize offsets[] = { 0 };

			vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);
			vkCmdBindIndexBuffer(commandBuffers[i], indexBuffer->getHandle(), 0, VK_INDEX_TYPE_UINT16);

			vkCmdDrawIndexed(commandBuffers[i], indexBuffer->getCount(), 1, 0, 0, 0);

			vkCmdEndRenderPass(commandBuffers[i]);

			if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to record command buffer!");
			}
		}
	}

	uint32_t RenderingEngine::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) &&
					(memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		throw std::runtime_error("failed to find suitable memory type!");
	}

	void RenderingEngine::drawEditor(VkCommandBuffer commandBuffer, VkFramebuffer framebuffer,
																	 std::shared_ptr<RenderPass> renderPass,
																	 std::shared_ptr<RenderPipeline> renderPipeline,
																	 std::shared_ptr<Buffer> vertexBuffer,
																	 std::shared_ptr<Buffer> indexBuffer) {
		VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };

		VkRenderPassBeginInfo renderPassInfo {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPass->getHandle();
		renderPassInfo.framebuffer = framebuffer;
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = renderSurface->getExtent();
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderPipeline->getHandle());

		VkBuffer vertexBuffers[] = { vertexBuffer->getHandle() };
		VkDeviceSize offsets[] = { 0 };

		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(commandBuffer, indexBuffer->getHandle(), 0, VK_INDEX_TYPE_UINT16);

		vkCmdDrawIndexed(commandBuffer, indexBuffer->getCount(), 1, 0, 0, 0);

		vkCmdEndRenderPass(commandBuffer);

		renderSurface->frameReady(nullptr, nullptr);
		renderSurface->nextFrame();
	}

	void RenderingEngine::draw() {
		int currentFrame = renderSurface->getCurrentFrame();
		vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
		vkResetFences(device, 1, &inFlightFences[currentFrame]);
		uint32_t imageIndex =
				renderSurface->getSwapchainImageIndex(device, imageAvailableSemaphores[currentFrame]);

		if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
			vkWaitForFences(device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
		}
		imagesInFlight[imageIndex] = inFlightFences[currentFrame];

		VkSubmitInfo submitInfo {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

		VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		vkResetFences(device, 1, &inFlightFences[currentFrame]);
		if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
			throw std::runtime_error("failed to submit draw command buffer!");
		}

		renderSurface->frameReady(signalSemaphores, presentQueue);
		renderSurface->nextFrame();
	}

	RenderingEngine::RenderingEngine(RenderEngineCreateInfo createInfo) {
		this->createInfo = createInfo;
	}

	std::shared_ptr<RenderingEngine> RenderingEngine::create(RenderEngineCreateInfo createInfo) {
		std::shared_ptr<RenderingEngine> instance(new RenderingEngine(createInfo));

		_instance = instance;

		return _instance;
	}

	std::shared_ptr<RenderingEngine> RenderingEngine::getInstance() {
		if (_instance == NULL) {
			throw std::runtime_error("you did not initialized rendering engine");
		}

		return _instance;
	}

	RenderingEngine::~RenderingEngine() {
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
			vkDestroyFence(device, inFlightFences[i], nullptr);
		}
		renderSurface->cleanup(instance, device);
		vkDestroyDevice(device, nullptr);

		if (enableValidationLayers) {
			VkImport::destroyDebugUtilsMessenger(instance, debugMessenger, nullptr);
		}

		vkDestroyInstance(instance, nullptr);
		glfwTerminate();
	}
} // namespace lc
