#pragma once
#include "core\Core.h"

namespace Graphics 
{
	class CrRendererContext
	{
	public:
		void* windowHandle;
		uint32_t viewportWidth;
		uint32_t viewportHeight;
	};
}
