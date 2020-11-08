#include "Precompiled.h"

// 핵심 로직 
bool QuadTree::Insert(const std::string& InKey, const Rectangle& InBound)
{

	// 마지막 Depth까지 도달하면 자료구조에 추가하고 종료 { return true; }
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
	// 자식 노드를 만들어야 하는 경우 4개의 자식 노드를 새롭게 생성 => Split()
	// Split된 상태에서 FindSubTree 함수의 결과에 따라 처리를 다르게 진행
	// 1. 겹쳐 있으면 자신의 자료구조에 넣고 끝내기 ( return true; )
	// 2. 안겹쳐져 있으면 자식으로 내려보내기 ( childNode->Insert ( 재귀 호출 ) )

}

void QuadTree::Split()
{
	// SW, SE, NW, SE 네 개의 영역으로 쪼개고 이들의 바운딩 볼륨을 계산

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

	// 네 개의 자식 노드를 돌면서 완전 포함하는 자식 트리가 있는지 조사.
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

	// 서브 트리가 없을 경우
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