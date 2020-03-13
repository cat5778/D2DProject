#include "stdafx.h"
#include "SkillLine.h"
#include "ScrollMgr.h"

CSkillLine::CSkillLine()
{
	m_tObjInfo.eObjectType = OBJECT_UI;
	m_tObjInfo.wstrObjectKey = L"SkillIcon";
	m_tObjInfo.wstrStateKey = L"Skill_Tree";
	m_tObjInfo.ImageIDX = 2;
	m_tFrame.fCurFrame = 0;
	m_tInfo.vPos = CScrollMgr::GetCenterPos();
	Initialize();
}


CSkillLine::~CSkillLine()
{
}

HRESULT CSkillLine::Initialize()
{
	D3DXMatrixIdentity(&m_tInfo.matWorld); // 다이렉트 항등행렬 함수
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 1.f, 1.f, 0.f };
	m_tInfo.vSize = { 1.5f, 1.5f, 0.f };
	return S_OK;
}

void CSkillLine::Release()
{
}

int CSkillLine::Update()
{

	m_tInfo.vPos = CScrollMgr::GetCenterPos();
	
	return 0;
}

void CSkillLine::LateUpdate()
{
}

void CSkillLine::Render()
{
	UpdateMatWorld(1.0f, 0.088f);
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_tObjInfo.wstrObjectKey,
		m_tObjInfo.wstrStateKey, 2);
	NULL_CHECK(pTexInfo);
	m_tFrame.fMaxFrame = pTexInfo->iMaxCnt;
	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}
