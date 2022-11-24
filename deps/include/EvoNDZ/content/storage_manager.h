#pragma once

namespace evo::content
{
	class ContentStorageManager {
	public:
		ContentStorageManager(std::unordered_map<std::type_index, std::unique_ptr<ContentLoaderBase>>&& loaders) {
			for (auto& [k, v] : loaders) {
				m_storages.push_back(v->store());
			}
		}

	private:
		std::vector<std::unique_ptr<ContentStorageBase>> m_storages;
	};
}