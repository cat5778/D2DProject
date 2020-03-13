#include "stdafx.h"
#include "Icon.h"
#include "ScrollMgr.h"

CIcon::CIcon()
{
}

CIcon::CIcon(wstring wstrObjKey, wstring wstrStateKey, int iImgKey, float fxRatio,float fyRatio)
	:m_fXRatio(fxRatio),m_fYRatio(fyRatio)
{
	m_tObjInfo.ImageIDX = iImgKey;
	m_tObjInfo.eObjectType = OBJECT_UI;
	m_tObjInfo.wstrObjectKey = wstrObjKey;
	m_tObjInfo.wstrStateKey = wstrStateKey;
	m_tFrame.fCurFrame = 0;
	m_tFrame.fMaxFrame = 10;
	Initialize();
}


CIcon::~CIcon()
{
}

HRESULT CIcon::Initialize()
{
	D3DXMatrixIdentity(&m_tInfo.matWorld); // 다이렉트 항등행렬 함수

	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 1.f, 1.f, 0.f };
	m_tInfo.vSize = { 1.5f, 1.5f, 0.f };
	return S_OK;
}

void CIcon::Release()
{
}

int CIcon::Update()
{
	m_tInfo.vPos = CScrollMgr::GetCenterPos();
	m_tInfo.vPos.x -= WINCX*m_fXRatio;
	m_tInfo.vPos.y += WINCY*m_fYRatio;
	return 0;
}

void CIcon::LateUpdate()
{
}

void CIcon::Render()
{
	UpdateMatWorld(1.0f, 0.09f);
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_tObjInfo.wstrObjectKey,
		m_tObjInfo.wstrStateKey, m_tObjInfo.ImageIDX);
	NULL_CHECK(pTexInfo);
	m_tFrame.fMaxFrame = pTexInfo->iMaxCnt;
	//float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	//float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, nullptr,
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}
