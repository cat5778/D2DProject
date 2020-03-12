#include "stdafx.h"
#include "ToolView.h"
#include "Terrain.h"

IMPLEMENT_SINGLETON(CTerrain)
CTerrain::CTerrain()
{
	
}


CTerrain::~CTerrain()
{
}

HRESULT CTerrain::Initialize()
{
	
	srand(time(nullptr));
	D3DXVECTOR3 vPos = {};
	D3DXVECTOR3 vSize = {};
	for (int i = 0; i < 50; i++)
	{
		for (int j = 0; j < 50; j++)
		{
			vPos = { (float)j*TILECX+ TILECX*0.5f,(float)i*TILECY+ TILECY*0.5f,0.f };
			vSize = { 1.f,1.f,0.f};
			PTILE_INFO tile = new TILE_INFO;
			tile->vPos = vPos;
			tile->vSize = vSize;
			/*if (tile->wsTexKey.compare(L"") == 0)
			{
				WORD i = rand() % 6+1;
				tile->byDrawID = 0;

			}
			else*/
				tile->byDrawID = 0;
			tile->byOption = 0;
			tile->wsTexKey = L"";
			m_vecTile.push_back(tile);
		}
	}
	return E_NOTIMPL;
}

void CTerrain::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), [](PTILE_INFO &ptile)
	{
		if (ptile != nullptr)
			delete ptile;
		ptile = nullptr;
	});

	m_vecTile.clear();

}

int CTerrain::Update()
{


	return S_OK;
}

void CTerrain::LateUpdate()
{


}

void CTerrain::Render()
{
	D3DXMATRIX matScale, matTrans;

	const TEX_INFO* pTexInfo = nullptr;
	float fCenterX = 0.f, fCenterY = 0.f;

	TCHAR szIndexNum[MIN_STR] = L"";

	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		if (m_vecTile[i]->wsTexKey.compare(L"") == 0)
			pTexInfo = m_pTextureMgr->GetTexInfo(L"DefalutTile1");
		else
			pTexInfo = m_pTextureMgr->GetTexInfo(L"Tile", m_vecTile[i]->wsTexKey);
		NULL_CHECK_CONTINUE(pTexInfo);

		D3DXMatrixScaling(&matScale,
			m_vecTile[i]->vSize.x,
			m_vecTile[i]->vSize.y,
			m_vecTile[i]->vSize.z);
		D3DXMatrixTranslation(&matTrans,
			m_vecTile[i]->vPos.x  - m_pView->GetScrollPos(0),
			m_vecTile[i]->vPos.y  - m_pView->GetScrollPos(1),
			1);
		
		POINT tID=TileDrawConverstion(m_vecTile[i]->byDrawID, pTexInfo->tImgInfo.Width/TILECX, pTexInfo->tImgInfo.Height/TILECY);
		fCenterX = TILECX*0.5f;
		fCenterY = TILECY*0.5f;
		RECT rt = { TILECX*tID.x , TILECY*tID.y , TILECX*tID.x+ TILECX , TILECY*tID.y+ TILECY };
		m_pDeviceMgr->GetSprite()->SetTransform(&(matScale * matTrans));
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, &rt,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		


		// 타일 인덱스 출력
		swprintf_s(szIndexNum, L"%d",(int)m_vecTile[i]->byOption);
		
		m_pDeviceMgr->GetFont()->DrawText(m_pDeviceMgr->GetSprite(), szIndexNum,
			lstrlen(szIndexNum), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	}

}

void CTerrain::MiniView()
{
	D3DXMATRIX matScale, matTrans;

	const TEX_INFO* pTexInfo = nullptr;
	float fCenterX = 0.f, fCenterY = 0.f;

	TCHAR szIndexNum[MIN_STR] = L"";

	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		pTexInfo = m_pTextureMgr->GetTexInfo(m_vecTile[i]->wsTexKey);
		NULL_CHECK_CONTINUE(pTexInfo);

		D3DXMatrixScaling(&matScale,
			m_vecTile[i]->vSize.x*0.25f,
			m_vecTile[i]->vSize.y*0.25f,
			m_vecTile[i]->vSize.z);
		D3DXMatrixTranslation(&matTrans,
			m_vecTile[i]->vPos.x*0.25f - m_pView->GetScrollPos(0),
			m_vecTile[i]->vPos.y*0.25f - m_pView->GetScrollPos(1),
			m_vecTile[i]->vPos.z);

		POINT tID = TileDrawConverstion(m_vecTile[i]->byDrawID,pTexInfo->tImgInfo.Width / TILECX, pTexInfo->tImgInfo.Height / TILECY);
		fCenterX = TILECX*0.5f;
		fCenterY = TILECY*0.5f;
		RECT rt = { TILECX*tID.x , TILECY*tID.y , TILECX*tID.x + TILECX , TILECY*tID.y + TILECY };
		m_pDeviceMgr->GetSprite()->SetTransform(&(matScale * matTrans));
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, &rt,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	}
}

void CTerrain::TileChange(const D3DXVECTOR3 & vPos, const BYTE & byDrawID, const BYTE & byOption, const wstring& wsTexKey)
{
	int iIndex = GetTileIndex(vPos);

	if (-1 == iIndex)
		return;

	m_vecTile[iIndex]->byDrawID = byDrawID;
	m_vecTile[iIndex]->byOption = byOption;
	m_vecTile[iIndex]->wsTexKey=wsTexKey;
	//wcout <<L"Terrain TileChange ="<< m_vecTile[iIndex]->wsTexKey << endl;

}
int CTerrain::GetTileIndex(const D3DXVECTOR3& vPos)
{
	for (int i = 0; i < (int)m_vecTile.size(); ++i)
	{
		if (IsPicking(vPos, i))
			return i;
	}

	return -1;
}
bool CTerrain::IsPicking(const D3DXVECTOR3 & vPos, int iIndex)
{
	// 내적을 이용한 방식 (데카르트 기준)
	// 마름모 꼭지점 설정 12시, 3시, 6시, 9시 순서
	D3DXVECTOR3 vPoint[4] =
	{
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x + TILECX * 0.5f, m_vecTile[iIndex]->vPos.y - TILECY * 0.5f, 0.f),
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x + TILECX * 0.5f, m_vecTile[iIndex]->vPos.y + TILECY * 0.5f, 0.f),
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x - TILECX * 0.5f, m_vecTile[iIndex]->vPos.y + TILECY * 0.5f, 0.f),
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x - TILECX * 0.5f, m_vecTile[iIndex]->vPos.y - TILECY * 0.5f, 0.f)
	};

	// 각 꼭지점으로부터 시계방향으로 방향벡터들을 구함.
	D3DXVECTOR3 vDir[4] =
	{
		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3],
	};

	// 위에서 구한 방향벡터들의 각 법선벡터들을 구한다.
	D3DXVECTOR3 vNormal[4] = {};

	for (int i = 0; i < 4; ++i)
		D3DXVec3Cross(&vNormal[i], &D3DXVECTOR3(0.f, 0.f, -1.f), &vDir[i]);

	// 각 꼭지점에서부터 마우스위치로의 방향벡터를 구한다.
	D3DXVECTOR3 vMouseDir[4] =
	{
		vPos - vPoint[0],
		vPos - vPoint[1],
		vPos - vPoint[2],
		vPos - vPoint[3]
	};

	// vMouseDir과 vNormal을 각각 내적해서 모두 둔각(음수)이 나오면 TRUE!
	for (int i = 0; i < 4; ++i)
	{
		if (0 < D3DXVec3Dot(&vNormal[i], &vMouseDir[i]))
			return false;
	}

	return true;
}

POINT CTerrain::TileDrawConverstion(int _drawID, int XCount, int YCount)
{
	int id = _drawID;
	POINT tileID;
	tileID.x = id % XCount;
	tileID.y = id / XCount;
	//cout << "tileid x=" << tileID.x <<" "<< XCount << endl;
	//cout << "tileid y=" << tileID.y <<" "<< YCount<< endl;
	return tileID;
}

void CTerrain::SaveTile(const TCHAR * pFilePath)
{
	HANDLE hFile = ::CreateFile(pFilePath, GENERIC_WRITE, 0, nullptr,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return;
	size_t iLen = 0;
	DWORD dwBytes = 0;
	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		iLen = m_vecTile[i]->wsTexKey.length() + 1;

		//WriteFile(hFile, m_vecTile[i], sizeof(TILE_INFO), &dwBytes, nullptr); //원본
		WriteFile(hFile, &iLen, sizeof(size_t), &dwBytes, nullptr);
		WriteFile(hFile, m_vecTile[i]->wsTexKey.c_str(), sizeof(wchar_t) * iLen, &dwBytes, nullptr);
		WriteFile(hFile, &m_vecTile[i]->vPos, sizeof(D3DXVECTOR3), &dwBytes, nullptr);
		WriteFile(hFile, &m_vecTile[i]->vSize, sizeof(D3DXVECTOR3), &dwBytes, nullptr);
		WriteFile(hFile, &m_vecTile[i]->byDrawID, sizeof(BYTE), &dwBytes, nullptr);
		WriteFile(hFile, &m_vecTile[i]->byOption, sizeof(BYTE), &dwBytes, nullptr);
		//wstring ws = m_vecTile[i]->wsTexKey+"/0"
	}
	CloseHandle(hFile);
}

void CTerrain::LoadTile(const TCHAR * pFilePath)
{
	HANDLE hFile = ::CreateFile(pFilePath, GENERIC_READ, 0, nullptr,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	if (!m_vecTile.empty())
	{
		for_each(m_vecTile.begin(), m_vecTile.end(), SafeDelete<TILE_INFO*>);
		m_vecTile.clear();
	}

	DWORD dwBytes = 0;
	TILE_INFO tTile = {};

	size_t iLen = 0;
		wchar_t* pString = nullptr;

	while (true)
	{
		//ReadFile(hFile, &tTile, sizeof(TILE_INFO), &dwBytes, nullptr);
		ReadFile(hFile, &iLen, sizeof(size_t), &dwBytes, nullptr);
		pString = new wchar_t[iLen];
		ReadFile(hFile, pString, sizeof(wchar_t) * iLen, &dwBytes, nullptr);
		ReadFile(hFile, &tTile.vPos, sizeof(D3DXVECTOR3), &dwBytes, nullptr);
		ReadFile(hFile, &tTile.vSize, sizeof(D3DXVECTOR3), &dwBytes, nullptr);
		ReadFile(hFile, &tTile.byDrawID, sizeof(BYTE), &dwBytes, nullptr);
		ReadFile(hFile, &tTile.byOption, sizeof(BYTE), &dwBytes, nullptr);
		tTile.wsTexKey = pString;
		if (0 == dwBytes)
			break;

		m_vecTile.push_back(new TILE_INFO(tTile));
	}

	CloseHandle(hFile);
}

