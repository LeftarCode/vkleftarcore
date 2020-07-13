#pragma once
#include "Reflection.h"

namespace lc {
	namespace reflect {
		struct TypeDescriptor_GlmVec2 : TypeDescriptor {
			TypeDescriptor_GlmVec2() : TypeDescriptor { "glm::vec2", sizeof(glm::vec2) } {}
		};

		template <>
		inline TypeDescriptor* getPrimitiveDescriptor<glm::vec2>() {
			static TypeDescriptor_GlmVec2 typeDesc;
			return &typeDesc;
		}

		struct TypeDescriptor_GlmVec3 : TypeDescriptor {
			TypeDescriptor_GlmVec3() : TypeDescriptor { "glm::vec3", sizeof(glm::vec3) } {}
		};

		template <>
		inline TypeDescriptor* getPrimitiveDescriptor<glm::vec3>() {
			static TypeDescriptor_GlmVec3 typeDesc;
			return &typeDesc;
		}

		struct TypeDescriptor_GlmVec4 : TypeDescriptor {
			TypeDescriptor_GlmVec4() : TypeDescriptor { "glm::vec4", sizeof(glm::vec4) } {}
		};

		template <>
		inline TypeDescriptor* getPrimitiveDescriptor<glm::vec4>() {
			static TypeDescriptor_GlmVec4 typeDesc;
			return &typeDesc;
		}
	} // namespace reflect
} // namespace lc
