#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <string>

namespace lc {
	enum ShaderType {
		eVertex = 0x00000001,
		eTesselationControll = 0x00000002,
		eTesselationEvaluation = 0x00000004,
		eGeometry = 0x00000008,
		eFragment = 0x00000010,
		eCompute = 0x00000020,
		eAllGraphics = 0x0000001F,
		eAll = 0x7FFFFFFF,
		SHADER_TYPE_COUNT = 5
	};

	class Shader {
		VkShaderModule handle;
		ShaderType type;

	public:
		VkShaderModule getHandle();
		ShaderType getShaderType();
		Shader(std::string filePath, ShaderType type);
		~Shader();
	};
} // namespace lc
