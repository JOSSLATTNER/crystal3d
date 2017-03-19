#include "Script.h"

namespace Scripting
{

	CrScript::CrScript(const std::string & a_File)
	{
		m_Script = m_State.load_file(a_File);
		CrFramework::RegisterTypes(m_State);

		CrLog("Script was loaded: %s", a_File.c_str());
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
