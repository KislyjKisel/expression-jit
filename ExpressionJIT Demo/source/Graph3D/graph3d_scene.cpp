#include "graph3d_scene.h"
#include <imgui/imgui.h>
#include <EvoNDZ/util/timer.h>
#include "../params.h"

namespace ed
{
	enum class G3d_InputField : size_t {
		Function, XMax, XMin, XStep, YMax, YMin, YStep
	};

	const char* G3d_InputText[][2] {
		{ "z(x, y)", "r(z, u)" },
		{ "X Max",	"Z Max" },
		{ "X Min", "Z Min" },
		{ "X Step", "Z Step" },
		{ "Y Max", "U Max" },
		{ "Y Min", "U Min" },
		{ "Y Step", "U Step" },
	};

	Vertex(*G3d_VertexFunction[2])( float, float, float ){  
		[](float x, float y, float z) -> Vertex { return { {x, z, y} };  },

		[](float z, float u, float r) -> Vertex { 
			evo::Vector3 n { std::cosf(u), std::sinf(u), 0.f };
			return { { r * n.x, z, r * n.y }, n };
		}
	};

	void Graph3dScene::initialize() {
		evo::input::InputMap::Current = &inputMap;
		
		camController.bindInput(inputMap);

		compiler.arg('x', 0, exprjit::DataType::Float);
		compiler.arg('y', 1, exprjit::DataType::Float);

		compiler.arg('z', 0, exprjit::DataType::Float);
		compiler.arg('u', 1, exprjit::DataType::Float);

		renderer = std::make_unique<Graph3dRenderer>();
		camController.initialize();
		frameTimer.reset();
	}
	
	void Graph3dScene::update() {
		float dt = frameTimer.time<double>();
		frameTimer.reset();

		camController.update(camera, dt);
	}

	void Graph3dScene::gui() {
		ImGui::SetNextWindowSize(ImVec2 { 290, 300 });
		ImGui::Begin("Graph3D");

		ImGui::Checkbox("Cylindric", &cylindric);

		ImGui::Text("Camera");
		ImGui::InputFloat("Move Speed", &camController.moveSpeed, 0, 0, FloatInputFormat);
		ImGui::InputFloat("Turn Speed", &camController.turnSpeed, 0, 0, FloatInputFormat);
		ImGui::Text("Press R to enable camera rotation.");
		ImGui::Text("WASD - camera movement, mouse - rotation.");
		ImGui::Separator();

		ImGui::InputDouble(G3d_InputText[(size_t)G3d_InputField::XMin][cylindric], &xmin, 0, 0, FloatInputFormat);
		ImGui::InputDouble(G3d_InputText[(size_t)G3d_InputField::XMax][cylindric], &xmax, 0, 0, FloatInputFormat);
		ImGui::InputDouble(G3d_InputText[(size_t)G3d_InputField::XStep][cylindric], &xstep, 0, 0, FloatInputFormat);
		ImGui::InputDouble(G3d_InputText[(size_t)G3d_InputField::YMin][cylindric], &ymin, 0, 0, FloatInputFormat);
		ImGui::InputDouble(G3d_InputText[(size_t)G3d_InputField::YMax][cylindric], &ymax, 0, 0, FloatInputFormat);
		ImGui::InputDouble(G3d_InputText[(size_t)G3d_InputField::YStep][cylindric], &ystep, 0, 0, FloatInputFormat);

		if (xmax < xstep + xmin) xmax = xmin + xstep;
		if (ymax < ystep + ymin) ymax = ymin + ystep;

		ImGui::Separator();

		static char func_src_buf[128]{};
		ImGui::InputText(G3d_InputText[(size_t)G3d_InputField::Function][cylindric], func_src_buf, sizeof(func_src_buf));

		static std::string perrtext;
		if (ImGui::Button("Build")) {
			perrtext.clear();
			delete function;
			try { 
				function = compiler.compile<double, double, double>(func_src_buf);
			}
			catch (exprjit::ParserException pe) {
				perrtext = pe.what();
				ImGui::OpenPopup("Parser Error");
				function = nullptr;
			}
			if(perrtext.empty()) buildGraph();
		}

		if (lastTriangulationTime != 0.0) {
			ImGui::Text("Evaluation Time: %8.5f", lastEvalTime);
			ImGui::Text("Triangulation Time: %8.5f", lastTriangulationTime - lastEvalTime);
			ImGui::Text("Total vertices: %5i", (int)vertexCount);
		}

		if (ImGui::BeginPopup("Parser Error")) {
			ImGui::Text(perrtext.c_str());
			if (ImGui::Button("OK")) 
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}

		ImGui::End();
	}

	void Graph3dScene::triangulateGraph(size_t n, size_t m) {
		indices.clear();
		auto index = [n, m](size_t i, size_t j) { return i * m + j; };

		evo::Vector3f normal(0, 1, 0);

		for (size_t i = cylindric ? 0 : 1; i < n; ++i) {
			size_t pi = i > 0 ? i - 1 : n - 1;
			size_t ci = i > 0 ? i : 0;
			for (size_t j = 0; j < m; ++j) {
				size_t p0 = index(ci, j), p1 = index(pi, j);
				if (j > 0) {
					size_t p2 = index(ci, j - 1);
					indices.push_back(p0);
					indices.push_back(p2);
					indices.push_back(p1);
				}
				if (j + 1 < m) {
					size_t p3 = index(pi, j + 1);
					indices.push_back(p0);
					indices.push_back(p1);
					indices.push_back(p3);
					evo::Vector3f a = vertices[p1].p - vertices[p0].p;
					evo::Vector3f b = vertices[p3].p - vertices[p0].p;

					normal = evo::Vector3f::Cross(b, a).normalized();
				}
				vertices[p0].n = normal;
			}
		}
	}



	void Graph3dScene::buildGraph() {
		evo::Timer timer;

		vertices.clear();
		size_t n = 1;
		size_t m = 0;
		double x, y;
		{
			x = xmin;
			y = ymin;
			while (x <= xmax) {
				vertices.push_back(G3d_VertexFunction[cylindric](x, y, (float)( *function )( x, y )));
				x += xstep;
				++m;
			}
		}
		y += ystep;
		while (y <= ymax) {
			x = xmin;
			while (x <= xmax) {
				vertices.push_back(G3d_VertexFunction[cylindric](x, y, (float)( *function )( x, y )));
				x += xstep;
			}
			y += ystep;
			++n;
		}

		lastEvalTime = timer.time<double>();
		vertexCount = vertices.size();

		triangulateGraph(n, m);
		lastTriangulationTime = timer.time<double>();
		renderer->data(vertices, indices);
	}
}