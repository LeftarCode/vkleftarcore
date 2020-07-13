#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <optional>
#include <iostream>
#include <stdexcept>

namespace lc {
	class QueueFamilyIndices {
	public:
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class VulkanUtilities {
	public:
		static VkPhysicalDevice pickPhysicalDevice(std::vector<VkPhysicalDevice> devices,
																							 VkSurfaceKHR surface,
																							 std::vector<const char*> requiredExtensions);
		static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
		static bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface,
																 std::vector<const char*> requiredExtensions);
		static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device,
																												 VkSurfaceKHR surface);
		static VkSurfaceFormatKHR
		chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		static bool checkDeviceExtensionSupport(VkPhysicalDevice device,
																						std::vector<const char*> deviceExtensions);
		static VkPresentModeKHR
		chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, int width,
																			 int height);
	};
} // namespace lc
