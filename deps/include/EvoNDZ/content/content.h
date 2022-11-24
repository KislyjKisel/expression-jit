#pragma once
#include <unordered_map>
#include <unordered_set>
#include <typeindex>
#include <memory>
#include <span>
#include "../util/string_key.h"
#include "reference.h"
#include "archive.h"
#include "name.h"

namespace evo::content
{
	class StorageBase {
	public:
		virtual ~StorageBase() { }
	};

	template<typename T>
	class Storage : public StorageBase{
	public:
		Storage(std::vector<T>&& data) : m_data(std::move(data)) { }

	private:
		std::vector<T> m_data;
	};

	class LoadingManager;

	class LoaderBase {
	public:
		virtual void load(LoadingManager&, Name name, std::span<const std::byte>) = 0;
		virtual const void* pointer_to(const Name&) const = 0;
		virtual void resolve_pointers(LoadingManager&) = 0;
		virtual std::unique_ptr<StorageBase> store() = 0;

		virtual ~LoaderBase() { }
	};

	class StorageManager {
	public:
		StorageManager(std::unordered_map<std::type_index, std::unique_ptr<LoaderBase>>&&, std::vector<std::string>&&);

	private:
		std::vector<std::unique_ptr<StorageBase>> m_storages;
		std::vector<std::string> m_names;
	};

	template<typename T>
	class Loader : public LoaderBase {
	public:
		using PointerResolvingFn = void(*)(LoadingManager&, T&);
		using LoadingFn = T(*)(LoadingManager&, std::span<const std::byte>);

		Loader(LoadingFn loader, PointerResolvingFn pointerResolver)
			: m_loader(loader), m_pointerResolver(pointerResolver) { }

		void load(LoadingManager& lm, Name name, std::span<const std::byte> data) override {
			m_map.insert({ name, m_data.size() });
			m_data.push_back(m_loader(lm, data));
		}

		const void* pointer_to(const Name& name) const override {
			return (void*)( m_data.data() + m_map.at(name) );
		}

		void resolve_pointers(LoadingManager& m) override {
			m_data.shrink_to_fit();
			for (T& v : m_data) {
				m_pointerResolver(m, v);
			}
		}

		std::unique_ptr<StorageBase> store() override {
			return std::make_unique<Storage<T>>(std::move(m_data));
		}

	private:
		std::vector<T> m_data;
		std::unordered_map<Name, size_t, Name::Hasher> m_map;
		LoadingFn m_loader;
		PointerResolvingFn m_pointerResolver;
	};

	class LoadingManager {
	public:
		void load_archive(std::string_view archiveName, std::span<const std::byte> archive);
		std::string_view decl_name(std::string_view n);
		void resolve_pointers();

		template<typename T>
		void declare(archive::type_index_t type, typename Loader<T>::LoadingFn loadingFn,  typename Loader<T>::PointerResolvingFn resolvingFn) {
			m_loaderMap.insert({ typeid( T ), std::make_unique<Loader<T>>(loadingFn, resolvingFn) });
			m_typeMap.insert({ type, typeid( T ) });
		}

		template<typename T>
		void name_to_pointer(Name name, const T*& ptr) {
			ptr = static_cast<const T*>( m_loaderMap.at(typeid(T))->pointer_to(name) );
		}

		StorageManager store() {
			return StorageManager(std::move(m_loaderMap), std::move(m_names));
		}

	private:
		std::unordered_map<std::type_index, std::unique_ptr<LoaderBase>> m_loaderMap;
		std::unordered_map<archive::type_index_t, std::type_index> m_typeMap;
		std::vector<std::string> m_names;
	};
}