#pragma once
#include <EvoNDZ/input/input.h>
#include "camera.h"

namespace ed
{
	class CameraController final {
	public:
		float moveSpeed;
		float turnSpeed;
		
		CameraController() = default;
		CameraController(float moveSpeed, float turnSpeed) : moveSpeed(moveSpeed), turnSpeed(turnSpeed) { }

		void bindInput(evo::input::InputMap& inputMap) {
			inputMap.simple_switch(0, 1, evo::input::Key::W, [this]() { m_moveForward = 1; }, [this]() { m_moveForward = 0; });
			inputMap.simple_switch(2, 3, evo::input::Key::A, [this]() { m_moveRight = -1; }, [this]() { m_moveRight = 0; });
			inputMap.simple_switch(4, 5, evo::input::Key::S, [this]() { m_moveForward = -1; }, [this]() { m_moveForward = 0; });
			inputMap.simple_switch(6, 7, evo::input::Key::D, [this]() { m_moveRight = 1; }, [this]() { m_moveRight = 0; });
			inputMap.simple_switch(8, 9, evo::input::Key::LeftShift, [this]() { m_turbo = true; }, [this]() { m_turbo = false; });
			inputMap.simple_key(10, evo::input::Key::R, true, [this]() { m_rotate = !m_rotate; m_rotate ? evo::input::lock_cursor() : evo::input::unlock_cursor();  });
		}

		void initialize() {
			evo::input::mouse_position(m_mouseX, m_mouseY);
		}

		void update(Camera& camera, float dt) {
			float ms = moveSpeed * dt;
			if (m_turbo) ms *= 3.0f;
			if (m_moveForward) camera.set_position(camera.position() + camera.forward() * ( m_moveForward * ms ));
			if (m_moveRight) camera.set_position(camera.position() + camera.right() * ( m_moveRight * ms ));

			if (m_rotate) {
				float rs = turnSpeed * dt;
				if (m_turbo) rs *= 2.0f;
				double lmx = m_mouseX;
				double lmy = m_mouseY;
				evo::input::mouse_position(m_mouseX, m_mouseY);
				double dmx = m_mouseX - lmx;
				double dmy = m_mouseY - lmy;
				camera.rotate(-rs * dmx, -rs * dmy, 0.0f);
			}
			else evo::input::mouse_position(m_mouseX, m_mouseY);
		}

	private:
		double m_mouseX = 0.0;
		double m_mouseY = 0.0;

		int m_moveForward = 0;
		int m_moveRight = 0;
		bool m_rotate = false;
		bool m_turbo = false;
	};
}