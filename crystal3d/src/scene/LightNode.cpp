#include "LightNode.h"

namespace Scene
{
	CrLightNode::CrLightNode(ELightType a_Type)
		: m_Type(a_Type)
	{
	}
	CrLightNode::~CrLightNode()
	{
	}

	ENodeType CrLightNode::GetType()
	{
		switch (m_Type)
		{

		}
	}
}