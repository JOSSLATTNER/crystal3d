#pragma once

#include "core\Core.h"
#include "SceneNode.h"
#include "graphics\interface\IRenderable.h"
#include "scripting\Script.h"

namespace Scene
{
	class CrScene
	{
	public:
		CrScene(const std::string& a_Script);
		~CrScene();
	
		void Initialize();
		void Update(const float delta);
		std::vector<Graphics::IRenderable*>& GetRenderList();

		template<typename T>
		void AddNode(T* a_Node)
		{
			Graphics::IRenderable* rn = dynamic_cast<Graphics::IRenderable*>(a_Node);
			if (rn != nullptr)
				m_RenderList.push_back(rn);

			size_t typeHash = typeid(T).hash_code();
			m_Nodes.insert({ typeHash, static_cast<Scene::CrSceneNode*>(a_Node) });
		}

		template<typename T>
		T* GetNode()
		{
			auto range = m_Nodes.equal_range(typeid(T).hash_code());
			return static_cast<T*>(range.first->second);
		}

		template<typename T>
		std::vector<T*> GetNodes()
		{
			auto range = m_Nodes.equal_range(typeid(T).hash_code());
			return{ range.first, range.second };
		}
		
	private:
		std::vector<Graphics::IRenderable*> m_RenderList;

		std::unordered_multimap<size_t, CrSceneNode*> m_Nodes;
		Scripting::CrScript m_Behaviour;

	};
}