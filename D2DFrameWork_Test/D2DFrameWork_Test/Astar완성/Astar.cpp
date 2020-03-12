#include "stdafx.h"
#include "Astar.h"
#include "Node.h"

CAstar::CAstar()
{
}

CAstar::CAstar(Coordinate _StartPoint, Coordinate _EndPoint)
{
	StartPoint.x = _StartPoint.x;
	StartPoint.y = _StartPoint.y;
	EndPoint.x = _EndPoint.x; 
	EndPoint.y = _EndPoint.y;
	FindPath();
}


CAstar::~CAstar()
{
	m_iter = path.begin();
	for (; m_iter != path.end(); m_iter++)
	{
		delete *m_iter;
	}
}

list<Coordinate*> CAstar::FindPath(CMyGrid * Navi, Coordinate StartPoint, Coordinate EndPoint)
{
	// (상,우,하,좌) 4방향 시계방향 탐색 후 결과에 따라 (우상,우하,좌하,좌상) 탐색.	
	list<CNode*> OpenNode; // 열린노드
	list<CNode*> CloseNode; // 닫힌노드
	CNode* SNode; // 선택된 노드
	list<Coordinate*> path;

	OpenNode.push_back(new CNode(StartPoint.x, StartPoint.y, NULL, EndPoint)); // 시작지점을 열린노드에 추가

	list<CNode*>::iterator iter;

	// 열린 노드가 비거나(열린노드의 시작==끝) 
	// 목적지에 도착(열린노드에서 값이 발견)한 경우 끝내야함
	// 즉 조건은 반대로 '열린 노드에 내용이 있거나 목적지를 못 찾은 경우' 반복
	while ((OpenNode.end() != OpenNode.begin())
		&& (OpenNode.end() == FindCoordNode(EndPoint.x, EndPoint.y, &OpenNode)))
	{
		iter = FindNextNode(&OpenNode); // 열린노드 중 F값이 제일 작은 노드의 주소를 찾아서 iter 에 저장
		SNode = *iter; // 열린노드 중 F값이 제일 작은 노드를 SNode에 저장

					   // 선택된 SNode 주변의 8방향 노드 탐색, 값이 수정될 수 있는 것은 열린 노드 뿐이므로 열린 노드는 주소를 전달.
		ExploreNode(Navi, SNode, &OpenNode, &CloseNode, EndPoint);

		CloseNode.push_back(SNode); // 현재 탐색한 노드를 닫힌 노드에 추가
		OpenNode.erase(iter); // 닫힌 노드에 추가한 노드를 열린 노드에서 제거
	}

	if ((OpenNode.end() != OpenNode.begin())) // 길을 찾은 경우
	{
		iter = FindCoordNode(EndPoint.x, EndPoint.y, &OpenNode); // 목적지의 노드를 찾아서 iter에 저장
		for (SNode = *iter; SNode->pParent != NULL; SNode = SNode->pParent)  // 부모가 NULL일 때까지 path에 경로 저장
		{
			path.push_back(new Coordinate(SNode->point.x, SNode->point.y));
		}	path.push_back(new Coordinate(SNode->point.x, SNode->point.y)); // 부모가 NULL인 경우의 path까지 저장(출발 지점)

		path.reverse(); // 목적지점으부터 역순으로 입력했으므로 다시 역순으로 뒤집어 출발지점이 첫 번째가 되도록 함.

						// 길을 찾은 경우 동적할당 해제
		iter = OpenNode.begin();
		for (; iter != OpenNode.end(); iter++)
		{
			delete *iter; // 열린 노드 동적할당 해제
		}
		iter = CloseNode.begin();
		for (; iter != CloseNode.end(); iter++)
		{
			delete *iter; // 닫힌 노드 동적할당 해제
		}

		return path; // 길을 찾은 경우 리턴
	}

	// 길을 찾지 못한 경우 동적할당 해제
	iter = CloseNode.begin();
	for (; iter != CloseNode.end(); iter++)
	{
		delete *iter; // 닫힌 노드 동적할당 해제
	}
	return path; // 길을 찾지 못한 경우 리턴
}

list<CNode*>::iterator CAstar::FindNextNode(list<CNode*>* pOpenNode)
{
	list<CNode*>::iterator iter = (*pOpenNode).begin();

	int minValue = 2000000000; // 현재 제일 작은 값을 저장
	int order = 0; // 가장작은 비용의 iterator를 얻기위한값

	for (int i = 1; iter != (*pOpenNode).end(); i++, iter++)
	{
		if ((*iter)->F <= minValue) // 비용이 작은경우 
		{
			minValue = (*iter)->F;
			order = i;
		}
	}

	iter = (*pOpenNode).begin();
	for (int i = 1; i < order; i++) 
	{
		iter++;
	}

	return iter;
}

list<CNode*>::iterator CAstar::FindCoordNode(int x, int y, list<CNode*>* NodeList)
{
	list<CNode*>::iterator iter = NodeList->begin();

	for (int i = 1; iter != NodeList->end(); i++, iter++)
	{
		if ((*iter)->point.x == x && (*iter)->point.y == y)
		{
			return iter;
		}
	}

	return NodeList->end();
}

void CAstar::ExploreNode(CMyGrid * Navi, CNode * SNode, list<CNode*>* OpenNode, list<CNode*>* CloseNode, Coordinate EndPoint)
{
	bool up = true, right = true, down = true, left = true; // 이 결과에 따라 대각선 방향 탐색 여부를 결정. true == 장애물 있음, false == 없음
	list<CNode*>::iterator iter;
	POINT point;

	// '상' 방향 탐색
	point = { SNode->point.x - 1 ,SNode->point.y };
	if (SNode->point.x > 0 && Navi->map[point.x][point.y] == 0) // '상' 방향에 맵이 존재하고 장애물이 없을 경우
	{
		// 장애물이 없는 경우에 해당하므로 장애물 false 세팅
		up = false;

		// 이미 열린 노드에 있는 경우
		if (OpenNode->end() != FindCoordNode(point.x, point.y, OpenNode))
		{
			iter = FindCoordNode(point.x, point.y, OpenNode);
			if ((*iter)->G > (SNode->G + 10)) // 원래 부모를 통해서 갔을 때의 비용보다 현재 노드를 통해서 갔을 때 비용이 더 낮아질 경우 
			{
				(*iter)->pParent = SNode; // 현재 노드를 부모로 바꿈
			}
		}

		// 닫힌 노드에 있는 경우
		else if (CloseNode->end() != FindCoordNode(point.x, point.y, CloseNode))
		{
		}

		// 상방향에 장애물이 없고 열린 노드 및 닫힌 노드에 추가되어있지 않은 경우
		// 상방향 노드를 열린 노드에 추가, 부모는 현재 탐색 노드로 지정.
		else
		{
			OpenNode->push_back(new CNode(point.x, point.y, SNode, EndPoint));
		}
	}
	// '우' 방향 탐색
	point = { SNode->point.x,SNode->point.y + 1 };
	if (SNode->point.y < (Navi->sizeY - 1) && Navi->map[point.x][point.y] == 0) // '우' 방향에 맵이 존재하고 장애물이 없을 경우
	{
		// 장애물이 없는 경우에 해당하므로 장애물 false 세팅
		right = false;

		// 이미 열린 노드에 있는 경우
		if (OpenNode->end() != FindCoordNode(point.x, point.y, OpenNode))
		{
			iter = FindCoordNode(point.x, point.y, OpenNode);
			if ((*iter)->G > (SNode->G + 10)) // 원래 부모를 통해서 갔을 때의 비용보다 현재 노드를 통해서 갔을 때 비용이 더 낮아질 경우 
			{
				(*iter)->pParent = SNode; // 현재 노드를 부모로 바꿈
			}
		}

		// 닫힌 노드에 있는 경우
		else if (CloseNode->end() != FindCoordNode(point.x, point.y, CloseNode))
		{
		}

		// 상방향에 장애물이 없고 열린 노드 및 닫힌 노드에 추가되어있지 않은 경우
		// 상방향 노드를 열린 노드에 추가, 부모는 현재 탐색 노드로 지정.
		else
		{
			OpenNode->push_back(new CNode(point.x, point.y, SNode, EndPoint));
		}
	}
	// '하' 방향 탐색
	point = { SNode->point.x + 1,SNode->point.y };
	if (SNode->point.x < (Navi->sizeX - 1) && Navi->map[point.x][point.y] == 0) // '하' 방향에 맵이 존재하고 장애물이 없을 경우
	{
		// 장애물이 없는 경우에 해당하므로 장애물 false 세팅
		down = false;

		// 이미 열린 노드에 있는 경우
		if (OpenNode->end() != FindCoordNode(point.x, point.y, OpenNode))
		{
			iter = FindCoordNode(point.x, point.y, OpenNode);
			if ((*iter)->G > (SNode->G + 10)) // 원래 부모를 통해서 갔을 때의 비용보다 현재 노드를 통해서 갔을 때 비용이 더 낮아질 경우 
			{
				(*iter)->pParent = SNode; // 현재 노드를 부모로 바꿈
			}
		}

		// 닫힌 노드에 있는 경우
		else if (CloseNode->end() != FindCoordNode(point.x, point.y, CloseNode))
		{
		}

		// 상방향에 장애물이 없고 열린 노드 및 닫힌 노드에 추가되어있지 않은 경우
		// 상방향 노드를 열린 노드에 추가, 부모는 현재 탐색 노드로 지정.
		else
		{
			OpenNode->push_back(new CNode(point.x, point.y, SNode, EndPoint));
		}
	}
	// '좌' 방향 탐색
	point = { SNode->point.x,SNode->point.y - 1 };
	if (SNode->point.y > 0 && Navi->map[point.x][point.y] == 0) // '좌' 방향에 맵이 존재하고 장애물이 없을 경우
	{
		// 장애물이 없는 경우에 해당하므로 장애물 false 세팅
		left = false;

		// 이미 열린 노드에 있는 경우
		if (OpenNode->end() != FindCoordNode(point.x, point.y, OpenNode))
		{
			iter = FindCoordNode(point.x, point.y, OpenNode);
			if ((*iter)->G > (SNode->G + 10)) // 원래 부모를 통해서 갔을 때의 비용보다 현재 노드를 통해서 갔을 때 비용이 더 낮아질 경우 
			{
				(*iter)->pParent = SNode; // 현재 노드를 부모로 바꿈
			}
		}

		// 닫힌 노드에 있는 경우
		else if (CloseNode->end() != FindCoordNode(point.x, point.y, CloseNode))
		{
		}

		// 상방향에 장애물이 없고 열린 노드 및 닫힌 노드에 추가되어있지 않은 경우
		// 상방향 노드를 열린 노드에 추가, 부모는 현재 탐색 노드로 지정.
		else
		{
			OpenNode->push_back(new CNode(point.x, point.y, SNode, EndPoint));
		}
	}

	// '우상' 방향 탐색
	//point = { SNode->point.x - 1,SNode->point.y + 1 };
	//if (SNode->point.x > 0 &&
	//	SNode->point.y < (Navi->sizeY - 1) &&
	//	Navi->map[point.x][point.y] == 0 &&
	//	up == false && right == false) // '우상' 방향에 맵이 존재하고 장애물이 없으며, 우방향과 상방향에도 장애물이 없을 경우
	//{
	//	// 이미 열린 노드에 있는 경우
	//	if (OpenNode->end() != FindCoordNode(point.x, point.y, OpenNode))
	//	{
	//		iter = FindCoordNode(point.x, point.y, OpenNode);
	//		if ((*iter)->G > (SNode->G + 14)) // 원래 부모를 통해서 갔을 때의 비용보다 현재 노드를 통해서 갔을 때 비용이 더 낮아질 경우 
	//		{
	//			(*iter)->pParent = SNode; // 현재 노드를 부모로 바꿈
	//		}
	//	}

	//	// 닫힌 노드에 있는 경우
	//	else if (CloseNode->end() != FindCoordNode(point.x, point.y, CloseNode))
	//	{
	//	}

	//	// 상방향에 장애물이 없고 열린 노드 및 닫힌 노드에 추가되어있지 않은 경우
	//	// 상방향 노드를 열린 노드에 추가, 부모는 현재 탐색 노드로 지정.
	//	else
	//	{
	//		OpenNode->push_back(new CNode(point.x, point.y, SNode, EndPoint));
	//	}
	//}
	//// '우하' 방향 탐색
	//point = { SNode->point.x + 1,SNode->point.y + 1 };
	//if (SNode->point.x < (Navi->sizeX - 1) && SNode->point.y < (Navi->sizeY - 1) &&
	//	Navi->map[point.x][point.y] == 0 && right == false && down == false)
	//	// '우하' 방향에 맵이 존재하고 장애물이 없으며, 우방향과 하방향에도 장애물이 없을 경우
	//{
	//	// 이미 열린 노드에 있는 경우
	//	if (OpenNode->end() != FindCoordNode(point.x, point.y, OpenNode))
	//	{
	//		iter = FindCoordNode(point.x, point.y, OpenNode);
	//		if ((*iter)->G > (SNode->G + 14)) // 원래 부모를 통해서 갔을 때의 비용보다 현재 노드를 통해서 갔을 때 비용이 더 낮아질 경우 
	//		{
	//			(*iter)->pParent = SNode; // 현재 노드를 부모로 바꿈
	//		}
	//	}

	//	// 닫힌 노드에 있는 경우
	//	else if (CloseNode->end() != FindCoordNode(point.x, point.y, CloseNode))
	//	{
	//	}

	//	// 상방향에 장애물이 없고 열린 노드 및 닫힌 노드에 추가되어있지 않은 경우
	//	// 상방향 노드를 열린 노드에 추가, 부모는 현재 탐색 노드로 지정.
	//	else
	//	{
	//		OpenNode->push_back(new CNode(point.x, point.y, SNode, EndPoint));
	//	}
	//}
	//// '좌하' 방향 탐색
	//point = { SNode->point.x + 1,SNode->point.y - 1 };
	//if (SNode->point.x < (Navi->sizeX - 1) && SNode->point.y > 0 && Navi->map[point.x][point.y] == 0 &&
	//	left == false && down == false) // '좌하' 방향에 맵이 존재하고 장애물이 없으며, 좌방향과 하방향에도 장애물이 없을 경우
	//{
	//	// 이미 열린 노드에 있는 경우
	//	if (OpenNode->end() != FindCoordNode(point.x, point.y, OpenNode))
	//	{
	//		iter = FindCoordNode(point.x, point.y, OpenNode);
	//		if ((*iter)->G > (SNode->G + 14)) // 원래 부모를 통해서 갔을 때의 비용보다 현재 노드를 통해서 갔을 때 비용이 더 낮아질 경우 
	//		{
	//			(*iter)->pParent = SNode; // 현재 노드를 부모로 바꿈
	//		}
	//	}

	//	// 닫힌 노드에 있는 경우
	//	else if (CloseNode->end() != FindCoordNode(point.x, point.y, CloseNode))
	//	{
	//	}

	//	// 상방향에 장애물이 없고 열린 노드 및 닫힌 노드에 추가되어있지 않은 경우
	//	// 상방향 노드를 열린 노드에 추가, 부모는 현재 탐색 노드로 지정.
	//	else
	//	{
	//		OpenNode->push_back(new CNode(point.x, point.y, SNode, EndPoint));
	//	}
	//}
	//// '좌상' 방향 탐색
	//point={SNode->point.x - 1,SNode->point.y - 1};
	//if (SNode->point.x > 0 && SNode->point.y > 0 && Navi->map[point.x][point.y] == 0 &&
	//	left == false && up == false) // '좌상' 방향에 맵이 존재하고 장애물이 없으며, 좌방향과 상방향에도 장애물이 없을 경우
	//{
	//	// 이미 열린 노드에 있는 경우
	//	if (OpenNode->end() != FindCoordNode(point.x, point.y, OpenNode))
	//	{
	//		iter = FindCoordNode(point.x, point.y, OpenNode);
	//		if ((*iter)->G > (SNode->G + 14)) // 원래 부모를 통해서 갔을 때의 비용보다 현재 노드를 통해서 갔을 때 비용이 더 낮아질 경우 
	//		{
	//			(*iter)->pParent = SNode; // 현재 노드를 부모로 바꿈
	//		}
	//	}

	//	// 닫힌 노드에 있는 경우
	//	else if (CloseNode->end() != FindCoordNode(point.x, point.y, CloseNode))
	//	{
	//	}

	//	// 상방향에 장애물이 없고 열린 노드 및 닫힌 노드에 추가되어있지 않은 경우
	//	// 상방향 노드를 열린 노드에 추가, 부모는 현재 탐색 노드로 지정.
	//	else
	//	{
	//		OpenNode->push_back(new CNode(point.x, point.y, SNode, EndPoint));
	//	}
	//}
}

void CAstar::FindPath()
{
	path = FindPath(&Navi, StartPoint, EndPoint);
	m_iter = path.begin(); // iter 값 원래대로 돌려주기
}

Coordinate CAstar::GetPos(int order)
{
	Coordinate pos;
	for (int i = 1; i < order; i++)
	{
		m_iter++;
	}
	pos.x = (*m_iter)->x;
	pos.y = (*m_iter)->y;
	m_iter = path.begin();
	return pos;
}

void CAstar::SetFree(int _x, int _y)
{
	Navi.map[_x][_y] = 0;
}

void CAstar::SetObstacle(int _x, int _y)
{
	Navi.map[_x][_y] = 1;
}

void CAstar::PrintPath(HDC hdc)
{
	for (int i = 0; m_iter != path.end(); m_iter++)
	{
		//cout << (*m_iter)->x << (*m_iter)->y << endl;
		Rectangle(hdc,
			(*m_iter)->x*TILECX - TILECX*0.5 + TILECX*0.5,
			(*m_iter)->y *TILECX - TILECY*0.5 + TILECY*0.5,
			(*m_iter)->x*TILECX + TILECX*0.5 + TILECX*0.5,
			(*m_iter)->y *TILECX + TILECY*0.5 + TILECY*0.5);

	}
	m_iter = path.begin(); // iter 값 원래대로 돌려주기
}

void CAstar::PrintMap(HDC hdc)
{
	Navi.PrintMap(hdc);
}

void CAstar::PrintNavi(HDC hdc)
{
	printNavi.Copy(&Navi);

	for (int i = 0; m_iter != path.end(); m_iter++) // 맵에서 경로에 해당하는 곳은 7로 표시
	{
		printNavi.map[(*m_iter)->x][(*m_iter)->y] = 7;
	}
	m_iter = path.begin(); // iter 값 원래대로 돌려주기

	printNavi.PrintMap(hdc);
}

void CAstar::SetStartPoint(Coordinate val)
{
	StartPoint.x = val.x;
	StartPoint.y = val.y;
}

void CAstar::SetEndPoint(Coordinate val)
{
	EndPoint.x = val.x;
	EndPoint.y = val.y;
	FindPath();
}
