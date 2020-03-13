#include "stdafx.h"
#include "Hud.h"
#include "ScrollMgr.h"
#include "Icon.h"
#include "HudButton.h"
#include "SkillFrame.h"
CHud::CHud()
	:m_HudButton(nullptr)
{
	
	m_tObjInfo.eObjectType = OBJECT_UI;
	m_tObjInfo.wstrObjectKey = L"SkillIcon";
	m_tObjInfo.wstrStateKey = L"Main_Hud";
	m_tFrame.fCurFrame = 0;
	m_tInfo.vPos = CScrollMgr::GetCenterPos();
	Initialize();

}


CHud::~CHud()
{

}

HRESULT CHud::Initialize()
{
	m_HudIcon[0] = new CIcon(L"SkillIcon", L"Skill_Amazon", 0,  0.382f, 0.397f);
	m_HudIcon[1] = new CIcon(L"SkillIcon", L"Skill_Amazon", 1, 0.332f, 0.397f);
	m_HudIcon[2] = new CIcon(L"SkillIcon", L"Skill_Amazon", 2, 0.283f, 0.397f);
	m_HudIcon[3] = new CIcon(L"SkillIcon", L"Skill_Amazon", 3, 0.234f, 0.397f);
	m_HudButton = new CHudButton(L"SkillIcon", L"Hud_Talents",  0, 0.185f, 0.397f);
	CObjectMgr::GetInstance()->AddObject(OBJECT_UI, m_HudButton);
	for(int i=0;i<4;i++)
		CObjectMgr::GetInstance()->AddObject(OBJECT_UI, m_HudIcon[i]);
	m_SkillFrame = new CSkillFrame();
	CObjectMgr::GetInstance()->AddObject(OBJECT_UI, m_SkillFrame);

	D3DXMatrixIdentity(&m_tInfo.matWorld); // 다이렉트 항등행렬 함수
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 1.f, 1.f, 0.f };
	m_tInfo.vSize = { 1.5f, 1.5f, 0.f };


	return S_OK;
}

void CHud::Release()
{
}

int CHud::Update()
{
	if (m_HudButton != nullptr)
	{
		if (m_HudButton->GetIsPick() || CKeyManager::GetInstance()->KeyDown(KEY_T))
		{
			if(!m_SkillFrame->GetIsOn())
				m_SkillFrame->On();
			else
				m_SkillFrame->Off();
		}
	}

	m_tInfo.vPos = CScrollMgr::GetCenterPos();
	m_tInfo.vPos.x -= WINCX*0.5;
	m_tInfo.vPos.y += WINCY*0.27f;
	return 0;
}

void CHud::LateUpdate()
{
	
}

void CHud::Render()
{
	UpdateMatWorld(1.0f, 0.1f);
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_tObjInfo.wstrObjectKey,
		m_tObjInfo.wstrStateKey, 0);
	NULL_CHECK(pTexInfo);
	m_tFrame.fMaxFrame = pTexInfo->iMaxCnt;
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,nullptr,
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CHud::InitData(int iLevel, float fSpd, float fDmg, float fAtkSpd)
{
	m_iLevel = iLevel;
	m_fSpeed = fSpd;
	m_fDamage = fDmg;
	m_fAtkSpeed = fAtkSpd;
	m_SkillFrame->InitData(m_iLevel, m_fSpeed, m_fDamage, m_fAtkSpeed);
}
