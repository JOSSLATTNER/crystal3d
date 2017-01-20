#pragma once
#include "core\Core.h"

namespace Window
{
	struct CrWindowContext
	{
		std::string title;
		uint32_t width;
		uint32_t height;
		bool fullscreen;
	};
}