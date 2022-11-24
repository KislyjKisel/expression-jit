#pragma once
#include "open_hash_table.h"

namespace evo
{
	template<
		typename Key,
		typename Hash = std::hash<Key>, 
		typename Equal = std::equal_to<Key>,
		typename KAlloc = std::allocator<Key>
	>
	class OpenSet {
	public:
#pragma region Typedefs

		using value_type			= Key;
		using reference			= value_type&;
		using const_reference	= const value_type&;
		using pointer			= value_type*;
		using const_pointer		= const value_type*;
		using size_type			= std::size_t;
		using difference_type	= std::ptrdiff_t;
		using hasher				= Hash;
		using key_equal			= Equal;
		using allocator_type		= KAlloc;

#pragma endregion

		using table_type			= detail::OpenHashTable<Key, Hash, Equal, KAlloc>;
		inline constexpr static bool transparent = Transparent<Hash> && Transparent<Equal>;

	public:
#pragma region Iterators

		using iterator					= table_type::const_iterator;
		using const_iterator				= table_type::const_iterator;
		using reverse_iterator			= table_type::reverse_iterator;
		using const_reverse_iterator		= table_type::const_reverse_iterator;

		iterator			begin	() const { return m_table.cbegin(); }
		iterator			end		() const { return m_table.cend();	}
		const_iterator	cbegin	() const { return m_table.cbegin(); }
		const_iterator	cend		() const { return m_table.cend();	}

		reverse_iterator			rbegin	() const { return m_table.crbegin();		}
		reverse_iterator			rend		() const { return m_table.crend();		}
		const_reverse_iterator	crbegin	() const { return m_table.crbegin();		}
		const_reverse_iterator	crend	() const { return m_table.crend();		}

#pragma endregion

#pragma region Constructors & Destructors & op=

		OpenSet(const OpenSet& other) noexcept( std::is_nothrow_copy_constructible_v<table_type> ) : m_table(other.m_table) { }
		OpenSet& operator=(const OpenSet& other) { m_table = other.m_table; return *this; }
		OpenSet(OpenSet&& other) noexcept(std::is_nothrow_move_constructible_v<table_type>) : m_table(std::move(other.m_table)) { }
		OpenSet& operator=(OpenSet&& other) { m_table = std::move(other.m_table); return *this; }
		template<typename InputIt>
		OpenSet(InputIt first, InputIt last, const Hash& hash = {}, const Equal& equal = {}, const KAlloc alloc = {}) 
			: m_table(first, last, hash, equal, alloc) { }

		// Doesn't allocate.
		explicit OpenSet(const Hash& hash = {}, const Equal& equal = {}, const KAlloc alloc = {}) : m_table(hash, equal, alloc) { }
		explicit OpenSet(const KAlloc alloc) : m_table({}, {}, alloc) { }

		//! Requires bucketCount > 0.
		explicit OpenSet(size_t bucketCount, const Hash& hash = {}, const Equal& equal = {}, const KAlloc alloc = {})
			: m_table(bucketCount, hash, equal, alloc) { }

#pragma endregion

#pragma region Observers

		size_type		size				() const noexcept	{ return m_table.size();				}
		bool				empty			() const noexcept	{ return m_table.empty();			}
		float			load_factor		() const noexcept	{ return m_table.load_factor();		}
		float			max_load_factor	() const noexcept	{ return 0.5f;						}
		hasher			hash_function	() const				{ return m_table.hash_function();	}
		hasher			key_eq			() const				{ return m_table.key_eq();			}
		allocator_type	get_allocator	() const noexcept	{ return m_table.get_allocator();	}

#pragma endregion

#pragma region Modifiers
		void clear() { 
			m_table.clear(); 
		}

		const_iterator erase(const const_iterator& it) { 
			return const_iterator(m_table.erase(it)); 
		}

		const_iterator erase(const const_iterator& first, const const_iterator& last) { 
			return const_iterator(m_table.erase(first, last)); 
		}

		const_iterator erase(const Key& key) {
			return erase(find(key));
		}

		template<DecayConvertibleTo<Key> V>
		std::pair<const_iterator, bool> insert(V&& value) {
			auto ibp = m_table.insert(std::forward<V>(value));
			return { const_iterator(ibp.first), ibp.second };
		}

#pragma endregion

#pragma region Lookup

		size_type		count	(const Key& key) const { return m_table.count(key);		}	
		const_iterator	find		(const Key& key) const { return m_table.find(key);		}
		bool				contains(const Key& key) const { return m_table.contains(key);	}

		template<typename V> requires (transparent) 
		size_type		count		(const V& value) const { return m_table.count(value);		}
		template<typename V> requires (transparent)
		const_iterator	find			(const V& value) const { return m_table.find(value);			}
		template<typename V> requires (transparent) 
		bool				contains		(const V& value) const { return m_table.contains(value);		}

#pragma endregion

#pragma region Storage

		void rehash(size_t bucketCount) {
			m_table.rehash(bucketCount);
		}

		void reserve(size_type count) {
			m_table.reserve(count);
		}

		void swap(OpenSet& other) noexcept(noexcept(m_table.swap(other.m_table))) {
			m_table.swap(other.m_table);
		}

#pragma endregion

	private:
		table_type m_table;
	};

	template<typename K, typename H, typename E, typename A>
	inline void swap(OpenSet<K, H, E, A>& a, OpenSet<K, H, E, A>& b) noexcept(noexcept(a.swap(b))) {
		a.swap(b);
	}
}