#include "Vertex.h"

namespace lc {
	REFLECT_STRUCT_BEGIN(Vertex)
	REFLECT_STRUCT_MEMBER(position)
	REFLECT_STRUCT_MEMBER(uv)
	REFLECT_STRUCT_MEMBER(tangent)
	REFLECT_STRUCT_MEMBER(normal)
	REFLECT_STRUCT_END(Vertex)
} // namespace lc