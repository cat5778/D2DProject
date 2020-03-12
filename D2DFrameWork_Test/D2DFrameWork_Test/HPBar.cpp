#include "stdafx.h"
#include "HPBar.h"
#include "GaugeFill.h"

CHPBar::CHPBar()
{
}

CHPBar::CHPBar(OBJECT_TYPE eType,D3DXVECTOR3 vPos,GAME_DATA data)
{
	ZeroMemory(&m_tRect, sizeof(RECT));
	m_tData = data;
	m_tObjInfo.eObjectType = eType;
	m_tInfo.vPos = vPos;
	m_CGugeFill[FILL_HP] = nullptr;
	m_CGugeFill[FILL_MANA] = nullptr;
	m_tObjInfo.wstrObjectKey = L"UI";
	m_tObjInfo.wstrStateKey = L"HP_Bar";
	m_tFrame.fCurFrame = 0;
	Initialize();
}


CHPBar::~CHPBar()
{
}

HRESULT CHPBar::Initialize()
{
	D3DXMatrixIdentity(&m_tInfo.matWorld); // 다이렉트 항등행렬 함수
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	switch (m_tObjInfo.eObjectType)
	{
	case OBJECT_MONSTER:
		m_wstrType = L"Monster_";
		break;
	case OBJECT_PLAYER:
	{
		m_wstrType = L"Player_";
	//m_CGugeFill[FILL_HP] = new CGaugeFill(FILL_HP,m_tInfo.vPos);
	}
	break;
	default:
		break;
	}
	CreateFill();
	return S_OK;
}

void CHPBar::Release()
{
}

int CHPBar::Update()
{
	
	
	return NO_EVENT;
}

void CHPBar::LateUpdate()
{
}

void CHPBar::Render()
{
	UpdateMatWorld(1.0f, 0.1f);
	//cout << m_fRadian << endl;
	wstring wstrStatekey = m_wstrType + m_tObjInfo.wstrStateKey;
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_tObjInfo.wstrObjectKey,
		wstrStatekey, m_tFrame.fCurFrame);
	NULL_CHECK(pTexInfo);

	m_tFrame.fMaxFrame = pTexInfo->iMaxCnt;
	m_vSize.x = pTexInfo->tImgInfo.Width * 0.5f;
	m_vSize.y = pTexInfo->tImgInfo.Height * 0.5f;
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(m_vSize.x, m_vSize.y, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	for (int i = 0; i < FILL_END; i++)
	{
		if (m_CGugeFill[i] != nullptr)
			m_CGugeFill[i]->Render();
	}
	
}

void CHPBar::SetData(D3DXVECTOR3 vPos, GAME_DATA data, float fY)
{
	m_tData = data;
	if (m_tData.fCurHp < 0.f)
		m_tData.fCurHp = 0;
	m_fHPRatio = m_tData.fCurHp / m_tData.fHp;
	m_tInfo.vPos = vPos; 
	m_tInfo.vPos.y -= fY;
	UpdateRect();
	for (int i = 0; i < FILL_END; i++)
	{
		if (m_CGugeFill[i] != nullptr)
			m_CGugeFill[i]->UpdateData(m_tInfo.vPos, m_tRect);

	}
}

void CHPBar::UpdateRect()
{
	m_tRect.left = m_tInfo.vPos.x - m_vSize.x*0.5f;
	m_tRect.right = m_tInfo.vPos.x + m_vSize.x*0.5f;
	m_tRect.top = m_tInfo.vPos.y - m_vSize.y*0.5f;
	m_tRect.bottom = m_tInfo.vPos.y + m_vSize.y*0.5f;
	m_tRect.right = m_tRect.left+((m_tRect.right - m_tRect.left)*m_fHPRatio);
}

void CHPBar::CreateFill()
{
	if (m_tObjInfo.eObjectType == OBJECT_PLAYER)
	{
		m_CGugeFill[FILL_HP] = new CGaugeFill(FILL_HP, m_tInfo.vPos, m_tRect);
		//m_CGugeFill[FILL_HP] = new CGaugeFill(FILL_HP, m_tInfo.vPos, m_tRect);
	}

}
