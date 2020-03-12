#include "stdafx.h"
#include "Shadow.h"


CShadow::CShadow()
{
}

CShadow::CShadow(D3DXVECTOR3 vPos)
{
	m_tInfo.vPos = vPos;
	Initialize();
}


CShadow::~CShadow()
{
}

HRESULT CShadow::Initialize()
{
	D3DXMatrixIdentity(&m_tInfo.matWorld); // 다이렉트 항등행렬 함수

	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 0.f, -1.f, 0.f };
	m_tInfo.vSize = { 0.5f, 0.5f, 0.f };

	m_tFrame.fCurFrame = 0.f;

	return S_OK;
}

void CShadow::Release()
{
}

int CShadow::Update()
{
	return 0;
}

void CShadow::LateUpdate()
{
	D3DXMATRIX matScale, matTrans;

	D3DXMatrixScaling(&matScale,
		m_tInfo.vSize.x,
		m_tInfo.vSize.y,
		0.f); // 다이렉트 크기행렬 함수
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x,
		m_tInfo.vPos.y,
		1); // 다이렉트 이동행렬 함수

	m_tInfo.matWorld = matScale * matTrans;

	D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vLook, &m_tInfo.matWorld);
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
}

void CShadow::Render()
{
	const TEX_INFO* pTexInfo= m_pTextureMgr->GetTexInfo(L"Effect", L"Shadow_Boulder", (int)m_tFrame.fCurFrame);
	NULL_CHECK(pTexInfo);
	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}
