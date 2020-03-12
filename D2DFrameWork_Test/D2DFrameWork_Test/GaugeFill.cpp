#include "stdafx.h"
#include "GaugeFill.h"
#include "ScrollMgr.h"

CGaugeFill::CGaugeFill()
{
}

CGaugeFill::CGaugeFill(FILL_TYPE eType, D3DXVECTOR3 vPos,RECT rt)
	:m_eFill(eType), m_tRect(rt)
{
	m_tInfo.vPos = vPos;
	m_tObjInfo.wstrObjectKey = L"UI";
	m_tFrame.fCurFrame = 0;
	Initialize();
}


CGaugeFill::~CGaugeFill()
{
}

HRESULT CGaugeFill::Initialize()
{
	D3DXMatrixIdentity(&m_tInfo.matWorld); // 다이렉트 항등행렬 함수
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	switch (m_eFill)
	{
	case FILL_HP:
		m_tObjInfo.wstrStateKey = L"HP_Fill";
		break;
	case FILL_MANA:
		m_tObjInfo.wstrStateKey = L"MANA_Fill";
		break;
	default:
		break;
	}

	return S_OK;
}

void CGaugeFill::Release()
{
}

int CGaugeFill::Update()
{
	
	return NO_EVENT;
}

void CGaugeFill::LateUpdate()
{
}

void CGaugeFill::Render()
{
 
	D3DXMATRIX matScale, matTrans;
	D3DXMatrixScaling(&matScale,
		m_tInfo.vSize.x,
		m_tInfo.vSize.y*0.5,
		0.f); // 다이렉트 크기행렬 함수
	
		D3DXMatrixTranslation(&matTrans,
			m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x,
			m_tInfo.vPos.y - CScrollMgr::GetScrollPos().y,
			0.09f); // 다이렉트 이동행렬 함수

	m_tInfo.matWorld = matScale * matTrans;

	D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vLook, &m_tInfo.matWorld);
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

	//cout << m_fRadian << endl;
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_tObjInfo.wstrObjectKey,
		m_tObjInfo.wstrStateKey, m_tFrame.fCurFrame);
	NULL_CHECK(pTexInfo);
	
	m_tFrame.fMaxFrame = pTexInfo->iMaxCnt;
	m_vSize.x = pTexInfo->tImgInfo.Width * 0.5f;
	m_vSize.y = pTexInfo->tImgInfo.Height * 0.5f;
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, &m_tRect, &D3DXVECTOR3(m_vSize.x, m_vSize.y, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CGaugeFill::UpdateData(D3DXVECTOR3 vPos, RECT rect)
{
	m_tInfo.vPos = vPos;
	m_tInfo.vPos.x -= 12;
	m_tRect = rect;
}
