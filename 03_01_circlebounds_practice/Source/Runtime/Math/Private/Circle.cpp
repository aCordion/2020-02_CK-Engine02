
#include "Precompiled.h"

Circle::Circle(const std::vector<Vector2> InVertices)
{
	Vector2 temp = Vector2::Zero;

	// 위치 정보를 받아 중심과 반지름의 값을 구하는 로직 ( 직접 구현할 것 )
	for (auto iter = InVertices.begin(); iter != InVertices.end(); iter++)
	{
		temp += (*iter);
	}

	Center = temp / InVertices.size();

	Vector2 farOne = Center;
	float farDist = 0.0f;
	

	for (auto iter = InVertices.begin(); iter != InVertices.end(); iter++)
	{
		Vector2 t = farOne - (*iter);
		float curDist = sqrtf(t.Dot(farOne));

		farDist = sqrtf((farOne - t).SizeSquared());

		if (farDist < curDist)
		{
			farOne = (*iter);
			curDist = farDist;
		}
	}

	Radius = farDist;

}
