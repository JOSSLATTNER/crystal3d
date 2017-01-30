#include "DirectionalLightNode.h"

namespace Scene
{
	CrDirectionalLightNode::CrDirectionalLightNode()
	{
	}

	CrDirectionalLightNode::~CrDirectionalLightNode()
	{
	}

	ENodeType CrDirectionalLightNode::GetType()
	{
		return ENodeType::DIRECTIONAL_LIGHT_NODE;
	}
}