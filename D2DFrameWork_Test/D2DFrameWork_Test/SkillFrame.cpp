#include "stdafx.h"
#include "SkillFrame.h"
#include "ScrollMgr.h"
#include "Icon.h"
#include "HudButton.h"
#include "SkillLine.h"
#include "TextBox.h"
CSkillFrame::CSkillFrame()
	:m_bIsRender(false), m_SkillLine(nullptr), m_iPoint(10)
{
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

	m_TextBox[0] = new CTextBox(0.25f, -0.18, 0.086f, 0.75f);
	m_TextBox[1] = new CTextBox(0.25f, -0.10, 0.086f, 0.75f);
	m_TextBox[2] = new CTextBox(0.25f, -0.02, 0.086f, 0.75f);
	m_TextBox[3] = new CTextBox(0.25f, 0.06, 0.086f, 0.75f);

	D3DXMatrixIdentity(&m_tInfo.matWorld); // ���̷�Ʈ �׵���� �Լ�
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
					m_SkillButton[i]->SetButtonText(to_wstring(m_iSkillPoint[i]));
				}
				if (i == 1)
				{
					m_iSkillPoint[i] ++;
					m_iPoint--;
					m_SkillButton[i]->SetButtonText(to_wstring(m_iSkillPoint[i]));
				}
				if (i == 2&& m_iSkillPoint[3]!=0)
				{
					m_iSkillPoint[i] ++;
					m_iPoint--;
					m_SkillButton[i]->SetButtonText(to_wstring(m_iSkillPoint[i]));
				}
				if (i == 3 && m_iSkillPoint[1] != 0)
				{
					m_iSkillPoint[i] ++;
					m_iPoint--;
					m_SkillButton[i]->SetButtonText(to_wstring(m_iSkillPoint[i]));
				}
				if (i == 4 && m_iSkillPoint[0] != 0)
				{
					m_iSkillPoint[i] ++;
					m_iPoint--;
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
				}
				if (i == 7 && m_iSkillPoint[4] != 0)
				{
					m_iSkillPoint[i] ++;
					m_iPoint--;
					m_SkillButton[i]->SetButtonText(to_wstring(m_iSkillPoint[i]));
				}
				if (i == 8 && m_iSkillPoint[5] != 0)
				{
					m_iSkillPoint[i] ++;
					m_iPoint--;
					m_SkillButton[i]->SetButtonText(to_wstring(m_iSkillPoint[i]));
				}
			}
			
		}
		m_SkillLine->Update();
		m_wsText = L"Skill Point " + to_wstring(m_iPoint);
		m_wsLevel = L"����:  " + to_wstring(m_iLevel);
		m_wsDamage = L"���ݷ�:  " + to_wstring(m_fDamage);
		m_wsAtkSpeed = L"���ݼӵ�:  " + to_wstring(m_fAtkSpeed);
		m_wsSpeed = L"�̵��ӵ�:  " + to_wstring(m_fAtkSpeed);



		for (int i = 0; i < 4; i++)
		{
			if (m_TextBox[i]!=nullptr)
				m_TextBox[i]->Update();
		}
		m_TextBox[0]->SetButtonText(m_wsLevel);
		m_TextBox[1]->SetButtonText(m_wsDamage);
		m_TextBox[2]->SetButtonText(m_wsAtkSpeed);
		m_TextBox[3]->SetButtonText(m_wsSpeed);

		#pragma endregion

	
	
	
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
			m_pDeviceMgr->GetSprite(), /* ��������Ʈ COM ��ü */
			m_wsText.c_str(), /* ����� ���ڿ� */
			m_wsText.size(), /* ���ڿ� ���� */
			nullptr, /* �簢�� ���� */
			0,
			D3DCOLOR_ARGB(255, 255, 255, 255));


		//m_wsLevel


	}
}

void CSkillFrame::InitData(int iLevel, float fSpd, float fDmg, float fAtkSpd)
{
	m_iLevel= iLevel;
	m_fSpeed= fSpd;
	m_fDamage= fDmg;
	m_fAtkSpeed= fAtkSpd;

}

void CSkillFrame::LevelUp()
{
	m_iLevel++;
	m_iPoint++;
}
