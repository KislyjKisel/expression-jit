#pragma once
#include <EvoNDZ/app/scene.h>

namespace ed
{
	class DemoSelectionScene final : public evo::Scene {
	public:

		void initialize() override { }
		void update() override { }
		void render() override { }
		void gui() override;
		void terminate() override { }
	};
}