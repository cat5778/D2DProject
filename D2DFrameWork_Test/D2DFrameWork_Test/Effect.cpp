#include "stdafx.h"
#include "Effect.h"


CEffect::CEffect()
{
}

CEffect::CEffect(D3DXVECTOR3 vPos, wstring wstrObjKey, wstring wstrStateKey,float Zorder)
	:m_fZOrder(Zorder)
{
	m_tInfo.vPos = vPos;
	m_tObjInfo.eObjectType = OBJECT_EFFECT;
	m_tObjInfo.wstrObjectKey = wstrObjKey;
	m_tObjInfo.wstrStateKey = wstrStateKey;
	m_tFrame.fCurFrame = 0;
	m_tFrame.fMaxFrame = 10;
	Initialize();
}


CEffect::~CEffect()
{
	Release();
}

HRESULT CEffect::Initialize()
{
	D3DXMatrixIdentity(&m_tInfo.matWorld); // 다이렉트 항등행렬 함수
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 1.f, 1.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	return S_OK;
}

void CEffect::Release()
{

}

int CEffect::Update()
{

	if (Animate(false))
		return DEAD_OBJ;

	return NO_EVENT;
}

void CEffect::LateUpdate()
{
}

void CEffect::Render()
{
	if (m_fZOrder == 1.f)
		UpdateMatWorld(1.0f, ZOrder(m_tInfo.vPos.y));
	else
		UpdateMatWorld(1.0f, m_fZOrder);

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_tObjInfo.wstrObjectKey,
		m_tObjInfo.wstrStateKey, m_tFrame.fCurFrame);
	NULL_CHECK(pTexInfo);
	m_tFrame.fMaxFrame = pTexInfo->iMaxCnt;
	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}
