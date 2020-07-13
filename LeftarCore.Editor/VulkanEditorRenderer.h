#pragma once
#include "../LeftarCore.Graphics.Core/RenderingEngine.h"
#include <QVulkanWindowRenderer>

class VulkanWindow;

class VulkanRenderer : public QVulkanWindowRenderer {
	std::vector<std::shared_ptr<lc::Shader>> shaders;
	std::shared_ptr<lc::RenderPass> renderPass;
	std::shared_ptr<lc::RenderPipeline> pipeline;
	std::shared_ptr<lc::Buffer> vertexBuffer;
	std::shared_ptr<lc::Buffer> indexBuffer;

public:
	VulkanRenderer(VulkanWindow* w);

	void initResources() override;
	void initSwapChainResources() override;
	void releaseSwapChainResources() override;
	void releaseResources() override;

	void startNextFrame() override;

private:
	VulkanWindow* window;
};

class VulkanWindow : public QVulkanWindow, public lc::IRenderSurface {
	QVulkanInstance inst;

public:
	VulkanWindow(std::string windowTitle);
	QVulkanWindowRenderer* createRenderer() override;
	void setSize();
	virtual void frameReady(VkSemaphore signalSemaphores[], VkQueue presentQueue);
	virtual int getSwapchainImageIndex(VkDevice device, VkSemaphore semaphore);
	virtual void createSurface(VkInstance instance);
	virtual void initSwapchain(VkPhysicalDevice physicalDevice, VkDevice device,
														 lc::QueueFamilyIndices indices);
	virtual VkFormat getSwapchainImageFormat();
	virtual void createSwapchainImages(VkDevice device, uint32_t imageCount);
	virtual void initFramebuffers(std::shared_ptr<lc::RenderPass> renderPass);
	virtual void pollEvents();
	virtual bool isOpen();
	virtual void cleanup(VkInstance instance, VkDevice device);
};
