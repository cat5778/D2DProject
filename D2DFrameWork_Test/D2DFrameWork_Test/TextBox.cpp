#include "stdafx.h"
#include "TextBox.h"
#include "ScrollMgr.h"
#include "Mouse.h"
CTextBox::CTextBox()
{
}

CTextBox::CTextBox(float fx, float fy, float fzOrder,float fsize)
{
	m_fXRatio = fx;
	m_fYRatio = fy;
	m_fZOrder = fzOrder;
	m_fSize = fsize;
	Initialize();
}


CTextBox::~CTextBox()
{
}

HRESULT CTextBox::Initialize()
{
	D3DXMatrixIdentity(&m_tInfo.matWorld); // 다이렉트 항등행렬 함수
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 1.f, 1.f, 0.f };
	m_tInfo.vSize = { 1.0f*m_fSize, 1.0f*m_fSize, 0.f };
	return S_OK;
}

void CTextBox::Release()
{
}

int CTextBox::Update()
{
	return 0;
}

void CTextBox::LateUpdate()
{
}

void CTextBox::Render()
{
	m_tInfo.vPos = CScrollMgr::GetCenterPos();
	m_tInfo.vPos.x -= WINCX*m_fXRatio;
	m_tInfo.vPos.y += WINCY*m_fYRatio;
	UpdateMatWorld(1.0f, m_fZOrder);
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	D3DXMatrixTranslation(&m_tInfo.matWorld, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	//m_CollBox->RenderLine();
	m_pDeviceMgr->GetFont()->DrawText(
		m_pDeviceMgr->GetSprite(), /* 스프라이트 COM 객체 */
		m_wsText.c_str(), /* 출력할 문자열 */
		m_wsText.size(), /* 문자열 길이 */
		nullptr, /* 사각형 영역 */
		0,
		D3DCOLOR_ARGB(255, 255, 255, 255));

}
