#pragma once
#include "scripting\Script.h"
#include "scene\interface\IComponent.h"

namespace Scene
{
	namespace Components
	{
		class CrScriptComponent 
			: public IComponent
		{
		public:
			CrScriptComponent();

			// Inherited via IComponent
			~CrScriptComponent() override;
			void OnDetach() override;
			void OnAttach(CrSceneNode* a_Node) override;
			void Update(const float a_Delta) override;

			void SetScript(Scripting::CrScript* a_Script);

		private:
			Scripting::CrScript* m_Script;

		};
	}
}