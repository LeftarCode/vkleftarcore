#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace lc {
	enum BufferUsage {
		eTransferSrcBuffer = 0x00000001,
		eTransferDstBuffer = 0x00000002,
		eUniformTexelBuffer = 0x00000004,
		eStorageTexelBuffer = 0x00000008,
		eUniformBuffer = 0x00000010,
		eStorageBuffer = 0x00000020,
		eIndexBuffer = 0x00000040,
		eVertexBuffer = 0x00000080,
		eIndirectBuffer = 0x00000100,
		eShaderDeviceAddressBuffer = 0x00020000,
		eTransformFeedbackBuffer = 0x00000800,
		eTransformFeedbackCounteBufferr = 0x00001000,
		eConditionalRenderingBuffer = 0x00000200,
		eRaytracingBuffer = 0x00000400
	};

	enum BufferSharingMode {
		eExclusive = 0,
		eConcurrent = 1,
	};

	class Buffer {
		VkBuffer handle;
		VkDeviceMemory bufferMemory;
		unsigned int size;
		unsigned int count;
		void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
											VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	public:
		unsigned int getSize();
		unsigned int getCount();
		VkBuffer getHandle();
		void* map();
		void unmap();
		Buffer(void* data, size_t typeSize, size_t count, BufferUsage usage,
					 BufferSharingMode sharingMode = eExclusive);
		~Buffer();
	};
} // namespace lc
