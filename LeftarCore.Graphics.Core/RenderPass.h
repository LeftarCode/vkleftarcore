#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace lc {
	class RenderPass {
		VkRenderPass handle;

	public:
		VkRenderPass getHandle();
		RenderPass();
		~RenderPass();
	};
} // namespace lc
