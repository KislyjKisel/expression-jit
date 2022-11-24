#pragma once
#include <coroutine>

namespace evo::cli
{
	class CommandTask {
	public:
		struct promise_type {

			auto get_return_object() {
				return CommandTask(std::coroutine_handle<promise_type>::from_promise(*this));
			}

			auto initial_suspend() const {
				return std::suspend_never {};
			}

			auto final_suspend() const {
				return std::suspend_always {};
			}

			void unhandled_exception() const { }
			void return_void() { }
		};

		CommandTask(std::coroutine_handle<promise_type> h) : m_handle(h) { }
		CommandTask() : m_handle{} { }

		void resume() const {
			m_handle.resume();
		}

		bool done() const noexcept {
			return (!m_handle) || m_handle.done();
		}

		operator bool() const noexcept {
			return (bool)m_handle;
		}

		void destroy() const noexcept {
			m_handle.destroy();
		}

	private:
		std::coroutine_handle<promise_type> m_handle;
	};
}