//#pragma once
//#include <functional>
//#include <filesystem>
//#include <unordered_map>
//#include "default_format.h"
//#include "archive.h"
//#include "../error.h"
//
//namespace evo::data {
//	class Texture2D;
//	class Mesh;
//
//	class Loading {
//	public:
//		static void LoadContent();
//
//		template<typename TContent>
//		static void CreateFileContentStorage(uint16_t typeID, std::function<TContent(unsigned char*, uint64_t)> loadf) {
//			EVO_ASSERT(ContentStorage<TContent>::m_typeID == UINT16_MAX);
//			ContentStorage<TContent>::m_loadingPosition = LoadingPosition[typeID];
//			ContentStorage<TContent>::m_typeID = typeID;
//			m_loadFunctions.insert({ typeID,
//				[loadf](std::string name, unsigned char* data, uint64_t length) {
//					ContentStorage<TContent>::Store(name, loadf(data, length));
//				}
//			});
//		}
//
//		static void CreateDefaultContentStorages();
//
//		static void TransferToGpuTexture2D(size_t& indexResult, const std::string& textureName);
//		static void TransferToGpuMesh(size_t& indexResult, const std::string& meshName);
//	private:
//		struct UnprocessedData;
//		typedef std::function<void(std::string, unsigned char*, uint64_t)> load_function_type;
//
//		static inline std::unordered_map<uint16_t, load_function_type> m_loadFunctions;
//
//		static void LoadSingle(const UnprocessedData& data);
//		static void LoadFile(std::filesystem::path path);
//		static void LoadDirectory(std::filesystem::path path);
//		static void ProcessData();
//	};
//}