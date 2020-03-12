#pragma once
class CMyGrid
{
public:
	CMyGrid();
	~CMyGrid();
public:
	void Copy(CMyGrid* _map); // 매개변수를 Map 으로 받아오면 매개변수에서 얕은복사가 일어남.
	void PrintMap(HDC hdc);



public:
	int sizeX, sizeY;
	int** map;

};

