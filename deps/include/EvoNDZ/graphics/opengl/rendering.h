#pragma once
#include "render_target.h"
#include "../../util/color.h"

namespace evo::ogl::rendering {
	extern RenderTarget* MainRenderTarget; //todo: needed? state should make this default

	void Initialize(RenderTarget*);
	// Don't clear color buffer
	void BeginDraw();
	// Clear color buffer with backgroundColor
	void BeginDraw(Color backgroundColor);
	void EndDraw();
}