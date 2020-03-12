#pragma once
class CNode
{
public:
	CNode();
	CNode(int _x, int _y, CNode* _pParent, Coordinate _EndPoint);
	~CNode();

	Coordinate point;
	Coordinate end;
	float F, G, H; // F = 비용, G = 지난 거리, H = 남은 거리

	CNode* pParent;
};

