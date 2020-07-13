#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <assert.h>
#include <iostream>

namespace lc {
	VKAPI_ATTR VkBool32 VKAPI_CALL
	vulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
											VkDebugUtilsMessageTypeFlagsEXT messageType,
											const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
};
