#include "stdafx.h"
#include "Hat.h"


CHat::CHat()
{
}

CHat::CHat(const OBJ_INFO & objInfo, D3DXMATRIX mat_World)
{
	m_tFrame.fCurFrame = 0;
	m_tObjInfo = objInfo;
	m_tInfo.matWorld = mat_World;
	Initialize();
}

CHat::CHat(const OBJ_INFO & objInfo, D3DXVECTOR3 vPos)
{
	m_tFrame.fCurFrame = 0;
	m_tObjInfo = objInfo;
	m_tInfo.vPos= vPos;
	Initialize();

}


CHat::~CHat()
{
}

HRESULT CHat::Initialize()
{
	D3DXMatrixIdentity(&m_tInfo.matWorld); // 다이렉트 항등행렬 함수
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 0.f, -1.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	return S_OK;
}

void CHat::Release()
{
}

int CHat::Update()
{
	//UpdateMatWorld(1.0f, 0.8f);
	m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDelta()/* * 20.f*/;
	if (m_tFrame.fMaxFrame <= m_tFrame.fCurFrame)
		m_tFrame.fCurFrame = 0.f;

		
	return 0;
}

void CHat::LateUpdate()
{
	//UpdateMatWorld();
}

void CHat::Render()
{
	UpdateMatWorld(1.0f, ZOrder(m_tInfo.vPos.y));
	wstring wstrStateKey= m_tObjInfo.wstrStateKey + m_wsImgDir;
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_tObjInfo.wstrObjectKey, wstrStateKey, m_tFrame.fCurFrame);
	NULL_CHECK(pTexInfo);
	m_tFrame.fMaxFrame = pTexInfo->iMaxCnt;
	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	

}

void CHat::SetvPos(D3DXVECTOR3 vPos,DWORD Dir)
{
	GetImageDir(Dir);
	m_tInfo.vPos = vPos;
	if (Dir == UP)
	{
		m_tInfo.vPos.y -= 7;
	}
	else if (Dir == RIGHT)
	{
		m_tInfo.vPos.x -= 4;
		m_tInfo.vPos.y -= 9;
	}
	else if (Dir == LEFT)
	{
		m_tInfo.vPos.x += 4;
		m_tInfo.vPos.y -= 9;
	}
	else if (Dir == DOWN)
	{
		m_tInfo.vPos.x += 2;
		m_tInfo.vPos.y -= 9;
	}
}
