#pragma once
#include "core/Core.h"

namespace Resources
{
	namespace Import
	{
		class CrImportException
			: public CrException
		{
			using CrException::CrException;
		};

	}
}