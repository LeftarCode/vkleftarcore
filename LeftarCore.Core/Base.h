#pragma once
#include "Logger.h"
#include "Reflection.h"

#ifdef _DEBUG
	#define LC_DEBUG
#endif

#ifdef _WIN32
	#ifdef _WIN64
		#define LC_WINDOWS
	#else
		#error "Only x64 builds are supported"
	#endif
#elif
	#error "Only Windows x64 is supported"
#endif

#ifdef _DEBUG
	#ifdef LC_WINDOWS
		#define LC_DEBUGBREAK() __debugbreak()
	#endif
#else
	#define LC_DEBUGBREAK()
#endif

#define LC_ASSERT(c, ...)                                                                          \
	if (!c) {                                                                                        \
		LC_ERROR("Assertion Failed: {0}", __VA_ARGS__);                                                \
		LC_DEBUGBREAK();                                                                               \
	}

class MetaObject {
	bool isInitialized = false;
	bool isCleanedUp = false;
	std::string typeName = "MetaObject";

public:
	virtual void cleanup() = 0;
};

#define INIT_OBJECT(c)                                                                             \
	typeName = #c;                                                                                   \
	isInitialized = true;
#define CLEANUP_OBJECT()		isCleanedUp = true;
#define DESCTRUCTOR_BASE(c) LC_ASSERT(isCleanedup, "Object not cleaned up: ", #c);
#define CREATE_DESTRUCTOR(c)                                                                       \
	c::~c() {                                                                                        \
		DESCTRUCTOR_BASE(c)                                                                            \
	}
