//#pragma once
//#include <vector>
//#include <cstdint>
//#include <unordered_map>
//#include "loading.h"
//
//namespace evo::data {
//	template<typename TContent>
//	class ContentStorage {
//	public:
//		static uint16_t TypeID() {
//			return m_typeID;
//		}
//		static uint8_t LoadingPosition() {
//			return m_loadingPosition;
//		}
//		static void Store(const std::string& name, TContent&& content) {
//			EVO_ASSERT(!m_nameIndexMap.contains(name));
//			m_nameIndexMap.insert({ name, m_content.size() });
//			m_content.push_back(std::move(content));
//		}
//		static void Store(const std::string& name, TContent&& content, size_t& index) {
//			EVO_ASSERT(!m_nameIndexMap.contains(name));
//			m_nameIndexMap.insert({ name, m_content.size() });
//			index = m_content.size();
//			m_content.push_back(std::move(content));
//		}
//		static size_t GetIndexByName(const std::string& name) {
//			EVO_ASSERT(m_nameIndexMap.contains(name));
//			return m_nameIndexMap.at(name);
//		}
//		static const TContent& GetByName(const std::string& name) {
//			EVO_ASSERT(m_nameIndexMap.contains(name));
//			return m_content[m_nameIndexMap.at(name)];
//		}
//		static const TContent& GetByIndex(size_t index) {
//			return m_content[index];
//		}
//
//	private:
//		inline static std::vector<TContent> m_content;
//		inline static uint16_t m_typeID = UINT16_MAX;
//		inline static uint8_t m_loadingPosition;
//		inline static std::unordered_map<std::string, size_t> m_nameIndexMap;
//
//		ContentStorage() = delete; 
//		ContentStorage(const ContentStorage&) = delete;
//		ContentStorage& operator=(const ContentStorage&) = delete;
//
//		friend class Loading;
//	};
//
//}