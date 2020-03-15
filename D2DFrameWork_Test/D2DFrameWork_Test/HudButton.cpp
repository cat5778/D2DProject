#include "stdafx.h"
#include "HudButton.h"
#include "ScrollMgr.h"
#include "ColliderBox.h"
#include "Mouse.h"
CHudButton::CHudButton()
{
}

CHudButton::CHudButton(wstring wstrObjKey, wstring wstrStateKey, int iImgKey, float fxRatio, float fyRatio, float zOrder, D3DXVECTOR2 ButtonSize)
	:m_fXRatio(fxRatio), m_fYRatio(fyRatio), m_IsPick(false), m_CollBox(nullptr), m_bIsRadioButton(false), m_fZOrder(zOrder)
{//ICON 크기 38
	m_vSize = ButtonSize;
	m_tObjInfo.ImageIDX = iImgKey;
	m_tObjInfo.eObjectType = OBJECT_UI;
	m_tObjInfo.wstrObjectKey = wstrObjKey;
	m_tObjInfo.wstrStateKey = wstrStateKey;
	m_tFrame.fCurFrame = 0;
	m_tFrame.fMaxFrame = 10;
	Initialize();
}


CHudButton::~CHudButton()
{
}

HRESULT CHudButton::Initialize()
{
	D3DXMatrixIdentity(&m_tInfo.matWorld); // 다이렉트 항등행렬 함수
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 1.f, 1.f, 0.f };
	m_tInfo.vSize = { 1.5f, 1.5f, 0.f };
	m_CollBox = new CColliderBox(m_tInfo.vPos, BUTTON_COLLISION, m_vSize);
	//ColliderMgr::GetInstance()->AddObject(BUTTON_COLLISION, m_CollBox);
	return S_OK;
}

void CHudButton::Release()
{
	if (m_CollBox != nullptr)
	{
		delete m_CollBox;
		m_CollBox = nullptr;
	}
}

int CHudButton::Update()//431 742
{
	D3DXVECTOR3 vConvertPos = m_tInfo.vPos;
	vConvertPos.x += 30;
	vConvertPos.y +=30;
	m_CollBox->SetvPos(vConvertPos);
	if (!m_IsPick&&m_CollBox->IsPicking(CMouse::GetMousePos()))
	{
		if (CKeyManager::GetInstance()->KeyDown(KEY_LBUTTON))
			m_IsPick = true;
		
	}
	else
		m_IsPick = false;
	
	return 0;
}

void CHudButton::LateUpdate()
{
}

void CHudButton::Render()
{
	m_tInfo.vPos = CScrollMgr::GetCenterPos();
	m_tInfo.vPos.x -= WINCX*m_fXRatio;
	m_tInfo.vPos.y += WINCY*m_fYRatio;
	UpdateMatWorld(1.0f, m_fZOrder);
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_tObjInfo.wstrObjectKey,
		m_tObjInfo.wstrStateKey, m_tObjInfo.ImageIDX);
	NULL_CHECK(pTexInfo);

	m_tFrame.fMaxFrame = pTexInfo->iMaxCnt;
	//float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	//float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, nullptr,
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	//D3DXMATRIX MatText;
	//D3DXMatrixTranslation(&MatText, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);


	//m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	D3DXVECTOR3 temp = m_tInfo.vPos - CScrollMgr::GetScrollPos();
	
	D3DXMatrixTranslation(&m_tInfo.matWorld, temp.x+45, temp.y+45, 0.f);
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	//m_CollBox->RenderLine();

	m_pDeviceMgr->GetFont()->DrawText(
		m_pDeviceMgr->GetSprite(), /* 스프라이트 COM 객체 */
		m_wsText.c_str(), /* 출력할 문자열 */
		m_wsText.size(), /* 문자열 길이 */
		nullptr, /* 사각형 영역 */
		0,
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CHudButton::SetTextPos(D3DXVECTOR2 vPos)
{
	m_vTextPos = m_tInfo.vPos;
	m_vTextPos.x += vPos.x;
	m_vTextPos.y += vPos.y;

}

bool CHudButton::RadioButton()
{
	m_bIsRadioButton ? m_bIsRadioButton = false : m_bIsRadioButton = true;
	return m_bIsRadioButton;
}
