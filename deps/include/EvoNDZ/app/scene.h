#pragma once
#include "../util/compiler.h"

namespace evo
{
	class Scene abstract {
	public:
		virtual void initialize() = 0;
		virtual void update() = 0;
		virtual void render() = 0;
		virtual void gui() = 0;
		virtual void terminate() = 0;
		virtual ~Scene() noexcept { };
	};
}