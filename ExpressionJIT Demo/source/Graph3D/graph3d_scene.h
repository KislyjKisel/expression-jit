#pragma once
#include <vector>
#include <memory>
#include <EvoNDZ/math/vector3.h>
#include <EvoNDZ/app/scene.h>
#include <EvoNDZ/input/input.h>
#include <EvoNDZ/util/timer.h>
#include "graph3d_renderer.h"
#include "../vertex.h"
#include "../camera.h"
#include "../camera_controller.h"
#include "../expression_compiler.h"

namespace ed
{
	class Graph3dScene final : public evo::Scene {
	public:
		void initialize() override;
		void gui() override;
		void update() override;

		void terminate() override { 
			delete function;
		}

		void render() override {
			renderer->render(camera, camera.position());
		}

	private:
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		evo::input::InputMap inputMap;
		exprjit::Function<double(double, double)>* function = nullptr;
		Camera camera = Camera(evo::Vector3f(0, 0, 15));
		CameraController camController = CameraController(3, 1);
		std::unique_ptr<Graph3dRenderer> renderer = nullptr;
		ExpressionCompiler compiler;
		evo::Timer frameTimer;

		evo::Vector3f light = (evo::Vector3f{ 1, -1, -1 }).normalized();

		double xstep = 0.1;
		double ystep = 0.1;
		double xmin = -10;
		double xmax = 10; 
		double ymin = -10;
		double ymax = 10;
		bool cylindric = false;

		double lastEvalTime = 0.0;
		double lastTriangulationTime = 0.0;
		size_t vertexCount = 0;

		void buildGraph();
		void triangulateGraph(size_t n, size_t m);
	};
}