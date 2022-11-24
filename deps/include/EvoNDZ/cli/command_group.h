#pragma once
#include <string_view>
#include <coroutine>
#include <vector>
#include <string>
#include <map>
#include "command_task.h"
#include "util.h"

namespace evo::cli
{
	template<typename TContext>
	class CommandGroup {
	public:
		using CommandFunction = CommandTask ( TContext::* )( );

		CommandGroup() = default;
		CommandGroup(CommandGroup&&) = default;

		CommandGroup* group(std::string_view n) {
			auto it = m_map.find(n);
			if (it == m_map.end() || it->second < 0) return nullptr;
			return &m_groups[it->second];
		}

		void add(std::string_view funcName, CommandFunction func) { 
			int i = m_functions.size();
			m_map.insert({ std::string(funcName), ~i });
			m_functions.push_back(func);
		}

		void add(std::string_view groupName) {
			m_map.insert({ std::string(groupName), (int)m_groups.size() });
			m_groups.emplace_back();
		}

		CommandTask invoke(TContext* context, std::string_view line) {
			auto [cmd, arg] = split(line);
			auto it = m_map.find(cmd);
			if (it == m_map.end()) {
				return CommandTask();
			}
			if (it->second >= 0) {
				return m_groups[it->second].invoke(context, arg);
			}
			else {
				return ((*context).*m_functions[~it->second])();
			}
		}

	private:
		std::map<std::string, int, std::less<>> m_map;
		std::vector<CommandGroup> m_groups;
		std::vector<CommandFunction> m_functions;

		CommandGroup(const CommandGroup&) = delete;
		CommandGroup& operator=(const CommandGroup&) = delete;
	};
}