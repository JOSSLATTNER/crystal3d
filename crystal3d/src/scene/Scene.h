#pragma once

#include "core\Core.h"
#include "SceneNode.h"
#include "graphics\interface\IRenderable.h"
#include "scripting\Script.h"
#include "resources\ResourceManager.h"

namespace Scene
{
	class CrScene
	{
	public:
		CrScene(const std::string& a_Script);
		~CrScene();
	
		void Initialize();
		void Update(const float& delta);
		std::vector<Graphics::IRenderable*>& GetRenderList();

		template<typename T>
		void AddNode(CrSceneNode* a_Node)
		{
			if (std::is_base_of<Graphics::IRenderable, T>)
			{
				auto renderObject = static_cast<Graphics::IRenderable*>(a_Node);
				m_RenderList.push_back(renderObject);
			}

			m_Nodes[typeid(T).hash_code()] = a_Node;
		}

		template<typename T>
		T* GetNode()
		{
			auto range = m_Nodes.equal_range(typeid(T).hash_code());
			return range.first->second;
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
		Resources::CrResourcePtr<Scripting::CrScript> m_Behaviour;

	};
}