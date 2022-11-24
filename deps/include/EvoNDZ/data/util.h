#pragma once
#include <limits>

namespace evo
{
	inline constexpr size_t dynamic_capacity = std::numeric_limits<size_t>::max();

	namespace detail
	{
		template<size_t Capacity>
		struct CapacityBase {
			constexpr size_t capacity() const noexcept {
				return m_capacity;
			}

		protected:
			inline static constexpr size_t m_capacity = Capacity;
		};

		template<>
		struct CapacityBase<dynamic_capacity> {
			constexpr CapacityBase(const size_t capacity) : m_capacity(capacity) { }

			constexpr size_t capacity() const noexcept {
				return m_capacity;
			}

		protected:
			size_t m_capacity;
		};
	}
}