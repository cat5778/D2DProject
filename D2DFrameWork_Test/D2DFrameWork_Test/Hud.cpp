#include "stdafx.h"
#include "Hud.h"
#include "ScrollMgr.h"
#include "Icon.h"
#include "HudButton.h"
#include "SkillFrame.h"
#include "Inven.h"
CHud::CHud()
	:m_HudButton(nullptr), m_bIsOn(false), m_bIsEnd(false), m_bIsOn2(false), m_bIsEnd2(false)
{
	
	m_tObjInfo.eObjectType = OBJECT_UI;
	m_tObjInfo.wstrObjectKey = L"SkillIcon";
	m_tObjInfo.wstrStateKey = L"Main_Hud";
	m_tFrame.fCurFrame = 0;
	m_tInfo.vPos = CScrollMgr::GetCenterPos();
	Initialize();

}

CHud::CHud(GAME_DATA gameData)
{
	m_tObjInfo.eObjectType = OBJECT_UI;
	m_tObjInfo.wstrObjectKey = L"SkillIcon";
	m_tObjInfo.wstrStateKey = L"Main_Hud";
	m_tFrame.fCurFrame = 0;
	m_tInfo.vPos = CScrollMgr::GetCenterPos();
	m_tData = gameData;
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
	m_SkillFrame = new CSkillFrame(m_tData);
	CObjectMgr::GetInstance()->AddObject(OBJECT_UI, m_SkillFrame);
	m_Inven = new CInven();
	CObjectMgr::GetInstance()->AddObject(OBJECT_UI, m_Inven);


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
		if (m_HudButton->GetIsPick() || m_bIsOn)
		{
			
			if (!m_SkillFrame->GetIsOn())
			{
				m_SkillFrame->On();
				m_bIsEnd = false;
			}
			else
			{
				m_SkillFrame->Off();
				m_bIsEnd = true;
			}
			SetOnSkillTree();
		}
		if (m_bIsOn2)
		{
			if (!m_Inven->GetIsOn())
			{
				m_Inven->On();
				m_bIsEnd2 = false;
			}
			else
			{
				m_Inven->Off();
				m_bIsEnd2 = true;
			}
			SetOnInven();
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

GAME_DATA CHud::GetData()
{
	m_tData = m_SkillFrame->GetData();
	
	return m_tData;
}
SKILL_DATA CHud::GetSkillData()
{
	m_tSkillData = m_SkillFrame->GetSkillData();
	return m_tSkillData;
}


void CHud::SetOnSkillTree()
{
	m_bIsOn ? m_bIsOn = false : m_bIsOn = true;
}

void CHud::SetOnInven()
{
	m_bIsOn2 ? m_bIsOn2 = false : m_bIsOn2 = true;
}

void CHud::SetData(GAME_DATA gameData)
{
	m_tData = gameData;
	m_SkillFrame->SetData(m_tData);
}

void CHud::InitData(GAME_DATA gameData)
{
	m_tData = gameData;
	m_SkillFrame->InitData(m_tData);
}

