#pragma once
#include <string_view>
#include <optional>
#include <concepts>
#include "command_group.h"
#include "command_task.h"

namespace evo::cli
{
	template<typename TContext>
	class WaitForInput;

	template<typename TContext>
	class CommandManager {
	public:
		void process(TContext* context, std::string_view line) {
			if (m_currentTask) {
				m_lastInput = line;
				m_currentTask.resume();
			}
			else {
				m_currentTask = m_mainGroup.invoke(context, line);
			}

			if (m_currentTask && m_currentTask.done()) {
				m_currentTask.destroy();
				m_currentTask = CommandTask();
			}
		}

		bool waiting() const noexcept {
			return m_currentTask;
		}
		
		CommandGroup<TContext>& main() noexcept {
			return m_mainGroup;
		}

		friend class WaitForInput<TContext>;

	private:
		CommandGroup<TContext> m_mainGroup;
		std::string_view m_lastInput;
		CommandTask m_currentTask;

		std::string_view lastInput() {
			return m_lastInput;
		}
	};
}