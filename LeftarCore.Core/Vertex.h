#pragma once
#include "Reflection.h"
#include <glm/glm.hpp>
#include <array>

namespace lc {
	struct Vertex {
		glm::vec3 position;
		glm::vec2 uv;
		glm::vec3 tangent;
		glm::vec3 normal;

		REFLECT()
	};
} // namespace lc
