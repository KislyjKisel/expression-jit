#pragma once
#include "../types.h"

namespace evo::ogl
{
	struct StorageFlags {
		constexpr gl_bitfield_t value() const noexcept { return m_value; }

		static StorageFlags DynamicStorageBit();
		static StorageFlags MapReadBit();
		static StorageFlags MapWriteBit();
		static StorageFlags MapPersistentBit();	// requires either MapReadBit or MapWriteBit
		static StorageFlags MapCoherentBit();		// requires MapPersistentBit
		static StorageFlags ClientStorageBit();

		constexpr StorageFlags operator|(StorageFlags r) const noexcept {
			return StorageFlags(m_value | r.m_value);
		}

	private:
		gl_bitfield_t m_value;

		constexpr StorageFlags(gl_bitfield_t value) : m_value(value) { }
	};
}