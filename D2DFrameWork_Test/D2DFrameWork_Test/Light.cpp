#include "stdafx.h"
#include "Light.h"


CLight::CLight()
{
}

CLight::CLight(LIGHT_TYPE eType, D3DXVECTOR3 vPos)
	:m_eLightType(eType)
{
	m_tInfo.vPos = vPos;
	Initialize();
}


CLight::~CLight()
{
}

HRESULT CLight::Initialize()
{
	switch (m_eLightType)
	{
	case HORIZONTAL_LIGHT:
	{
		m_tObjInfo.wstrObjectName=L"Horizontal_Light";
		m_tObjInfo.wstrObjectKey = L"Effect";
		m_tObjInfo.wstrStateKey = L"Horizontal_Light";
	}
		break;
	case VERTICAL_LIGHT:
	{
		m_tObjInfo.wstrObjectName = L"Vertical_Light";
		m_tObjInfo.wstrObjectKey = L"Effect";
		m_tObjInfo.wstrStateKey = L"Vertical_Light";
	}
	break;
	default:
		break;
	}

	m_tObjInfo.ImageIDX = 0;
	m_tObjInfo.IsAni = false;
	m_tObjInfo.eObjectType = OBJECT_EFFECT;
	D3DXMatrixIdentity(&m_tInfo.matWorld); // 다이렉트 항등행렬 함수
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 0.f, -1.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 10.f;

	return E_NOTIMPL;
}

void CLight::Release()
{
}

int CLight::Update()
{
	return NO_EVENT;
}

void CLight::LateUpdate()
{
	UpdateMatWorld(1.0f,0.92f);

}

void CLight::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_tObjInfo.wstrObjectKey, m_tObjInfo.wstrStateKey, m_tObjInfo.ImageIDX);
	NULL_CHECK(pTexInfo);
	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height ;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(100, 255, 255, 255));
}

void CLight::GetTexture()
{
	//const TEX_INFO* pTexInfo;
	//if (m_tObjInfo.IsAni)
	//{
	//	pTexInfo = m_pTextureMgr->GetTexInfo(m_tObjInfo.wstrObjectKey, m_tObjInfo.wstrStateKey, (int)m_tFrame.fCurFrame);
	//	m_tFrame.fMaxFrame = pTexInfo->iMaxCnt;
	//}
	//else
	//	pTexInfo = m_pTextureMgr->GetTexInfo(m_tObjInfo.wstrObjectKey, m_tObjInfo.wstrStateKey, m_tObjInfo.ImageIDX);
	//NULL_CHECK(pTexInfo);
	//float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	//float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	//m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	//m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
	//	nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}
