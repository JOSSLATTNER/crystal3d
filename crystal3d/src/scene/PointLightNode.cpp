#include "PointLightNode.h"

namespace Scene
{
	CrPointLightNode::CrPointLightNode()
	{
	}

	CrPointLightNode::~CrPointLightNode()
	{
	}

	ENodeType CrPointLightNode::GetType()
	{
		return ENodeType::POINT_LIGHT_NODE;
	}

}