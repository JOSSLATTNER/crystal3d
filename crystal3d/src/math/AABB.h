#pragma once
#include "glm\GLM.h"

namespace Math
{
	struct AABB
	{
		glm::vec3 min;
		glm::vec3 max;
	};

	inline AABB CreateBoundingBox(glm::vec3* vertices, size_t size)
	{
		AABB b;
		b.max = vertices[0];
		b.min = vertices[0];

		for (uint32_t i = 1; i < size; ++i)
		{
			if (vertices[i].x < b.min.x) b.min.x = vertices[i].x;
			if (vertices[i].y < b.min.y) b.min.y = vertices[i].y;
			if (vertices[i].z < b.min.z) b.min.z = vertices[i].z;
			if (vertices[i].x > b.max.x) b.max.x = vertices[i].x;
			if (vertices[i].y > b.max.y) b.max.y = vertices[i].y;
			if (vertices[i].z > b.max.z) b.max.z = vertices[i].z;
		}
		return b;
	}
}