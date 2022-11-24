#pragma once
#include <coroutine>
#include "command_manager.h"

namespace evo::cli
{
	template<typename TContext>
	class WaitForInput {
	public:
		WaitForInput(CommandManager<TContext>* context) : m_manager(context) { }

		auto operator co_await() const noexcept {
			class awaiter {
			public:
				awaiter(CommandManager<TContext>* context) : m_manager(context) { }

				bool await_ready() const noexcept {
					return false;
				}

				std::string_view await_resume() noexcept {
					return m_manager->lastInput();
				}

				void await_suspend(std::coroutine_handle<>)  const noexcept { }

			private:
				CommandManager<TContext>* m_manager;
			};

			return awaiter(m_manager);
		}

	private:
		CommandManager<TContext>* m_manager;
	};
}