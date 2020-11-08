#include "Precompiled.h"

// �ٽ� ���� 
bool QuadTree::Insert(const std::string& InKey, const Rectangle& InBound)
{

	// ������ Depth���� �����ϸ� �ڷᱸ���� �߰��ϰ� ���� { return true; }
	if (Level >= MaxLevel)
	{
		Nodes.push_back(TreeNode(InKey, InBound));
		return true;
	}
	else
	{
		//if (IsLeaf || SubTrees == nullptr)
		if (IsLeaf)
		{
			Split();
		}

		QuadTree* qt = FindSubTree(InBound);
		if (qt == nullptr)
		{
			Nodes.push_back(TreeNode(InKey, InBound));
			return true;
		}
		else
		{
			qt->Insert(InKey, InBound);
			return false;
			
		}

		return false;
	}
	// �ڽ� ��带 ������ �ϴ� ��� 4���� �ڽ� ��带 ���Ӱ� ���� => Split()
	// Split�� ���¿��� FindSubTree �Լ��� ����� ���� ó���� �ٸ��� ����
	// 1. ���� ������ �ڽ��� �ڷᱸ���� �ְ� ������ ( return true; )
	// 2. �Ȱ����� ������ �ڽ����� ���������� ( childNode->Insert ( ��� ȣ�� ) )

}

void QuadTree::Split()
{
	// SW, SE, NW, SE �� ���� �������� �ɰ��� �̵��� �ٿ�� ������ ���

	Vector2 center = Bound.Min + Bound.GetExtent();

	SubTrees[SubNames::TopLeft] = new QuadTree(Rectangle(Vector2(Bound.Min.X, center.Y), Vector2(center.X, Bound.Max.Y)), Level + 1);
	SubTrees[SubNames::TopRight] = new QuadTree(Rectangle(center, Bound.Max), Level + 1);
	SubTrees[SubNames::BottomRight] = new QuadTree(Rectangle(Vector2(center.X, Bound.Min.Y), Vector2(Bound.Max.X, center.Y)), Level + 1);
	SubTrees[SubNames::BottomLeft] = new QuadTree(Rectangle(Bound.Min, center), Level + 1);
	IsLeaf = false;
}

QuadTree* QuadTree::FindSubTree(const Rectangle& InBound)
{
	/*if (IsLeaf)
	{
		return nullptr;
	}*/

	// �� ���� �ڽ� ��带 ���鼭 ���� �����ϴ� �ڽ� Ʈ���� �ִ��� ����.
	/*for (QuadTree* subTree : SubTrees)
	{
		assert(subTree != nullptr);
		if (subTree->Contains(InBound))
		{
			return subTree;
		}
	}*/
	for (int i = 0; i < 4; i++)
	{
		if (SubTrees[i] == nullptr)
		{
			return nullptr;
		}
		if (SubTrees[i]->Contains(InBound))
		{
			return SubTrees[i];
		}
	}


	return nullptr;
}

void QuadTree::Query(const CK::Rectangle& InRectangleToQuery, std::vector<std::string>& InOutItems) const
{
	if (SubTrees)
	{
		for (int i = 0; i < 4; i++)
		{
			if (SubTrees[i]->Contains(InRectangleToQuery))
			{
				for (auto node : SubTrees[i]->Nodes)
				{
					InOutItems.push_back(node.NodeKey);
				}
			}
		}
	}
}

void QuadTree::Clear()
{

	// ���� Ʈ���� ���� ���
	if (SubTrees == nullptr)
	{
		return;
	}
	else
	{
		//delete SubTrees;
	}

}

bool QuadTree::Contains(const Rectangle& InBox) const
{
	return Bound.IsInside(InBox);
}