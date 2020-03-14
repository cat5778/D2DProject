#include "stdafx.h"
#include "ImageObject.h"
#include "ScrollMgr.h"
#include "Mouse.h"
#include "ColliderBox.h"
CImageObject::CImageObject()
{
}

CImageObject::CImageObject(wstring wstrObjectKey, wstring wstrStateKey, D3DXVECTOR3 vPos, bool IsAni, int ImageIdx,
	float fXScale, float fYScale, float fzOrder, int iAlpha)
	:m_CollBox(nullptr)
{
	m_tObjInfo.wstrObjectKey = wstrObjectKey;
	m_tObjInfo.wstrStateKey = wstrStateKey;
	m_tInfo.vPos = vPos;
	m_tObjInfo.IsAni = IsAni;
	m_tObjInfo.ImageIDX = ImageIdx;
	m_fXScale=fXScale;
	m_fYScale=fYScale;
	m_fzOrder=fzOrder;
	m_tFrame.fCurFrame = 0;
	m_tFrame.fMaxFrame = 10;
	m_iAlpha - iAlpha;
	m_bIsCollsion = false;
	Initialize();
}


CImageObject::~CImageObject()
{
	Release();
}

HRESULT CImageObject::Initialize()
{
	D3DXMatrixIdentity(&m_tInfo.matWorld); // 다이렉트 항등행렬 함수
										   //m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };


	return S_OK;
}

void CImageObject::Release()
{
}

int CImageObject::Update()
{
	if (m_tObjInfo.IsAni)
		Animate();
	if (m_CollBox != nullptr)
	{
		m_CollBox->SetvPos(m_tInfo.vPos);
		m_bIsCollsion = m_CollBox->IsCollsion();
		
	}
	return NO_EVENT;
}

void CImageObject::LateUpdate()
{
}

void CImageObject::Render()
{
	D3DXMATRIX matScale, matTrans, matRot;
	D3DXMatrixScaling(&matScale,
		m_tInfo.vSize.x*m_fXScale,
		m_tInfo.vSize.y*m_fYScale,
		0.f); // 다이렉트 크기행렬 함수
	if (m_fzOrder == 1)
	{
		D3DXMatrixTranslation(&matTrans,
			m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x,
			m_tInfo.vPos.y - CScrollMgr::GetScrollPos().y,
			ZOrder(m_tInfo.vPos.y - CScrollMgr::GetScrollPos().y)); // 다이렉트 이동행렬 함수
	}
	else
	{

		D3DXMatrixTranslation(&matTrans,
			m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x,
			m_tInfo.vPos.y - CScrollMgr::GetScrollPos().y,
			m_fzOrder); // 다이렉트 이동행렬 함수

	}

	D3DXMatrixRotationZ(&matRot, 0);
	m_tInfo.matWorld = matScale* matRot*matTrans;
	D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vLook, &m_tInfo.matWorld);
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
	//
	const TEX_INFO* pTexInfo;
	if (m_tObjInfo.IsAni)
		pTexInfo = m_pTextureMgr->GetTexInfo(m_tObjInfo.wstrObjectKey, m_tObjInfo.wstrStateKey, m_tFrame.fCurFrame);
	else
		pTexInfo = m_pTextureMgr->GetTexInfo(m_tObjInfo.wstrObjectKey, m_tObjInfo.wstrStateKey, m_tObjInfo.ImageIDX);
	NULL_CHECK(pTexInfo);

	m_tFrame.fMaxFrame = pTexInfo->iMaxCnt;
	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
}

void CImageObject::SetCollider(D3DXVECTOR2 _vSIze)
{
	m_vSize = _vSIze;
	if (m_CollBox == nullptr)
	{
		m_CollBox = new CColliderBox(m_tInfo.vPos, OBJECT_HITBOX_COLLISION, m_vSize);
		m_pColliderMgr->AddObject(OBJECT_HITBOX_COLLISION, m_CollBox);
	}
}

