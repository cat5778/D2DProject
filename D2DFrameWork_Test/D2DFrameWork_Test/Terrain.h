#pragma once
#include "GameObject.h"

class CToolView;
class CTerrain :
	public CGameObject
{
	DECLARE_SINGLETON(CTerrain)
public:
	CTerrain();
	CTerrain(wstring file);
	~CTerrain();

	// CGameObject을(를) 통해 상속됨
public:
	virtual void Release() override;
	virtual HRESULT Initialize() override;

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
public:
	void TileChange(const D3DXVECTOR3& vPos, const BYTE& byDrawID, const BYTE& byOption = 0,const wstring &wsTexKey= L"TileSet1");
	void SaveTile(const TCHAR* pFilePath);
	HRESULT LoadTile(const TCHAR* pFilePath);
	int GetCenterPos();
private:
	//int	GetTileIndex(const D3DXVECTOR3& vPos);
	//bool IsPicking(const D3DXVECTOR3& vPos, int iIndex);
	POINT TileDrawConverstion(int _drawID, int XCount, int YCount);
	HRESULT ReadyGraph();

public:
	static CTerrain* Create(wstring file);
public:
	vector<TILE_INFO*>	m_vecTile;
	vector<list<TILE_INFO*>>	m_vecGraph;
	D3DXVECTOR3 m_CenterPos;
	wstring		m_wsFile;

};

