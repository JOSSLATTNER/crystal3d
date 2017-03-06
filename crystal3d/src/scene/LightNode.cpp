#include "LightNode.h"

namespace Scene
{
	CrLightNode::CrLightNode(ELightType a_Type)
		: m_Type(a_Type)
	{
		CrAssert(a_Type != ELightType::Spot, "Spotlight not yet implemented.");

	}
	CrLightNode::~CrLightNode()
	{
	}

	void CrLightNode::Update(const float & a_DeltaTime)
	{
	}

	ELightType CrLightNode::GetType()
	{
		return m_Type;
	}
}