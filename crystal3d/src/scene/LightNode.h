#pragma once
#include "scene\SceneNode.h"

namespace Scene
{
	enum class ELightType
	{
		POINT_LIGHT,
		DIRECTIONAL_LIGHT,
		SPOT_LIGHT
	};

	class CrLightNode
		: public CrSceneNode
	{
	public:
		CrLightNode(ELightType a_Type);

		//Inherited via CrSceneNode
		~CrLightNode() override;
		ENodeType GetType() override;

	private:
		ELightType m_Type;

	};
}
