#pragma once
#include <vector>
#include <algorithm>
#include <concepts>
#include <functional>
#include "exception.h"

namespace evo
{
	template<typename... TArgs>
	class EventInterface;

	template<typename... TArgs>
	class Event {
	public:
		typedef std::function<void(TArgs...)> Callback;//void(*Callback)(TArgs...);

		Event() = default;
		Event(const Event& e) : m_callbacks(e.m_callbacks) { }
		Event(Event&& e) noexcept : m_callbacks(std::move(e.m_callbacks)) { }
		
		Event& operator=(const Event& e) {
			m_callbacks = e.m_callbacks;
		}

		Event& operator=(Event&& e) noexcept {
			m_callbacks = std::move(e.m_callbacks);
			return *this;
		}

		void operator+=(Callback callback) {
			m_callbacks.push_back(callback);
		}

		void operator-=(Callback callback) {
			auto it = std::find(m_callbacks.cbegin(), m_callbacks.cend(), callback);
			if (it != m_callbacks.cend())
				m_callbacks.erase(it);
		}

		template<typename... TPassedArgs>
		void operator()(TPassedArgs&&... args) 
			requires (std::same_as<std::decay_t<TArgs>, std::decay_t<TPassedArgs>> && ...)
		{
			for (size_t i = 0; i < m_callbacks.size(); ++i) {
				// todo: ? exception handling
				m_callbacks[i](std::forward<TPassedArgs>(args)...);
			}
		}

		void clear() {
			m_callbacks.clear();
		}

		void reserve(size_t cap) {
			m_callbacks.reserve(cap);
		}

		friend class EventInterface<TArgs...>;

	private:
		std::vector<Callback> m_callbacks;
	};

	template<typename... TArgs>
	class EventInterface {
	public:
		typedef Event<TArgs...> Event;

		EventInterface(Event& e) : m_event(e) { }

		void operator+=(typename Event::Callback c) {
			m_event += c;
		}
		void operator-=(typename Event::Callback c) {
			m_event -= c;
		}
		void clear() {
			m_event.clear();
		}
		void reserve() {
			m_event.reserve();
		}

	private:
		Event& m_event;
	};
}