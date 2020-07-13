#include "Framebuffer.h"
#include "RenderingEngine.h"

namespace lc {
	Framebuffer::Framebuffer(glm::vec2 size, std::vector<VkImageView> attachments,
													 std::shared_ptr<RenderPass> renderPass) {
		VkFramebufferCreateInfo framebufferInfo {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass->getHandle();
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = size.x;
		framebufferInfo.height = size.y;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(RenderingEngine::getInstance()->getDeviceHandle(), &framebufferInfo,
														nullptr, &handle) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer!");
		}
	}

	Framebuffer::Framebuffer(VkDevice device, glm::vec2 size, std::vector<VkImageView> attachments,
													 std::shared_ptr<RenderPass> renderPass) {
		VkFramebufferCreateInfo framebufferInfo {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass->getHandle();
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = size.x;
		framebufferInfo.height = size.y;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &handle) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer!");
		}
	}

	VkFramebuffer Framebuffer::getHandle() {
		return handle;
	}

	Framebuffer::~Framebuffer() {
		vkDestroyFramebuffer(RenderingEngine::getInstance()->getDeviceHandle(), handle, nullptr);
	}
} // namespace lc
