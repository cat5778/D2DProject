#include "stdafx.h"
#include "Leg.h"


CLeg::CLeg()
{
}

CLeg::CLeg(const OBJ_INFO & objInfo, D3DXMATRIX mat_World)
{
	m_tFrame.fCurFrame = 0;
	m_tObjInfo = objInfo;
	m_tInfo.matWorld = mat_World;
	Initialize();
}

CLeg::CLeg(const OBJ_INFO & objInfo, D3DXVECTOR3 vPos)
{
	m_tFrame.fCurFrame = 0;
	m_tObjInfo = objInfo;
	m_tInfo.vPos = vPos;
	Initialize();
}


CLeg::~CLeg()
{
}

HRESULT CLeg::Initialize()
{
	D3DXMatrixIdentity(&m_tInfo.matWorld); // 다이렉트 항등행렬 함수
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 0.f, -1.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	return S_OK;
}

void CLeg::Release()
{
}

int CLeg::Update()
{
	m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDelta()/* * 20.f*/;
	if (m_tFrame.fMaxFrame <= m_tFrame.fCurFrame)
		m_tFrame.fCurFrame = 0.f;

	return 0;
}

void CLeg::LateUpdate()
{
}

void CLeg::Render()
{
	UpdateMatWorld(1.0f, ZOrder(m_tInfo.vPos.y-27));
	wstring wstrStateKey = m_tObjInfo.wstrStateKey + m_wsImgDir;
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_tObjInfo.wstrObjectKey, wstrStateKey, m_tFrame.fCurFrame);
	NULL_CHECK(pTexInfo);
	m_tFrame.fMaxFrame = pTexInfo->iMaxCnt;
	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height *0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CLeg::SetvPos(D3DXVECTOR3 vPos, DWORD Dir)
{
	GetImageDir(Dir);
	m_tInfo.vPos = vPos;
	//m_tInfo.vPos.x -= 2;
	m_tInfo.vPos.y += 14;
}
