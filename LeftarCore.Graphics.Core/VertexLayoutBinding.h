#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "VertexInputLayout.h"
#include "../LeftarCore.Core/Vertex.h"
#include <vector>
#include <array>

namespace lc {
	class VertexLayoutBinding {
		std::array<VertexInputLayout, 4> inputs;

	public:
		VertexLayoutBinding() {
			inputs[0] = { eVec3, 0, offsetof(Vertex, position) };
			inputs[1] = { eVec2, 1, offsetof(Vertex, uv) };
			inputs[2] = { eVec3, 2, offsetof(Vertex, tangent) };
			inputs[3] = { eVec3, 3, offsetof(Vertex, normal) };
		};

		std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions() {
			std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions {};

			int index = 0;
			for (auto input : inputs) {
				attributeDescriptions[index].binding = 0;
				attributeDescriptions[index].location = input.inputIndex;
				attributeDescriptions[index].format = (VkFormat)input.inputType;
				attributeDescriptions[index].offset = input.inputOffset;

				index++;
			}

			return attributeDescriptions;
		};
	};
} // namespace lc
