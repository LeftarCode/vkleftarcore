#include "Shader.h"
#include "RenderingEngine.h"

namespace lc {
	Shader::Shader(std::string filePath, ShaderType type) {
		this->type = type;
		std::ifstream file(filePath.c_str(), std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			throw std::runtime_error("failed to open file!");
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);
		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		VkShaderModuleCreateInfo createInfo {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = buffer.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(buffer.data());

		if (vkCreateShaderModule(RenderingEngine::getInstance()->getDeviceHandle(), &createInfo,
														 nullptr, &handle) != VK_SUCCESS) {
			throw std::runtime_error("failed to create shader module!");
		}
	}

	VkShaderModule Shader::getHandle() {
		return handle;
	}

	ShaderType Shader::getShaderType() {
		return type;
	}

	Shader::~Shader() {
		vkDestroyShaderModule(RenderingEngine::getInstance()->getDeviceHandle(), handle, nullptr);
	}
} // namespace lc
