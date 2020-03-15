#include "stdafx.h"
#include "SkillFrame.h"
#include "ScrollMgr.h"
#include "Icon.h"
#include "HudButton.h"
#include "SkillLine.h"
#include "TextBox.h"
CSkillFrame::CSkillFrame()
{
}
CSkillFrame::CSkillFrame(GAME_DATA gameData)
	:m_bIsRender(false), m_SkillLine(nullptr), m_iPoint(10)
{
	ZeroMemory(&m_tSkillData, sizeof(SkillData));
	for (int i = 0; i < 9; i++)
	{
		m_SkillButton[i] = nullptr;
		m_iSkillPoint[i] = 0;
	}
	for (int i = 0; i < 4; i++)
	{
		m_TextBox[i] = nullptr;
	}
	m_tObjInfo.eObjectType = OBJECT_UI;
	m_tObjInfo.wstrObjectKey = L"SkillIcon";
	m_tObjInfo.wstrStateKey = L"Skill_Tree";
	m_tFrame.fCurFrame = 0;
	m_tInfo.vPos = CScrollMgr::GetCenterPos();
	m_tData= gameData;
	m_iOldLevel = m_tData.iLevel;
	Initialize();

}


CSkillFrame::~CSkillFrame()
{
}

HRESULT CSkillFrame::Initialize()
{
	m_SkillLine = new CSkillLine();
	m_SkillButton[0] = new CHudButton(L"SkillIcon", L"Talents_Amazon", 0, -0.05f, -0.15f, 0.087f);
	m_SkillButton[1] = new CHudButton(L"SkillIcon", L"Talents_Amazon", 1, -0.2f, -0.15f, 0.087f);
	m_SkillButton[2] = new CHudButton(L"SkillIcon", L"Talents_Amazon", 2, -0.125f, -0.05f, 0.087f);
	m_SkillButton[3] = new CHudButton(L"SkillIcon", L"Talents_Amazon", 3, -0.2f, -0.05f, 0.087f);
	m_SkillButton[4] = new CHudButton(L"SkillIcon", L"Talents_Amazon", 4, -0.05f, 0.05f, 0.087f);
	m_SkillButton[5] = new CHudButton(L"SkillIcon", L"Talents_Amazon", 5, -0.05f, 0.15f, 0.087f);
	m_SkillButton[6] = new CHudButton(L"SkillIcon", L"Talents_Amazon", 6, -0.125f, 0.15f, 0.087f);
	m_SkillButton[7] = new CHudButton(L"SkillIcon", L"Talents_Amazon", 7, -0.2f, 0.15f, 0.087f);
	m_SkillButton[8] = new CHudButton(L"SkillIcon", L"Talents_Amazon", 8, -0.05f, 0.25f, 0.087f);

	for (int i = 0; i < 9; i++)
	{
		m_SkillButton[i]->SetButtonText(to_wstring(0));
	}

	m_TextBox[0] = new CTextBox(0.27f, -0.165, 0.086f, 0.75f);
	m_TextBox[1] = new CTextBox(0.27f, -0.14, 0.086f, 0.75f);
	m_TextBox[2] = new CTextBox(0.27f, -0.09, 0.086f, 0.75f);
	m_TextBox[3] = new CTextBox(0.27f, -0.070, 0.086f, 0.75f);

	D3DXMatrixIdentity(&m_tInfo.matWorld); // 다이렉트 항등행렬 함수
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 1.f, 1.f, 0.f };
	m_tInfo.vSize = { 1.5f, 1.5f, 0.f };
	
	

	
	return S_OK;
}

void CSkillFrame::Release()
{
}

int CSkillFrame::Update()
{
	if (m_bIsRender)
	{
		m_tInfo.vPos = CScrollMgr::GetCenterPos();
		#pragma region SkillTree

		for (int i = 0; i < 9; i++)
		{
			m_SkillButton[i]->Update();
		
			if (m_SkillButton[i]->GetIsPick()&& m_iPoint > 0)
			{
				if (i == 0)
				{
					m_iSkillPoint[i] ++;
					m_iPoint--;
					m_tData.fDamage = 10 * m_iSkillPoint[0] + m_tData.fDamage;
					m_SkillButton[i]->SetButtonText(to_wstring(m_iSkillPoint[i]));
				}
				if (i == 1)
				{
					m_iSkillPoint[i] ++;
					m_iPoint--;
					m_SkillButton[i]->SetButtonText(to_wstring(m_iSkillPoint[i]));
					m_tSkillData.iDashLv = m_iSkillPoint[i];

				}
				if (i == 2&& m_iSkillPoint[3]!=0)
				{
					m_iSkillPoint[i] ++;
					m_iPoint--;
					m_tData.fSpeed = m_iSkillPoint[2] * 50 + m_tData.fSpeed;
					m_SkillButton[i]->SetButtonText(to_wstring(m_iSkillPoint[i]));
				}
				if (i == 3 && m_iSkillPoint[1] != 0)
				{
					m_iSkillPoint[i] ++;
					m_iPoint--;
					m_tSkillData.iMulptLv = m_iSkillPoint[i];
					m_SkillButton[i]->SetButtonText(to_wstring(m_iSkillPoint[i]));
				}
				if (i == 4 && m_iSkillPoint[0] != 0)
				{
					m_iSkillPoint[i] ++;
					m_iPoint--;
					m_tData.fAtkSpeed = m_iSkillPoint[4] * 0.5f + m_tData.fAtkSpeed;
					m_SkillButton[i]->SetButtonText(to_wstring(m_iSkillPoint[i]));

				}
				if (i == 5 && m_iSkillPoint[6] != 0)
				{
					m_iSkillPoint[i] ++;
					m_iPoint--;
					m_SkillButton[i]->SetButtonText(to_wstring(m_iSkillPoint[i]));
				}
				if (i == 6 && m_iSkillPoint[7] != 0)
				{
					m_iSkillPoint[i] ++;
					m_iPoint--;
					m_SkillButton[i]->SetButtonText(to_wstring(m_iSkillPoint[i]));
					m_tSkillData.iThunderLv = m_iSkillPoint[i];
				}
				if (i == 7 && m_iSkillPoint[4] != 0)
				{
					m_iSkillPoint[i] ++;
					m_iPoint--;
					m_SkillButton[i]->SetButtonText(to_wstring(m_iSkillPoint[i]));
					m_tSkillData.iBuffLv= m_iSkillPoint[i];
				}
				if (i == 8 && m_iSkillPoint[5] != 0)
				{
					m_iSkillPoint[i] ++;
					m_iPoint--;
					m_SkillButton[i]->SetButtonText(to_wstring(m_iSkillPoint[i]));
					m_tSkillData.iFinish = m_iSkillPoint[i];

				}
			}
			
		}

		#pragma endregion
		m_SkillLine->Update();

		m_wsText = L"Skill Point " + to_wstring(m_iPoint);
		m_wsLevel = L"레벨:  " + to_wstring(m_tData.iLevel);
		m_wsDamage = L"공격력:  " + to_wstring(m_tData.fDamage);
		m_wsAtkSpeed = L"공격속도:  " + to_wstring(m_tData.fAtkSpeed);
		m_wsSpeed = L"이동속도:  " + to_wstring(m_tData.fSpeed);

		for (int i = 0; i < 4; i++)
		{
			if (m_TextBox[i]!=nullptr)
				m_TextBox[i]->Update();
		}
		m_TextBox[0]->SetButtonText(m_wsLevel);
		m_TextBox[1]->SetButtonText(m_wsDamage);
		m_TextBox[2]->SetButtonText(m_wsAtkSpeed);
		m_TextBox[3]->SetButtonText(m_wsSpeed);

	}
	return 0;
}

void CSkillFrame::LateUpdate()
{
}

void CSkillFrame::Render()
{
	if (m_bIsRender)
	{
		for (int i = 0; i < 9; i++)
		{
			if (m_SkillButton[i] != nullptr)
				m_SkillButton[i]->Render();
		}
		for (int i = 0; i < 4; i++)
		{
			if (m_TextBox[i] != nullptr)
				m_TextBox[i]->Render();
		}
		UpdateMatWorld(1.0f, 0.089f);
		const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_tObjInfo.wstrObjectKey,
			m_tObjInfo.wstrStateKey, 0);
		NULL_CHECK(pTexInfo);
		m_tFrame.fMaxFrame = pTexInfo->iMaxCnt;
		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;
		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
			nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_SkillLine->Render();
		D3DXVECTOR3 temp = m_tInfo.vPos-CScrollMgr::GetScrollPos();
		D3DXMatrixTranslation(&m_tInfo.matWorld, temp.x+135, temp.y-165, 0.f);
		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);

	
		m_pDeviceMgr->GetFont()->DrawText(
			m_pDeviceMgr->GetSprite(), /* 스프라이트 COM 객체 */
			m_wsText.c_str(), /* 출력할 문자열 */
			m_wsText.size(), /* 문자열 길이 */
			nullptr, /* 사각형 영역 */
			0,
			D3DCOLOR_ARGB(255, 255, 255, 255));


		//m_wsLevel


	}
}

void CSkillFrame::InitData(GAME_DATA gameData)
{
	m_tData = gameData;
	m_iOldLevel = m_tData.iLevel;
}

void CSkillFrame::LevelUp()
{
	m_tData.iLevel++;
	m_iPoint++;
}

void CSkillFrame::SetData(GAME_DATA gameData)
{
	m_tData = gameData;
	m_iPoint+=m_tData.iLevel- m_iOldLevel;
	m_iOldLevel = m_tData.iLevel;
}

SKILL_DATA CSkillFrame::GetSkillData()
{
	return m_tSkillData;
}
