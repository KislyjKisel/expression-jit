#pragma once
#include <chrono>
#include "concepts.h"

namespace evo
{
	class Timer {
	public:
		Timer() : m_startPoint(std::chrono::high_resolution_clock::now()) { }

		template<typename TType, typename TUnits = std::ratio<1, 1>>
		inline auto duration() const {
			auto endPoint = std::chrono::high_resolution_clock::now();
			return std::chrono::duration<TType, TUnits>(endPoint - m_startPoint);
		}

		template<typename TType, typename TUnits = std::ratio<1, 1>>
		inline auto time() const {
			return duration<TType, TUnits>().count();
		}

		inline void reset() { 
			m_startPoint = std::chrono::high_resolution_clock::now(); 
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_startPoint;
	};  
}