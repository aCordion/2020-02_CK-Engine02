
#include "Precompiled.h"

Circle::Circle(const std::vector<Vector2> InVertices)
{
	Vector2 temp = Vector2::Zero;

	// ��ġ ������ �޾� �߽ɰ� �������� ���� ���ϴ� ���� ( ���� ������ �� )
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
