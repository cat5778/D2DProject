#include "stdafx.h"
#include "GameButton.h"
#include "ColliderBox.h"
#include "Mouse.h"

CGameButton::CGameButton()
	: m_vSize(0, 0), m_CollBox(nullptr), m_IsPick(false)
{
}

CGameButton::CGameButton(const OBJ_INFO & objInfo, D3DXVECTOR3 vPos)
	: m_vSize(0, 0), m_CollBox(nullptr), m_IsPick(false), m_wsText(L"")
{
	m_tObjInfo = objInfo;
	m_tInfo.vPos = vPos;
	Initialize();

}


CGameButton::~CGameButton()
{
	Release();
}

HRESULT CGameButton::Initialize()
{
	// 행렬 초기화 -> 항등행렬(I)
	//CMathMgr::MyMatrixIdentity(&m_tInfo.matWorld);
	D3DXMatrixIdentity(&m_tInfo.matWorld); // 다이렉트 항등행렬 함수

	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 0.f, -1.f, 0.f };
	m_tInfo.vSize = { 1.4f, 1.4f, 0.f };

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 10.f;

	return S_OK;
}

void CGameButton::Release()
{
	if (m_CollBox != nullptr)
	{
		delete m_CollBox;
		m_CollBox = nullptr;
	}
}

int CGameButton::Update()
{
	if (m_CollBox != nullptr)
	{
		//m_CollBox->SetvPos(m_tInfo.vPos);
		m_CollBox->Update();
	}
	return 0;
}

void CGameButton::LateUpdate()
{
}

void CGameButton::Render()
{
	const TEX_INFO* pTexInfo;
	if (m_tObjInfo.IsAni)
	{
		pTexInfo = m_pTextureMgr->GetTexInfo(m_tObjInfo.wstrObjectKey, m_tObjInfo.wstrStateKey, (int)m_tFrame.fCurFrame);
		m_tFrame.fMaxFrame = pTexInfo->iMaxCnt;
	}
	else
		pTexInfo = m_pTextureMgr->GetTexInfo(m_tObjInfo.wstrObjectKey, m_tObjInfo.wstrStateKey, m_tObjInfo.ImageIDX);
	NULL_CHECK(pTexInfo);


	m_vSize.x = pTexInfo->tImgInfo.Width;
	m_vSize.y = pTexInfo->tImgInfo.Height;

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;
	//ConvertCenter(m_tObjInfo, *pTexInfo, fCenterX, fCenterY);
	D3DXMATRIX matScale, matTrans;

	D3DXMatrixScaling(&matScale,
		m_tInfo.vSize.x,
		m_tInfo.vSize.y,
		0.f); // 다이렉트 크기행렬 함수

	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x ,
		m_tInfo.vPos.y ,
		0.1); // 다이렉트 이동행렬 함수
	m_tInfo.matWorld = matScale * matTrans;

	D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vLook, &m_tInfo.matWorld);
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);



	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	if (m_vSize.x != 0 && m_CollBox == nullptr)
	{
		m_CollBox = new CColliderBox(m_tInfo.vPos,BUTTON_COLLISION, m_vSize);
	}
	if (m_CollBox != nullptr&&m_CollBox->IsPicking(CMouse::GetMousePos()))
	{
		//m_CollBox->Render();
		m_tObjInfo.ImageIDX = 1;
		if (CKeyManager::GetInstance()->KeyPressing(KEY_LBUTTON))
		{
			m_IsPick = true;
			m_tObjInfo.ImageIDX = 2;
		}
	}
	else
	{
		m_IsPick = false;
		m_tObjInfo.ImageIDX = 0;
	}
	D3DXMATRIX MatText;
	D3DXMatrixTranslation(&MatText, m_tInfo.vPos.x-50 , m_tInfo.vPos.y-10 ,0.f);

	m_pDeviceMgr->GetSprite()->SetTransform(&MatText);
	m_pDeviceMgr->GetFont()->DrawText(
		m_pDeviceMgr->GetSprite(), /* 스프라이트 COM 객체 */
		m_wsText.c_str(), /* 출력할 문자열 */
		m_wsText.size(), /* 문자열 길이 */
		nullptr, /* 사각형 영역 */
		0,
		D3DCOLOR_ARGB(255, 255, 255, 255));

}

