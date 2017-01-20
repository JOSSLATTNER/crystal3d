#pragma once
#include "resources\Script.h"
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
			virtual void OnDetach() override;
			virtual void OnAttach(CrSceneNode* a_Node) override;
			void Update(float & a_Delta) override;

			void SetScript(Scripting::CrScript* a_Script);

		private:
			Scripting::CrScript* m_Script;

		};
	}
}