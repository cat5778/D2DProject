#include "stdafx.h"
#include "MyGrid.h"


CMyGrid::CMyGrid()
{
	// 맵의 크기를 불러옴
	char csizeX[4], csizeY[4];
	GetPrivateProfileStringA("MAP", "sizeX", "", csizeX, sizeof(csizeX) - 1, "../Data/Map.txt");
	GetPrivateProfileStringA("MAP", "sizeY", "", csizeY, sizeof(csizeY) - 1, "../Data/Map.txt");
	sizeX = atoi(csizeX); 
	sizeY = atoi(csizeY);

	// 맵 크기 할당
	map = new int*[sizeX]; // 행 할당
	for (int i = 0; i < sizeX; i++) // 열 할당
	{
		map[i] = new int[sizeY];
	}

	FILE* stream = fopen("../Data/Map.txt", "rb"); // 맵의 자료를 불러옴 0=지나갈 수 있는 곳 , 1=장애물
	for (int i = 0; i < sizeX; i++)
	{
		for (int j = 0; j < sizeY; j++)
		{
			fscanf(stream, "%d", &map[i][j]);
			
		}
	}
	fclose(stream);
	for (int i = 0; i < sizeX; i++)
	{
		for (int j = 0; j < sizeY; j++)
		{
			printf("%d ", map[i][j]);
		}
		printf("\n");
	}
	//sizeX = WINCX / TILECX;
	//sizeY = WINCY / TILECY;
	//map = new int*[sizeX]; // 행 할당
	//for (int i = 0; i < sizeX; i++) // 열 할당
	//{
	//	map[i] = new int[sizeY]();
	//}

}


CMyGrid::~CMyGrid()
{
	for (int i = 0; i < sizeX; i++)
	{
		delete[] map[i]; // 맵의 열 동적할당 해제, 열은 행의 갯수만큼 동적할당 되었으므로 sizeX만큼 반복
	}
	delete[] map; // 맵의 행 동적할당 해제

}

void CMyGrid::Copy(CMyGrid * _map)
{
	sizeX = _map->sizeX;
	sizeY = _map->sizeY;

	for (int i = 0; i < sizeX; i++)
	{
		for (int j = 0; j < sizeY; j++)
		{
			map[i][j] = _map->map[i][j];
		}
	}

}

void CMyGrid::PrintMap(HDC hdc)
{
	for (int i = 0; i < sizeX; i++)
	{
		for (int j = 0; j < sizeY; j++)
		{
			HBRUSH brush;
			HBRUSH oldBrush;
			switch (map[i][j])
			{
			case 0:
			{
				brush = CreateSolidBrush(RGB(255, 255, 255));
				oldBrush = (HBRUSH)SelectObject(hdc, brush);
			}
			break;
			case 1:
			{
				brush = CreateSolidBrush(RGB(0, 0, 0));
				oldBrush = (HBRUSH)SelectObject(hdc, brush);
			}
			break;

			default:
			{
				brush = CreateSolidBrush(RGB(255, 0, 0));
				oldBrush = (HBRUSH)SelectObject(hdc, brush);
			}
			break;
			}

			Rectangle(hdc,
				j*TILECX - TILECX*0.5 + TILECX*0.5,
				i*TILECX - TILECY*0.5 + TILECY*0.5,
				j*TILECX + TILECX*0.5 + TILECX*0.5,
				i*TILECX + TILECY*0.5 + TILECY*0.5);
			SelectObject(hdc, oldBrush);
			DeleteObject(brush);

			//printf("%d ", map[i][j]);
		}
		//printf("\n");
	}
}


