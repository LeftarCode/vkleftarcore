#pragma once

namespace lc {
	enum VertexInputType { eScalar = 100, eVec2 = 103, eVec3 = 106, eVec4 = 109 };

	struct VertexInputLayout {
		VertexInputType inputType;
		unsigned int inputIndex;
		unsigned int inputOffset;
	};
} // namespace lc
