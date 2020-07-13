#include "VulkanEditorRenderer.h"
#include <QVulkanFunctions>

VulkanWindow::VulkanWindow(std::string windowTitle) {
	this->setTitle(windowTitle.c_str());
}

QVulkanWindowRenderer* VulkanWindow::createRenderer() {
	return new VulkanRenderer(this);
}

void VulkanWindow::setSize() {
	QSize size = swapChainImageSize();

	IRenderSurface::size = { size.width(), size.height() };
	this->swapchainExtent = VkExtent2D { (uint32_t)size.width(), (uint32_t)size.height() };
}

void VulkanWindow::frameReady(VkSemaphore signalSemaphores[], VkQueue presentQueue) {
	QVulkanWindow::frameReady();
	requestUpdate();
}

int VulkanWindow::getSwapchainImageIndex(VkDevice device, VkSemaphore semaphore) {
	return currentSwapChainImageIndex();
}

void VulkanWindow::createSurface(VkInstance instance) {
	inst.setVkInstance(instance);
	if (!inst.create())
		qFatal("Failed to create Vulkan instance: %d", inst.errorCode());

	this->setVulkanInstance(&inst);
	this->show();

	this->surface = QVulkanInstance::surfaceForWindow(this);
}

void VulkanWindow::initSwapchain(VkPhysicalDevice physicalDevice, VkDevice device,
																 lc::QueueFamilyIndices indices) {
	createSwapchainImages(device, swapChainImageCount());
}

VkFormat VulkanWindow::getSwapchainImageFormat() {
	swapchainImageFormat = colorFormat();
	return swapchainImageFormat;
}

void VulkanWindow::createSwapchainImages(VkDevice device, uint32_t imageCount) {
	swapchainImageViews.clear();
	swapchainImages.clear();
	for (int i = 0; i < imageCount; i++) {
		swapchainImageViews.push_back(swapChainImageView(i));
		swapchainImages.push_back(swapChainImage(i));
	}
}

void VulkanWindow::initFramebuffers(std::shared_ptr<lc::RenderPass> renderPass) {
	for (size_t i = 0; i < swapchainImageViews.size(); i++) {
		std::vector<VkImageView> attachments = { swapchainImageViews[i] };

		framebuffers.push_back(std::shared_ptr<lc::Framebuffer>(new lc::Framebuffer(
				device(), { swapchainExtent.width, swapchainExtent.height }, attachments, renderPass)));
	}
}

void VulkanWindow::pollEvents() {}

bool VulkanWindow::isOpen() {
	return false;
}

void VulkanWindow::cleanup(VkInstance instance, VkDevice device) {}

VulkanRenderer::VulkanRenderer(VulkanWindow* w) : window(w) {}

void VulkanRenderer::initResources() {
	qDebug("initResources");
}

void VulkanRenderer::initSwapChainResources() {
	window->setSize();

	std::shared_ptr<lc::RenderingEngine> renderer = lc::RenderingEngine::getInstance();

	renderer->init(window->physicalDevice(), window->device(), window->graphicsQueue(),
								 window->graphicsCommandPool());
	renderer->initSwapchain();

	shaders.push_back(std::shared_ptr<lc::Shader>(
			new lc::Shader("./res/shaders/compiled/simple.vspv", lc::eVertex)));
	shaders.push_back(std::shared_ptr<lc::Shader>(
			new lc::Shader("./res/shaders/compiled/simple.fspv", lc::eFragment)));

	lc::RenderViewport viewport;
	viewport.x = 0;
	viewport.y = 0;
	viewport.width = window->getExtent().width;
	viewport.height = window->getExtent().height;
	viewport.minDepth = 0;
	viewport.maxDepth = 1.0f;

	lc::RasterizationSettings settings;
	settings.cullMode = lc::CullMode::eBack;
	settings.frontFace = lc::FrontFace::eClockwise;
	settings.primitiveMode = lc::PrimitiveMode::eFillMode;
	settings.primitiveType = lc::PrimitiveType::eTriangle;
	settings.viewport = viewport;

	renderPass = std::shared_ptr<lc::RenderPass>(new lc::RenderPass());
	pipeline =
			std::shared_ptr<lc::RenderPipeline>(new lc::RenderPipeline(shaders, renderPass, settings));

	const std::vector<lc::Vertex> vertexBufferData = {
		{ { -0.5f, -0.5f, 0.0f }, { 1, 0 }, { 0, 0, 0 }, { 0, 0, 0 } },
		{ { 0.5f, -0.5f, 0.0f }, { 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } },
		{ { 0.5f, 0.5f, 0.0f }, { 0, 1 }, { 0, 0, 0 }, { 0, 0, 0 } },
		{ { -0.5f, 0.5f, 0.0f }, { 1, 1 }, { 0, 0, 0 }, { 0, 0, 0 } },
	};

	const std::vector<uint16_t> indexBufferData = { 0, 1, 2, 2, 3, 0 };

	vertexBuffer = std::shared_ptr<lc::Buffer>(
			new lc::Buffer((void*)vertexBufferData.data(), sizeof(lc::Vertex), vertexBufferData.size(),
										 lc::BufferUsage::eVertexBuffer));
	indexBuffer = std::shared_ptr<lc::Buffer>(new lc::Buffer((void*)indexBufferData.data(),
																													 sizeof(uint16_t), indexBufferData.size(),
																													 lc::BufferUsage::eIndexBuffer));

	window->initFramebuffers(renderPass);
}

void VulkanRenderer::releaseSwapChainResources() {
	qDebug("releaseSwapChainResources");
}

void VulkanRenderer::releaseResources() {
	qDebug("releaseResources");
}

void VulkanRenderer::startNextFrame() {
	lc::RenderingEngine::getInstance()->drawEditor(window->currentCommandBuffer(),
																								 window->currentFramebuffer(), renderPass, pipeline,
																								 vertexBuffer, indexBuffer);
}
