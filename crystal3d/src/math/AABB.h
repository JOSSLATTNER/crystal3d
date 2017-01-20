#pragma once
#include "glm\GLM.h"

namespace Math
{
	struct AABB
	{
		glm::vec3 m_Min;
		glm::vec3 m_Max;
	};

	inline AABB CreateBoundingBox(glm::vec3* vertices, uint32_t size)
	{
		AABB b;
		b.m_Max = vertices[0];
		b.m_Min = vertices[0];

		for (uint32_t i = 1; i < size; ++i)
		{
			if (vertices[i].x < b.m_Min.x) b.m_Min.x = vertices[i].x;
			if (vertices[i].y < b.m_Min.y) b.m_Min.y = vertices[i].y;
			if (vertices[i].z < b.m_Min.z) b.m_Min.z = vertices[i].z;
			if (vertices[i].x > b.m_Max.x) b.m_Max.x = vertices[i].x;
			if (vertices[i].y > b.m_Max.y) b.m_Max.y = vertices[i].y;
			if (vertices[i].z > b.m_Max.z) b.m_Max.z = vertices[i].z;
		}
		return b;
	}
}