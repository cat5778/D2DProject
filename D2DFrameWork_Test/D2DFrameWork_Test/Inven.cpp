#include "stdafx.h"
#include "Inven.h"
#include "ScrollMgr.h"
#include "HudButton.h"

CInven::CInven()
	:m_bIsRender(false)
{
	m_tObjInfo.eObjectType = OBJECT_UI;
	m_tObjInfo.wstrObjectKey = L"Inventory";
	m_tObjInfo.wstrStateKey = L"Inventory_List_Back";
	m_tFrame.fCurFrame = 0;
	m_tInfo.vPos = CScrollMgr::GetCenterPos();
	Initialize();
}


CInven::~CInven()
{
	Release();
}

HRESULT CInven::Initialize()
{

	m_ItemButton[WEAPONE_FIST] = nullptr;
	m_ItemButton[WEAPONE_BOW1] = new CHudButton(L"Item",		L"Weapon_Bow_Unique",		6,0.15f, -0.40f, 0.084f,D3DXVECTOR2(100,100));
	m_ItemButton[WEAPONE_BOW] =	new CHudButton(L"Item", L"Weapon_Bow_Unique",		8,0.15f, -0.23f, 0.084f, D3DXVECTOR2(100,100));
	m_ItemButton[WEAPONE_JAVELIN] = new CHudButton(L"Item", L"Weapon_Javelin_Unique",	6,0.15f, -0.06f, 0.084f, D3DXVECTOR2(100, 100));
	m_ItemButton[WEAPONE_JAVELIN1] =new CHudButton(L"Item", L"Weapon_Javelin_Unique",	2,0.15f, +0.1f, 0.084f, D3DXVECTOR2(100, 100));

	D3DXMatrixIdentity(&m_tInfo.matWorld); // 다이렉트 항등행렬 함수
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 1.f, 1.f, 0.f };
	m_tInfo.vSize = { 1.5f, 1.5f, 0.f };

	return S_OK;
}

void CInven::Release()
{
}

int CInven::Update()
{
	m_tInfo.vPos = CScrollMgr::GetCenterPos();
	if (m_bIsRender)
	{
		for (int i = 0; i < WEAPONE_END; i++)
		{
			if (m_ItemButton[i] != nullptr)
			{
				m_ItemButton[i]->Update();
				if (m_ItemButton[i]->GetIsPick())
					g_eWpType = (WEAPONE_TYPE)i;
			}
		}
	}
	return NO_EVENT;
}

void CInven::LateUpdate()
{
}

void CInven::Render()
{
	if (m_bIsRender)
	{
		for (int i = 0; i < WEAPONE_END; i++)
		{
			if (m_ItemButton[i] != nullptr)
				m_ItemButton[i]->Render();
		}

	UpdateMatWorld(1.0f, 0.085f);
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_tObjInfo.wstrObjectKey,m_tObjInfo.wstrStateKey, 0);
	NULL_CHECK(pTexInfo);
	m_tFrame.fMaxFrame = pTexInfo->iMaxCnt;
	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

}
