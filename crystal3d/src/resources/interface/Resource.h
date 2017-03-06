#pragma once
#include "core\Core.h"

namespace Resources
{
	typedef uint64_t CrResourceHandle;

	class CrResource
	{
	public:
		CrResource();
		~CrResource();

		CrResourceHandle GetResourceHandle() const;
		void SetResourceHandle(const CrResourceHandle a_Handle);

	private:
		CrResourceHandle m_ResourceHandle;


	};
}