#pragma once
#include <unordered_map>
#include <cstdint>
#include <utility>
#include <vector>
#include <span>
#include "../math/vector2.h"
#include "../util/event.h"
#include "key.h"

struct GLFWwindow;

namespace evo::input
{
	using KeyStatePair = std::pair<Key, bool>;
	using EventID = uint64_t;
}

namespace std
{
	template<>
	struct hash<evo::input::KeyStatePair> {
		size_t operator()(const evo::input::KeyStatePair& ks) const noexcept {
			return (size_t)ks.first | (size_t)ks.second << 32;
		}
	};
}

namespace evo::input
{
	class InputMap final {
	public:
		void add_key_event(EventID);
		void remove_key_event(EventID);
		EventInterface<> key_event(EventID);
		void bind(EventID, Key key, bool pressed);
		void unbind(EventID);

		void simple_key(EventID id, evo::input::Key k, bool press, std::function<void()> f);
		void simple_switch(EventID idY, EventID idN, evo::input::Key k, std::function<void()> fy, std::function<void()> fn);

		EventInterface<Key> any_key_event() { return m_anyKeyEvent; }
		EventInterface<double, double> scroll_event() { return m_scrollEvent; }

		//impl det
		void on_key(Key, bool pressed); 
		//impl det
		void on_scroll(double x, double y);

		inline static InputMap* Current = nullptr;

	private:
		std::unordered_map<EventID, size_t> m_idEventMap;
		std::unordered_map<KeyStatePair, size_t> m_keyEventMap;
		std::vector<Event<>> m_keyEvents;
		Event<double, double> m_scrollEvent;
		Event<Key> m_anyKeyEvent;
	};

	const char* key_name(Key) noexcept;

	void keyboard_callback(GLFWwindow*, int key, int, int action, int);
	void mouse_button_callback(GLFWwindow*, int btn, int act, int);
	void mouse_scroll_callback(GLFWwindow*, double x, double y);
	
	void update_joystick_input();
	std::span<const float> get_joystick_axes() noexcept;

	void lock_cursor();
	void unlock_cursor();
	void mouse_position(double& x, double& y);
	Vector2d mouse_position_normalized();
}