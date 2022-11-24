//#pragma once
//#include "open_hash_table.h"
//
//namespace evo
//{
//	template<
//		typename Key, 
//		typename Value, 
//		typename Hasher = std::hash<Key>, 
//		typename KeyEqual = std::equal_to<Key>, 
//		typename APAllocator = std::allocator<std::pair<const Key, Value>>
//	>	
//	class OpenMap {
//	public:
//#pragma region Typedefs
//
//		using value_type = Key;
//		using reference = value_type&;
//		using const_reference = const value_type&;
//		using pointer = value_type*;
//		using const_pointer = const value_type*;
//		using size_type = std::size_t;
//		using difference_type = std::ptrdiff_t;
//		using hasher = Hash;
//		using key_equal = Equal;
//		using allocator_type = KAlloc;
//
//#pragma endregion
//
//	private:
//		struct KeyValuePair {
//
//		};
//
//		inline constexpr static bool transparent = Transparent<Hash> && Transparent<Equal>;
//		using kv_pair_type = KeyValuePair
//		using table_type = OpenHashTable<kv_pair_type, Hash, Equal, KAlloc>;
//	};
//}