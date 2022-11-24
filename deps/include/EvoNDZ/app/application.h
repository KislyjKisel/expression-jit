#pragma once

#include <memory>

#ifndef EVO_DISABLE_LOGGING
#include "../debug/logger.h"

#define EVO_LOG(...)		evo::app::log(evo::dbg::Logger::Severity::Info,		__VA_ARGS__)
#define EVO_LOG_WARN(...)	evo::app::log(evo::dbg::Logger::Severity::Warning,	__VA_ARGS__)
#define EVO_LOG_ERROR(...)	evo::app::log(evo::dbg::Logger::Severity::Error,	__VA_ARGS__)
#else
#define EVO_LOG(...)			((void)0)
#define EVO_LOG_WARN(...)	((void)0)
#define EVO_LOG_ERROR(...)	((void)0)
#endif

namespace evo
{
	class Scene;
	enum class WindowFlags;
}

namespace evo::ogl
{
	class State;
}

namespace evo::app
{
	void run(int windowWidth, int windowHeight, const char* title, WindowFlags, std::unique_ptr<evo::Scene>&&);
	void set_scene(std::unique_ptr<evo::Scene>&&);
	void exit();

	ogl::State* gl();

	void set_window_size(int width, int height);
	void set_window_title(const char* title);

	void window_size(int& width, int& height);
	void window_size(double& width, double& y);
	double aspect_ratio();

#ifndef DGL_DISABLE_LOGGING
	extern evo::dbg::Logger logger;

	template<typename... T>
	void log(evo::dbg::Logger::Severity sev, const T&... args) {
		logger.log(sev, args...);
	}
#endif
}