#include "Precompiled.h"

Rectangle::Rectangle(const std::vector<Vector2> InVertices)
{
	Rectangle t;

	// ���� �����غ��ÿ�.
	for (auto iter = InVertices.begin(); iter != InVertices.end(); iter++)
	{
		t += (*iter);
	}
	Min = t.Min;
	Max = t.Max;
}