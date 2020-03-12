#include "stdafx.h"
#include "Node.h"


CNode::CNode()
{
}

CNode::CNode(int _x, int _y, CNode * _pParent, Coordinate _EndPoint)
{
	point.x = _x;
	point.y = _y;
	pParent = _pParent;
	end = _EndPoint;

	if (pParent == NULL) // 부모가 없는 경우
	{
		G = 0;
	}
	else if ( // 십자 방향인 경우
		(pParent->point.x == point.x - 1 && pParent->point.y == point.y) || // 부모가 '상'방향에 있거나
		(pParent->point.x == point.x + 1 && pParent->point.y == point.y) || // 부모가 '하'방향에 있거나
		(pParent->point.x == point.x && pParent->point.y == point.y - 1) || // 부모가 '좌'방향에 있거나
		(pParent->point.x == point.x && pParent->point.y == point.y + 1)) // 부모가 '우'방향에 있으면		
	{
		G = pParent->G + 10;
	}
	else if ( // 대각선 방향인 경우
		(pParent->point.x == point.x - 1 && pParent->point.y == point.y - 1) || // 부모가 '좌상'방향에 있거나
		(pParent->point.x == point.x - 1 && pParent->point.y == point.y + 1) || // 부모가 '우상'방향에 있거나
		(pParent->point.x == point.x + 1 && pParent->point.y == point.y - 1) || // 부모가 '좌하'방향에 있거나
		(pParent->point.x == point.x + 1 && pParent->point.y == point.y + 1)) // 부모가 '우하'방향에 있으면
	{
		G = pParent->G + 14;
	}
	else {
		cout << " 부모 설정 오류 " << endl;
		F = -100000;  H = -100000; G = -100000;
	}
	//남은거리   끝점-시작점 *10 
	H = (abs(end.x - point.x) + abs(end.y - point.y)) * 10;

	F = G + H;
	
}

CNode::~CNode()
{
}


