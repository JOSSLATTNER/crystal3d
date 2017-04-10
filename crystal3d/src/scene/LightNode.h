#pragma once
#include "scene\SceneNode.h"

namespace Scene
{
	enum class ELightType
	{
		Point,
		Directional,
		Spot
	};

	class CrLightNode
		: public CrSceneNode
	{
	public:
		CrLightNode(ELightType a_Type);

		//Inherited via CrSceneNode
		~CrLightNode() override;
		void Update(const float a_DeltaTime) override;

		ELightType GetType();
		//TODO: LIGHT VALUES

	private:
		ELightType m_Type;

	};
}
