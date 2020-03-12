#pragma once
#include "MyGrid.h"
#include "Node.h"

class CAstar
{
public:
	CAstar();
	CAstar(Coordinate _StartPoint, Coordinate _EndPoint);
	~CAstar();


private:
	list<Coordinate*> FindPath(CMyGrid* Navi, Coordinate StartPoint, Coordinate EndPoint);
	list<CNode*>::iterator FindNextNode(list<CNode*>* pOpenNode); // 오픈노드 중 F값이 제일 작은 노드 찾아서 반환
	list<CNode*>::iterator FindCoordNode(int x, int y, list<CNode*>* NodeList); // 노드리스트에서 x,y 좌표의 노드를 찾아서 주소를 반환. 없으면 end()반환.
	void ExploreNode(CMyGrid* Navi, CNode* SNode, list<CNode*>* OpenNode, list<CNode*>* CloseNode, Coordinate EndPoint); // 8방향 노드를 탐색하고 열린 노드에 추가 및 부모 변경을 실행함

public:
	void FindPath();
	Coordinate GetPos(int order); // order번째 경로의 좌표를 받아옴
	list<Coordinate*> GetPath() { return path; } // 경로를 Coordinate* 리스트로 통째로 받아옴
	void SetFree(int _x, int _y); // 해당 좌표의 장애물을 없앰
	void SetObstacle(int _x, int _y); // 해당 좌표에 장애물을 설치
	void PrintPath(HDC hdc);
	void PrintMap(HDC hdc);
	void PrintNavi(HDC hdc);
	void SetStartPoint(Coordinate val);
	void SetEndPoint(Coordinate val);
private:
	CMyGrid Navi; // 맵 생성
	CMyGrid printNavi; // 출력용 맵 생성(경로까지 출력)

private:
	Coordinate StartPoint; // 출발지점
	Coordinate EndPoint; // 목표지점
	list<Coordinate*> path; // 경로
	list<Coordinate*>::iterator m_iter; // 경로 iterator

};

