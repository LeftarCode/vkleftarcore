#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "RenderPass.h"

namespace lc {
	class Framebuffer {
		VkFramebuffer handle;

	public:
		VkFramebuffer getHandle();
		Framebuffer(glm::vec2 size, std::vector<VkImageView> attachments,
								std::shared_ptr<RenderPass> renderPass);
		Framebuffer(VkDevice device, glm::vec2 size, std::vector<VkImageView> attachments,
								std::shared_ptr<RenderPass> renderPass);
		~Framebuffer();
	};
} // namespace lc
