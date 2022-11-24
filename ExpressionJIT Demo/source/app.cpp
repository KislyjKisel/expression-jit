#include <memory>
#include <EvoNDZ/app/application.h>
#include <EvoNDZ/app/window.h>
#include "demo_selection_scene.h"
#include "params.h"

int main() {
	evo::app::run(ed::WindowWidth, ed::WindowHeight, "ExpressionJIT Demo", evo::WindowFlags::None, std::make_unique<ed::DemoSelectionScene>());
	return 0;
}