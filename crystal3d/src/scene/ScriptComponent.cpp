#include "ScriptComponent.h"

namespace Scene
{
	namespace Components
	{

		CrScriptComponent::CrScriptComponent()
		{
		}

		CrScriptComponent::~CrScriptComponent()
		{
		}

		void CrScriptComponent::OnDetach()
		{
		}

		void CrScriptComponent::OnAttach(CrSceneNode* a_Node)
		{
			if (m_Script != nullptr)
			{
				m_Script->Run();
				m_Script->BindGlobal("this", a_Node);
				m_Script->Invoke<void>("Init");
			}
		}

		void CrScriptComponent::SetScript(Scripting::CrScript * a_Script)
		{
			CrAssert(a_Script != nullptr, "Script was nullptr!");
			m_Script = a_Script;
		}

		void CrScriptComponent::Update(const float a_Delta)
		{
			if (m_Script != nullptr)
			{
				m_Script->Invoke<void, float>("Update", a_Delta);
			}
		}

	}
}
