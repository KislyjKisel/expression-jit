#include "demo_selection_scene.h"
#include <imgui/imgui.h>
#include <iostream>
#include <EvoNDZ/app/application.h>

#include "Graph3D/graph3d_scene.h"
#include "Benchmark/benchmark_scene.h"

namespace ed
{
	template<typename TDemoScene>
	void select() {
		evo::app::set_scene(std::make_unique<TDemoScene>());
	}

	void DemoSelectionScene::gui() {
		ImGui::Begin("Demo Selection", 0, 
			ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);

		if (ImGui::Button("Graph3D"		)) select<Graph3dScene		>();
		if (ImGui::Button("Benchmark"	)) select<BenchmarkScene		>();
		
		ImGui::End();
	}
}