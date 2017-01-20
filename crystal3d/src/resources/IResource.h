#pragma once
#include "core\Core.h"

namespace Resources
{
	struct ResourceCreateInfo {};

	struct IResource
	{
		virtual ~IResource() {}
		virtual void LoadFromFile(const std::string& a_File, ResourceCreateInfo* a_Info) = 0;
	};
}