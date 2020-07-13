#include "VulkanUtilities.h"

namespace lc {
	VkPhysicalDevice
	VulkanUtilities::pickPhysicalDevice(std::vector<VkPhysicalDevice> devices, VkSurfaceKHR surface,
																			std::vector<const char*> requiredExtensions) {
		VkPhysicalDevice bestDevice = nullptr;

		for (const auto& device : devices) {
			if (isDeviceSuitable(device, surface, requiredExtensions)) {
				bestDevice = device;
				break;
			}
		}

		if (bestDevice == nullptr) {
			throw std::runtime_error("failed to find a suitable GPU!");
		}

		return bestDevice;
	}

	QueueFamilyIndices VulkanUtilities::findQueueFamilies(VkPhysicalDevice device,
																												VkSurfaceKHR surface) {
		QueueFamilyIndices indices;

		std::optional<uint32_t> graphicsFamily;
		graphicsFamily = 0;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}

			if (presentSupport) {
				indices.presentFamily = i;
			}

			if (indices.isComplete()) {
				break;
			}

			i++;
		}

		return indices;
	}

	bool VulkanUtilities::isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface,
																				 std::vector<const char*> requiredExtensions) {
		QueueFamilyIndices indices = findQueueFamilies(device, surface);

		bool extensionsSupported = checkDeviceExtensionSupport(device, requiredExtensions);

		bool swapChainAdequate = false;
		if (extensionsSupported) {
			SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device, surface);
			swapChainAdequate =
					!swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		return indices.isComplete() && extensionsSupported && swapChainAdequate;
	}

	SwapChainSupportDetails VulkanUtilities::querySwapChainSupport(VkPhysicalDevice device,
																																 VkSurfaceKHR surface) {
		SwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount,
																								details.presentModes.data());
		}

		return details;
	}

	VkSurfaceFormatKHR VulkanUtilities::chooseSwapSurfaceFormat(
			const std::vector<VkSurfaceFormatKHR>& availableFormats) {
		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
					availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}
	}

	bool VulkanUtilities::checkDeviceExtensionSupport(VkPhysicalDevice device,
																										std::vector<const char*> deviceExtensions) {
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
																				 availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	VkPresentModeKHR VulkanUtilities::chooseSwapPresentMode(
			const std::vector<VkPresentModeKHR>& availablePresentModes) {
		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}
	VkExtent2D VulkanUtilities::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities,
																							 int width, int height) {
		if (capabilities.currentExtent.width != UINT32_MAX) {
			return capabilities.currentExtent;
		} else {
			VkExtent2D actualExtent = { width, height };

			actualExtent.width =
					std::max(capabilities.minImageExtent.width,
									 std::min(capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height =
					std::max(capabilities.minImageExtent.height,
									 std::min(capabilities.maxImageExtent.height, actualExtent.height));

			return actualExtent;
		}
	}
} // namespace lc
