#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__NT__) || defined(__WIN32) && !defined(__CYGWIN__)
	#ifndef _WIN64
		#error 32-bit Windows isn't supported by Divine Galaxy Lib.
	#endif
	#define EVONDZ_TARGET_WINDOWS
#elif __APPLE__
	#include <TargetConditionals.h>
	#if TARGET_OS_IPHONE
		#define EVONDZ_TARGET_IPHONE
	#elif TARGET_OS_MAC
		#define	EVONDZ_TARGET_MAC
	#else
		#define EVONDZ_TARGET_UNKNOWN
	#endif
#elif __linux__
	#define EVONDZ_TARGET_LINUX
#else 
	#define EVONDZ_TARGET_UNKNOWN
#endif

#if !defined(EVONDZ_TARGET_WINDOWS)
	#error Unsupported target.
#endif

#include <bit>

namespace evo
{
	inline constexpr std::endian global_byte_order = std::endian::little;
}