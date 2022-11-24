#pragma once
#include <unordered_map>
#include <typeindex>
#include <vector>
#include <memory>

namespace evo::content
{
	class ContentStorageBase {
	public:
	};

	template<typename T>
	class ContentStorage {
	public:
		ContentStorage(std::vector<T>&& data) : m_data(std::move(data)) { }

	private:
		std::vector<T> m_data;
	};
}