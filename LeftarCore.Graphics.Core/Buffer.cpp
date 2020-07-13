#include "Buffer.h"
#include "RenderingEngine.h"

namespace lc {

	unsigned int Buffer::getSize() {
		return size;
	}

	unsigned int Buffer::getCount() {
		return count;
	}

	VkBuffer Buffer::getHandle() {
		return handle;
	}

	void Buffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
														VkMemoryPropertyFlags properties, VkBuffer& buffer,
														VkDeviceMemory& bufferMemory) {
		VkBufferCreateInfo bufferInfo {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(RenderingEngine::getInstance()->getDeviceHandle(), &bufferInfo, nullptr,
											 &buffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to create buffer!");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(RenderingEngine::getInstance()->getDeviceHandle(), buffer,
																	&memRequirements);

		VkMemoryAllocateInfo allocInfo {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex =
				RenderingEngine::getInstance()->findMemoryType(memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(RenderingEngine::getInstance()->getDeviceHandle(), &allocInfo, nullptr,
												 &bufferMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate buffer memory!");
		}

		vkBindBufferMemory(RenderingEngine::getInstance()->getDeviceHandle(), buffer, bufferMemory, 0);
	}

	void Buffer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
		VkCommandBufferAllocateInfo allocInfo {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = RenderingEngine::getInstance()->getCommandPool();
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(RenderingEngine::getInstance()->getDeviceHandle(), &allocInfo,
														 &commandBuffer);

		VkCommandBufferBeginInfo beginInfo {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		VkBufferCopy copyRegion {};
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(RenderingEngine::getInstance()->getGraphicsQueue(), 1, &submitInfo,
									VK_NULL_HANDLE);
		vkQueueWaitIdle(RenderingEngine::getInstance()->getGraphicsQueue());

		vkFreeCommandBuffers(RenderingEngine::getInstance()->getDeviceHandle(),
												 RenderingEngine::getInstance()->getCommandPool(), 1, &commandBuffer);
	}

	void* Buffer::map() {
		void* pointer;
		vkMapMemory(RenderingEngine::getInstance()->getDeviceHandle(), bufferMemory, 0, size, 0,
								&pointer);

		return pointer;
	}

	void Buffer::unmap() {
		vkUnmapMemory(RenderingEngine::getInstance()->getDeviceHandle(), bufferMemory);
	}

	Buffer::Buffer(void* data, size_t typeSize, size_t count, BufferUsage usage,
								 BufferSharingMode sharingMode) {
		size = typeSize * count;
		this->count = count;
		VkDeviceSize bufferSize = typeSize * count;

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
								 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
								 stagingBuffer, stagingBufferMemory);

		void* pMem;
		vkMapMemory(RenderingEngine::getInstance()->getDeviceHandle(), stagingBufferMemory, 0,
								bufferSize, 0, &pMem);
		memcpy(pMem, data, (size_t)bufferSize);
		vkUnmapMemory(RenderingEngine::getInstance()->getDeviceHandle(), stagingBufferMemory);

		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage,
								 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, handle, bufferMemory);

		copyBuffer(stagingBuffer, handle, size);

		vkDestroyBuffer(RenderingEngine::getInstance()->getDeviceHandle(), stagingBuffer, nullptr);
		vkFreeMemory(RenderingEngine::getInstance()->getDeviceHandle(), stagingBufferMemory, nullptr);
	}

	Buffer::~Buffer() {
		vkDestroyBuffer(RenderingEngine::getInstance()->getDeviceHandle(), handle, nullptr);
		vkFreeMemory(RenderingEngine::getInstance()->getDeviceHandle(), bufferMemory, nullptr);
	}

} // namespace lc