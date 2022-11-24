#pragma once
#include <numeric>
#include <cstdint>
#include <span>
#include "../data/array.h"

namespace evo::archive {
	using data_size_t = size_t;
	using type_index_t = uint16_t;
	using name_size_t = uint16_t;
	using entry_count_t = uint32_t;

	constexpr data_size_t entry_size(name_size_t nameSize, data_size_t dataSize) noexcept {
		return sizeof(type_index_t) + sizeof(name_size_t) + nameSize + sizeof(data_size_t) + dataSize;
	}

	struct EntryExternal {
		const std::byte* data;
		const std::byte* name;	
		data_size_t dataSize;
		name_size_t nameSize;
		type_index_t type;
	};

	struct EntryInternal {
		data_size_t dataOffset;
		data_size_t nameOffset;
		data_size_t dataSize;
		name_size_t nameSize;
		type_index_t type;
	};

	class EntryIterator {
	public:
		using difference_type = ptrdiff_t;
		using value_type = EntryInternal;
		using reference = const EntryInternal&;
		using pointer = const EntryInternal*;
		using iterator_category = std::forward_iterator_tag;

		EntryIterator() : m_pointer(nullptr), m_remaining(0), m_info{} {}

		static EntryIterator Begin(const std::byte* archive);
		static EntryIterator End() { return {}; }
	
		reference operator*() const { return m_info; }
		pointer operator->() const { return &m_info; }

		EntryIterator& operator++(); 
		EntryIterator operator++(int);

		bool operator==(const EntryIterator& it) const {
			return m_pointer == it.m_pointer;
		}
		bool operator!=(const EntryIterator& it) const {
			return m_pointer != it.m_pointer;
		}

	private:
		EntryInternal m_info;
		const std::byte* m_pointer;
		entry_count_t m_remaining;

		EntryIterator(const std::byte* pointer, entry_count_t remaining, data_size_t offset);
	};

	Array<std::byte> create(EntryExternal* entries, entry_count_t entryCount);
	Array<EntryInternal> entries(const std::byte* data, data_size_t size);
}