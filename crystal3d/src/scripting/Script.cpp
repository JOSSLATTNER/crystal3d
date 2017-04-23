#include "Script.h"
#include "core\Engine.h"

namespace Scripting
{
	CrScript::CrScript(const std::string& a_Source)
	{
		m_Script = m_State.load(a_Source);
		CrFramework::RegisterTypes(m_State);
	}

	CrScript::~CrScript()
	{
	}

	void CrScript::Run()
	{
		try
		{
			m_Script();
		}
		catch (sol::error err)
		{
			CrAssert(0, err.what());
		}
	}
}
