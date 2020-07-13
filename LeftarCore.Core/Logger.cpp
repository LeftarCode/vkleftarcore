#include "Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace lc {
	std::shared_ptr<spdlog::logger> Logger::instance;

	void Logger::init() {
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(
				std::make_shared<spdlog::sinks::basic_file_sink_mt>("leftarcore.log", true));

		logSinks[0]->set_pattern("%^[%T] %n: %v%$");
		logSinks[1]->set_pattern("[%T] [%l] %n: %v");

		instance = std::make_shared<spdlog::logger>("LEFTARCORE", begin(logSinks), end(logSinks));
		spdlog::register_logger(instance);
		instance->set_level(spdlog::level::trace);
		instance->flush_on(spdlog::level::trace);
	}

	std::shared_ptr<spdlog::logger> Logger::getInstance() {
		if (instance == nullptr) {
			Logger::init();
		}

		return instance;
	}
} // namespace lc
