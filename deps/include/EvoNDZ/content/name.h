#pragma once
#include <string_view>
#include "../util/util.h"

namespace evo::content
{
	class Name {
	public:
		Name(std::string_view archive, std::string_view entry) : m_archive(archive), m_entry(entry) { }

		std::string_view archive() const { return { m_archive.data(), m_archive.size() }; }
		std::string_view entry() const { return { m_entry.data(), m_entry.size() }; }

		bool operator==(const Name& cn) const {
			return m_archive == cn.m_archive && m_entry == cn.m_entry;
		}

		bool operator!=(const Name& cn) const {
			return m_archive != cn.m_archive || m_entry != cn.m_entry;
		}

		struct Hasher {
			size_t operator()(const Name& ref) const noexcept {
				return hash_combine(
					std::hash<std::string_view>()( ref.archive() ),
					std::hash<std::string_view>()( ref.entry() ));
			}
		};

	private:
		std::string_view m_archive;
		std::string_view m_entry;
	};
}