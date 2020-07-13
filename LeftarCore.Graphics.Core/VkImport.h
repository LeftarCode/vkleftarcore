#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace lc {
	namespace VkImport {
		VkResult createDebugUtilsMessenger(VkInstance instance,
																			 const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
																			 const VkAllocationCallbacks* pAllocator,
																			 VkDebugUtilsMessengerEXT* pDebugMessenger);
		void destroyDebugUtilsMessenger(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
																		const VkAllocationCallbacks* pAllocator);
	} // namespace VkImport
} // namespace lc
