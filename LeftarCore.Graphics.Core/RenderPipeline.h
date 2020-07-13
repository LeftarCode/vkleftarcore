#pragma once
#include "Shader.h"
#include "RenderPass.h"
#include "RasterizationSettings.h"
#include <vector>

namespace lc {
	class RenderPipeline {
		VkPipelineLayout pipelineLayout;
		VkPipeline graphicsPipeline;

	public:
		VkPipeline getHandle();
		RenderPipeline(std::vector<std::shared_ptr<Shader>> shaders,
									 std::shared_ptr<RenderPass> renderPass, RasterizationSettings renderSettings);
		~RenderPipeline();
	};
} // namespace lc
