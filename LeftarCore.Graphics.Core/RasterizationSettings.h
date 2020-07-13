#pragma once
#include "VertexLayoutBinding.h"
#include "../LeftarCore.Core/Vertex.h"

namespace lc {
	enum PrimitiveType {
		ePoint = 0,
		eLine = 1,
		eLineStrip = 2,
		eTriangle = 3,
		eTriangleStrip = 4,
		eTriangleFan = 5,
		eLineWithAdjacency = 6,
		eLineStripWithAdjacency = 7,
		eTriangleWithAdjacency = 8,
		eTriangleStripWithAdjacency = 9,
		ePatch = 10
	};

	enum PrimitiveMode { eFillMode = 0, eLineMode = 1, ePointMode = 2 };

	enum CullMode { eNone = 0, eFront = 1, eBack = 2, eBoth = 3 };

	enum FrontFace { eCounterClockwise = 0, eClockwise = 1 };

	struct RenderViewport {
		float x;
		float y;
		float width;
		float height;
		float minDepth;
		float maxDepth;
	};

	struct RasterizationSettings {
		PrimitiveType primitiveType;
		PrimitiveMode primitiveMode;
		CullMode cullMode;
		FrontFace frontFace;
		RenderViewport viewport;
		VertexLayoutBinding vertexLayoutBinding;
	};
} // namespace lc
