#pragma once
#include <utility>
#include <memory>
#include <bitset>
#include "../math/math.h"
#include "../util/concepts.h"

namespace evo::detail
{
	template<
		typename Key,
		typename Hash = std::hash<Key>, 
		typename Equal = std::equal_to<Key>, 
		typename KAlloc = std::allocator<Key>
	>
	class OpenHashTable {
		public:
#pragma region Typedefs

			using value_type = Key;
			using reference = value_type&;
			using const_reference = const value_type&;
			using pointer = value_type*;
			using const_pointer = const value_type*;
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using hasher = Hash;
			using key_equal = Equal;
			using allocator_type = KAlloc;

#pragma endregion

		private:
			inline constexpr static bool transparent = Transparent<Hash> && Transparent<Equal>;

			enum FieldState {
				Empty = 0b00, Grave = 0b01, Full = 0b10
			};

			struct Bucket {
				constexpr static size_type Size = 32;
				constexpr static size_type SizeBits = math::log2(Size);

				std::aligned_storage_t<sizeof(Key), alignof( Key )> data[Size];
				uint64_t bitv = 0;

				Key* at(size_type i) {
					return reinterpret_cast<Key*>( &data[i] );
				}

				FieldState state(size_type i) const {
					return FieldState(bitv >> ( i << 1u ) & 0b11u);
				}

				void set_state(size_type i, FieldState s) {
					uint64_t mask = 0b11ui64 << ( i << 1u );
					uint64_t sb = uint64_t(s) << ( i << 1u );
					bitv = ( bitv & ~mask ) | sb;
				}


				template<typename V> requires DecayConvertibleTo<V, Key>
					void insert(V&& value, size_type i) {
						std::construct_at(reinterpret_cast<Key*>( &data[i] ), std::forward<V>(value));
						set_state(i, FieldState::Full);
					}

					void erase(size_type i) {
						std::destroy_at<Key>(reinterpret_cast<Key*>( &data[i] ));
						set_state(i, FieldState::Grave);
					}

					void clear() {
						uint64_t m = FieldState::Full;
						for (size_type i = 0; i < Size; ++i, m <<= 2) {
							if (bitv & m) {
								std::destroy_at<Key>(reinterpret_cast<Key*>( &data[i] ));
							}
						}
						bitv = 0;
					}
			};

			static_assert( std::is_trivially_destructible_v<Bucket> );

			using BAlloc = typename std::allocator_traits<KAlloc>::template rebind_alloc<Bucket>;

#pragma region Iterators

			template<bool Const>
			struct iterator_impl {
			public:
				using value_type = typename OpenHashTable::value_type;
				using reference = std::conditional_t<Const, const value_type&, value_type&>;
				using pointer = std::conditional_t<Const, const value_type*, value_type*>;
				using iterator_category = std::bidirectional_iterator_tag;
				using difference_type = std::ptrdiff_t;

				iterator_impl() : m_table(nullptr), m_bi(0), m_ei(0) { }
				template<bool ConstOther>
				iterator_impl(const iterator_impl<ConstOther>& i) noexcept requires ( ConstOther&& Const ) || ( !ConstOther )
					: m_table(i.m_table), m_bi(i.m_bi), m_ei(i.m_ei) { }

				reference operator*() const {
					return *m_table->m_buckets[m_bi].at(m_ei);
				}
				pointer operator->() const {
					return m_table->m_buckets[m_bi].at(m_ei);
				}

				iterator_impl& operator++() {
					next();
					return *this;
				}
				iterator_impl operator++(int) {
					auto copy = *this;
					next();
					return copy;
				}
				iterator_impl& operator--() {
					prev();
					return *this;
				}
				iterator_impl operator--(int) {
					auto copy = *this;
					prev();
					return copy;
				}

				auto operator<=>(const iterator_impl& b) const noexcept {
					std::strong_ordering bord = m_bi <=> b.m_bi;
					return bord == std::strong_ordering::equal ? ( m_ei <=> b.m_ei ) : bord;
				}
				bool operator==(const iterator_impl& b) const noexcept {
					return m_table == b.m_table && m_bi == b.m_bi && m_ei == b.m_ei;
				}
				bool operator!=(const iterator_impl& b) const noexcept {
					return m_ei != b.m_ei || m_bi != b.m_bi || m_table != b.m_table;
				}

				friend class OpenHashTable;

			private:
				using table_ptr = std::conditional_t<Const, const OpenHashTable*, OpenHashTable*>;

				table_ptr m_table;
				size_t m_bi;
				size_t m_ei;

				void next(const Bucket* buckets, size_t bucketCount) {
					while (1) {
						++m_ei;
						if (m_ei >= Bucket::Size) {
							++m_bi;
							m_ei = 0;
						}
						if (m_bi >= bucketCount) return; // END
						if (buckets[m_bi].state(m_ei) == FieldState::Full) return; // FOUND
					}
				}
				void next() {
					next(m_table->m_buckets, m_table->m_bucketCount);
				}
				void prev() {
					std::ptrdiff_t bi = m_bi;
					std::ptrdiff_t ei = m_ei;
					while (1) {
						--ei;
						if (ei < 0) {
							--bi;
							ei = Bucket::Size - 1;
							if (bi < 0) {
								m_bi = bi;
								m_ei = ei;
								return; // OUT OF BOUNDS. UB (?)
							}
						}
						if (m_table->m_buckets[bi].state(ei) == FieldState::Full) {
							m_bi = bi;
							m_ei = ei;
							return; // FOUND
						}
					}
				}

				static iterator_impl begin(table_ptr table) {
					iterator_impl it(table);
					it.m_ei = -1;
					it.next();
					return it;
				}
				static iterator_impl end(table_ptr table) {
					iterator_impl it(table);
					it.m_bi = table->m_bucketCount;
					return it;
				}

				iterator_impl(table_ptr table) : m_table(table), m_bi(0), m_ei(0) { }
				iterator_impl(table_ptr table, size_t bi, size_t ei) : m_table(table), m_bi(bi), m_ei(ei) { }
			};
		public:

			using iterator = iterator_impl<false>;
			using const_iterator = iterator_impl<true>;
			using reverse_iterator = std::reverse_iterator<iterator>;
			using const_reverse_iterator = std::reverse_iterator<const_iterator>;

			iterator				begin() { return iterator::begin(this); }
			iterator				end() { return iterator::end(this); }
			reverse_iterator		rbegin() { return reverse_iterator(end()); }
			reverse_iterator		rend() { return reverse_iterator(begin()); }

			const_iterator			cbegin() const { return const_iterator::begin(this); }
			const_iterator			cend() const { return const_iterator::end(this); }
			const_reverse_iterator	crbegin() const { return const_reverse_iterator(cend()); }
			const_reverse_iterator	crend() const { return const_reverse_iterator(cbegin()); }

			const_iterator			begin() const { return cbegin(); }
			const_iterator			end() const { return cend(); }
			const_reverse_iterator	rbegin() const { return crbegin(); }
			const_reverse_iterator	rend() const { return crend(); }

#pragma endregion

#pragma region Constructors & Destructors & op=

			OpenHashTable(const OpenHashTable& other) {
				copy_(other);
			}

			OpenHashTable& operator=(const OpenHashTable& other) {
				destroy_();
				copy_(other);
				return *this;
			}

			OpenHashTable(OpenHashTable&& other) {
				move_(std::move(other));
			}

			OpenHashTable& operator=(OpenHashTable&& other) {
				destroy_();
				move_(std::move(other));
				return *this;
			}

			// Doesn't allocate.
			OpenHashTable(const Hash& hash = {}, const Equal& equal = {}, const KAlloc alloc = {}) noexcept
				: m_hash(hash), m_equal(equal), m_alloc(alloc), m_buckets(nullptr), m_bucketCount(0), m_buckBits(0), m_size(0) { }

			//! Requires bucketCount > 0.
			OpenHashTable(size_type bucketCount, const Hash& hash = {}, const Equal& equal = {}, const KAlloc alloc = {})
				: OpenHashTable(hash, equal, alloc), m_bucketCount(bucketCount) 		{
#ifndef NDEBUG
				if (bucketCount == 0) throw std::bad_alloc();
#endif
			/*if (m_bucketCount > ( 1ui64 << m_buckBits )) {
				++m_buckBits;
				m_bucketCount = 1ui64 << m_buckBits;
			}*/
				normalize_bucket_count_(m_bucketCount, m_buckBits);
				m_buckets = m_alloc.allocate(m_bucketCount);
				for (size_type i = 0; i < m_bucketCount; ++i) std::allocator_traits<BAlloc>::construct(m_alloc, m_buckets + i);
			}

			template<typename InputIt>
			OpenHashTable(InputIt first, InputIt last, const Hash& hash = {}, const Equal& equal = {}, const KAlloc alloc = {})
				: m_hash(hash), m_equal(equal), m_alloc(alloc), m_size(0), m_bucketCount(std::distance(first, last) << 1ui64) 		{
				normalize_bucket_count_(m_bucketCount, m_buckBits);
				m_buckets = m_alloc.allocate(m_bucketCount);
				for (size_t bi = 0; bi < m_bucketCount; ++bi) std::allocator_traits<BAlloc>::construct(m_alloc, m_buckets + bi);
				for (; first != last; ++first) {
					insert(*first);
				}
			}

			~OpenHashTable() {
				destroy_();
			}

#pragma endregion

#pragma region Observers

			size_t size() const noexcept {
				return m_size;
			}

			bool empty() const noexcept {
				return m_size == 0;
			}

			float load_factor() const noexcept {
				return m_bucketCount == 0 ? 0.f : (float)m_size / m_bucketCount;
			}

			float max_load_factor() const noexcept {
				return 0.5f;
			}

			hasher hash_function() const {
				return m_hash;
			}

			key_equal key_eq() const {
				return m_equal;
			}

			allocator_type get_allocator() const noexcept {
				return allocator_type(m_alloc);
			}

#pragma endregion

#pragma region Modifiers

			void clear() {
				for (size_t i = 0; i < m_bucketCount; ++i) m_buckets[i].clear();
			}

			template<bool Const>
			iterator_impl<Const> erase(iterator_impl<Const> pos) {
				m_buckets[pos.m_bi].erase(pos.m_ei);
				return ++pos;
			}

			template<bool Const>
			iterator_impl<Const> erase(const iterator_impl<Const>& first, const iterator_impl<Const>& last) {
				for (; first != last; ++first) erase(first);
				return first;
			}

			template<DecayConvertibleTo<Key> V>
			std::pair<iterator, bool> insert(V&& value) {
				if (m_bucketCount == 0 || m_size > ( m_bucketCount << ( Bucket::SizeBits - 1 ) )) grow_();
				size_type i = m_hash(value);
				size_type io = 0;
				std::pair<size_type, size_type> ip = index_pair_(i);
				while (m_buckets[ip.first].state(ip.second) == FieldState::Full) {
					if (m_equal(value, *m_buckets[ip.first].at(ip.second))) return std::make_pair(iterator(this, ip.first, ip.second), false);
					i += ++io;
					ip = index_pair_(i);
				}
				m_buckets[ip.first].insert(std::forward<V>(value), ip.second);
				++m_size;
				return std::make_pair(iterator(this, ip.first, ip.second), true);
			}


#pragma endregion

#pragma region Lookup

			size_type count(const Key& key) const {
				return contains(key);
			}

			template<typename V> requires transparent
				size_type count(const V& value) const {
				size_type c = 0;
				size_t i = m_hash(value);
				size_t io = 0;
				while (true) {
					std::pair<size_t, size_t> ip = index_pair_(i);
					FieldState st = m_buckets[ip.first].state(ip.second);
					if (st == FieldState::Empty) {
						return c;
					}
					if (st == FieldState::Full && m_equal(value, *m_buckets[ip.first].at(ip.second))) [[likely]] {
						++c;
					}
					i += ++io;
				}
			}

			const_iterator find(const Key& key) const {
				auto ip = find_(key);
				return const_iterator(this, ip.first, ip.second);
			}

			template<typename V> requires transparent
				const_iterator find(const V& value) const {
				auto ip = find_(value);
				return const_iterator(this, ip.first, ip.second);
			}

			iterator find(const Key& key) {
				auto ip = find_(key);
				return iterator(this, ip.first, ip.second);
			}

			template<typename V> requires transparent
				iterator find(const V& value) {
				auto ip = find_(value);
				return iterator(this, ip.first, ip.second);
			}

			bool contains(const Key& key) const {
				return find(key) != cend();
			}

			template<typename V> requires transparent
				bool contains(const V& value) const {
				return find(value) != cend();
			}

#pragma endregion

#pragma region Storage

			void rehash(size_type bucketCount) {
				if (( m_size << 1ui64 ) > bucketCount) bucketCount = m_size << 1ui64;

				/*size_type buckBits = math::log2(bucketCount);
				if (bucketCount > ( 1ui64 << buckBits )) {
					++buckBits;
					bucketCount = 1ui64 << buckBits;
				}*/
				size_type buckBits;
				normalize_bucket_count_(bucketCount, buckBits);

				if (m_bucketCount != 0) {
					auto it = begin();
					auto ed = end();
					size_type oldBuckCount = m_bucketCount;
					size_type oldBuckBits = m_buckBits;
					Bucket* oldBuckets = m_buckets;

					m_bucketCount = bucketCount;
					m_buckBits = buckBits;
					m_buckets = m_alloc.allocate(bucketCount);
					for (size_t bi = 0; bi < bucketCount; ++bi) std::allocator_traits<BAlloc>::construct(m_alloc, m_buckets + bi);

					for (; it != ed; it.next(oldBuckets, oldBuckCount)) {
						insert(std::move_if_noexcept(*oldBuckets[it.m_bi].at(it.m_ei)));
						if constexpr (!std::is_trivially_destructible_v<Key>) {
							std::destroy_at(oldBuckets[it.m_bi].at(it.m_ei));
						}
					}
					m_alloc.deallocate(oldBuckets, oldBuckCount);
				}
				else {
					m_bucketCount = bucketCount;
					m_buckBits = buckBits;
					m_buckets = m_alloc.allocate(bucketCount);
					for (size_t bi = 0; bi < bucketCount; ++bi) std::allocator_traits<BAlloc>::construct(m_alloc, m_buckets + bi);
				}
			}

			void reserve(size_type count) {
				rehash(count << 1ui64);
			}

			void swap(OpenHashTable& other) noexcept( std::allocator_traits<BAlloc>::is_always_equal::value
				&& std::is_nothrow_swappable_v<Hash>
				&& std::is_nothrow_swappable_v<Equal> ) 		{
				using std::swap;
				swap(m_buckets, other.m_buckets);
				swap(m_bucketCount, other.m_bucketCount);
				swap(m_buckBits, other.m_buckBits);
				swap(m_size, other.m_size);
				swap(m_hash, other.m_hash);
				swap(m_equal, other.m_equal);
				swap(m_alloc, other.m_alloc);
			}

#pragma endregion

		private:
			Bucket* m_buckets;
			size_type m_bucketCount;
			size_type m_buckBits;
			size_type m_size;
			[[no_unique_address]] Hash m_hash;
			[[no_unique_address]] Equal m_equal;
			[[no_unique_address]] BAlloc m_alloc;

			std::pair<size_t, size_t> index_pair_(size_t h) const {
				size_t iabs = mod2p(h, m_buckBits + Bucket::SizeBits);
				return { div2p(iabs, Bucket::SizeBits), mod2p(iabs, Bucket::SizeBits) };
			}

			void normalize_bucket_count_(size_type& bucketCount, size_type& buckBits) {
				buckBits = math::log2(bucketCount);
				size_type buckBitsNum = 1ui64 << buckBits;
				if (bucketCount > buckBitsNum) {
					++buckBits;
					bucketCount = buckBitsNum;
				}
			}

			template<typename V>
			std::pair<size_t, size_t> find_(const V& value) const {
				size_t i = m_hash(value);
				size_t io = 0;
				while (true) {
					std::pair<size_t, size_t> ip = index_pair_(i);
					FieldState st = m_buckets[ip.first].state(ip.second);
					if (st == FieldState::Empty) {
						return std::make_pair(m_bucketCount, 0ui64);
					}
					if (st == FieldState::Full && m_equal(value, *m_buckets[ip.first].at(ip.second))) [[likely]] {
						return ip;
					}
					i += ++io;
				}
			}

			void destroy_() {
				for (iterator it = begin(); it != end(); ++it) std::destroy_at<Key>(&*it);
				m_alloc.deallocate(m_buckets, m_bucketCount);
			}

			void copy_(const OpenHashTable& other) {
				m_bucketCount = other.m_bucketCount;
				m_buckBits = other.m_buckBits;
				m_size = other.m_size;
				m_hash = other.m_hash;
				m_equal = other.m_equal;
				m_alloc = other.m_alloc;
				m_buckets = m_alloc.allocate(m_bucketCount);
				for (size_t bi = 0; bi < m_bucketCount; ++bi) {
					std::allocator_traits<BAlloc>::construct(m_alloc, m_buckets + bi);
					for (size_t ei = 0; ei < Bucket::Size; ++ei) {
						std::construct_at<Key>(m_buckets[bi].at(ei), other.m_buckets[bi].at(ei));
					}
				}
			}

			void move_(OpenHashTable&& other) {
				m_bucketCount = other.m_bucketCount;
				m_buckBits = other.m_buckBits;
				m_size = other.m_size;
				m_buckets = other.m_buckets;
				m_hash = std::move(other.m_hash);
				m_equal = std::move(other.m_equal);
				m_alloc = std::move(other.m_alloc);
				other.m_bucketCount = 0;
				other.m_buckBits = 0;
				other.m_size = 0;
				other.m_buckets = nullptr;
			}

			void grow_() noexcept( std::is_nothrow_move_constructible_v<Key> || std::is_nothrow_copy_constructible_v<Key> ) {
				if (m_bucketCount != 0) {
					auto it = begin();
					auto ed = end();
					size_type oldBuckCount = m_bucketCount;
					size_type oldBuckBits = m_buckBits;
					Bucket* oldBuckets = m_buckets;

					m_bucketCount <<= 1;
					++m_buckBits;
					m_buckets = m_alloc.allocate(m_bucketCount);

					for (; it != ed; it.next(oldBuckets, oldBuckCount)) {
						insert(std::move_if_noexcept(*oldBuckets[it.m_bi].at(it.m_ei)));
						if constexpr (!std::is_trivially_destructible_v<Key>) {
							std::destroy_at(oldBuckets[it.m_bi].at(it.m_ei));
						}
					}
					m_alloc.deallocate(oldBuckets, oldBuckCount);
				}
				else {
					m_bucketCount = 1;
					m_buckBits = 0;
					m_buckets = m_alloc.allocate(1);
				}
			}
	};


	template<typename K, typename H, typename E, typename A>
	inline void swap(OpenHashTable<K, H, E, A>& a, OpenHashTable<K, H, E, A>& b) noexcept( noexcept( a.swap(b) ) ) {
		a.swap(b);
	}
}