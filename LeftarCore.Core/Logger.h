#pragma once
#include <spdlog/spdlog.h>

namespace lc {
	class Logger {
		static std::shared_ptr<spdlog::logger> instance;

	public:
		static void init();
		static std::shared_ptr<spdlog::logger> getInstance();
	};

} // namespace lc

#define LC_TRACE(...) lc::Logger::getInstance()->trace(__VA_ARGS__)
#define LC_INFO(...)	lc::Logger::getInstance()->info(__VA_ARGS__)
#define LC_WARN(...)	lc::Logger::getInstance()->warn(__VA_ARGS__)
#define LC_ERROR(...) lc::Logger::getInstance()->error(__VA_ARGS__)
#define LC_FATAL(...) lc::Logger::getInstance()->critical(__VA_ARGS__)
