#pragma once
#include "core\Core.h"

namespace Resources
{
	namespace Import
	{
		class CrImportException
			: public CrException
		{
			using CrException::CrException;
		};

		struct CrImageInfo
		{
			unsigned int width;
			unsigned int height;
			bool hasAlpha;
		};
	}
}