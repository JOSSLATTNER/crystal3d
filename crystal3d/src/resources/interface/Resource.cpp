#include "Resource.h"

namespace Resources
{
	CrResource::CrResource()
	{
	}

	CrResource::~CrResource()
	{
	}

	CrResourceHandle CrResource::GetResourceHandle() const
	{
		return m_ResourceHandle;
	}

	void CrResource::SetResourceHandle(const CrResourceHandle a_Handle)
	{
		m_ResourceHandle = a_Handle;
	}
}