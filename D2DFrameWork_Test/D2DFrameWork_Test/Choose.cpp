#include "stdafx.h"
#include "Choose.h"
#include "Shadow.h"
#include "ColliderBox.h"
#include "ScrollMgr.h"
#include "Mouse.h"
#include "Light.h"
CChoose::CChoose()

{
}

CChoose::CChoose(const OBJ_INFO & objInfo, D3DXVECTOR3 vPos)
	:m_CollBox(nullptr),m_Light(nullptr), m_bIsChose(false)
{
	m_tObjInfo = objInfo;
	m_tInfo.vPos = vPos;
	Initialize();
}


CChoose::~CChoose()
{
}

HRESULT CChoose::Initialize()
{
	D3DXMatrixIdentity(&m_tInfo.matWorld); // 다이렉트 항등행렬 함수
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 0.f, -1.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 10.f;

	m_Shadow = new CShadow(m_tInfo.vPos);
	CObjectMgr::GetInstance()->AddObject(OBJECT_EFFECT, m_Shadow);

	return S_OK;
}

void CChoose::Release()
{
}

int CChoose::Update()
{
	if (m_tObjInfo.IsAni)
	{
		m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDelta()/* * 20.f*/;
		if (m_tFrame.fMaxFrame <= m_tFrame.fCurFrame)
			m_tFrame.fCurFrame = 0.f;
	}
	return 0;
}
//float CChoose::ZOrder(float _y)
//{
//	return 1 - (_y *0.0001f);
//}
void CChoose::LateUpdate()
{

	UpdateMatWorld(1.0f,0.9f);
	m_Shadow->SetvPos(ConvertShadow());
}

void CChoose::Render()
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
	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	if (pTexInfo !=nullptr && m_CollBox == nullptr)
	{
		m_CollBox = new CColliderBox(m_tInfo.vPos, BUTTON_COLLISION, D3DXVECTOR2(pTexInfo->tImgInfo.Width, pTexInfo->tImgInfo.Height));
	}

	if (m_CollBox != nullptr)
	{
		if(m_CollBox->IsPicking(CMouse::GetMousePos())&& !m_bIsChose&&
			CKeyManager::GetInstance()->KeyDown(KEY_LBUTTON))
		{
			CLight* Temp = new CLight(VERTICAL_LIGHT,m_tInfo.vPos);
			CObjectMgr::GetInstance()->AddObject(OBJECT_LIGHT, Temp);
			m_bIsChose = true;
		}
		if (m_bIsChose  && CKeyManager::GetInstance()->KeyDown(KEY_LBUTTON))
		{
			m_bIsChose = false;
			CObjectMgr::GetInstance()->DeleteObject(OBJECT_LIGHT);
		}
	}

}

D3DXVECTOR3 CChoose::ConvertShadow()
{
	D3DXVECTOR3 vPos = m_tInfo.vPos;
	vPos.x -= CScrollMgr::GetScrollPos().x;
	vPos.y -= CScrollMgr::GetScrollPos().y;

	if (m_tObjInfo.wstrStateKey.compare(L"Amazon_Choose") == 0)
	{
		vPos.x+=5;
		vPos.y+=5;
	}
	else if (m_tObjInfo.wstrStateKey.compare(L"Stickman_Choose") == 0)
	{
		vPos.y += 15;
	}
	else if (m_tObjInfo.wstrStateKey.compare(L"Marauder_Choose") == 0)
	{
		vPos.x -=10;
		vPos.y +=7;

	}
	else if (m_tObjInfo.wstrStateKey.compare(L"Samurai_Choose") == 0)
	{
		vPos.x +=9;
		vPos.y +=15;

	}
	else if (m_tObjInfo.wstrStateKey.compare(L"White_Mage_Choose") == 0)
	{
		vPos.x += 5;
		vPos.y += 10;

	}
	else
	{
		vPos.y+=5;
	}
		return vPos;
}
