#include "lcpch.h"

namespace lc {
	VKAPI_ATTR VkBool32 VKAPI_CALL
	vulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
											VkDebugUtilsMessageTypeFlagsEXT messageType,
											const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
		if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
			std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		}

		return VK_FALSE;
	}
}; // namespace lc
