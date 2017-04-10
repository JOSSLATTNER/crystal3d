#pragma once
#include "window\interface\IWindow.h"
#include "core\Core.h"

namespace Graphics 
{
	struct CrRendererContext
	{
		Window::IWindow* targetWindow;
		uint32_t viewportWidth;
		uint32_t viewportHeight;
	};
}
