#include "stdafx.h"
#include "Title.h"
#include "ColliderBox.h"
#include "KeyManager.h"
CTitle::CTitle()
{
}

CTitle::CTitle(const OBJ_INFO & objInfo, D3DXVECTOR3 vPos)
	:m_isFire(false)
{
	
	m_tObjInfo = objInfo;
	m_tInfo.vPos = vPos;
	Initialize();
}


CTitle::~CTitle()
{
}

HRESULT CTitle::Initialize()
{
	// 행렬 초기화 -> 항등행렬(I)
	//CMathMgr::MyMatrixIdentity(&m_tInfo.matWorld);
	D3DXMatrixIdentity(&m_tInfo.matWorld); // 다이렉트 항등행렬 함수

	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 0.f, -1.f, 0.f };
	m_tInfo.vSize = { 1.4f, 1.4f, 0.f };

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 10.f;
	if (m_tObjInfo.wstrObjectName.compare(L"Menu_Fire") == 0)
		m_isFire = true;
	return S_OK;
}

void CTitle::Release()
{
}

int CTitle::Update()
{
	if (m_tObjInfo.IsAni)
	{
		m_tFrame.fCurFrame  += m_tFrame.fMaxFrame * m_pTimeMgr->GetDelta() *0.75f;
		if (m_tFrame.fMaxFrame <= m_tFrame.fCurFrame)
			m_tFrame.fCurFrame = 0.f;
	}
	

	return 0;
}

void CTitle::LateUpdate()
{
	D3DXMATRIX matScale, matTrans;

	D3DXMatrixScaling(&matScale,
		m_tInfo.vSize.x*g_fWheel,
		m_tInfo.vSize.y*g_fWheel,
		0.f); // 다이렉트 크기행렬 함수

	if (m_isFire)
	{
		D3DXMatrixTranslation(&matTrans,
			m_tInfo.vPos.x - 25,
			m_tInfo.vPos.y + 90,
			0.9); // 다이렉트 이동행렬 함수
	}
	else
	{
		D3DXMatrixTranslation(&matTrans,
			m_tInfo.vPos.x + 100,
			m_tInfo.vPos.y + 100,
			ZOrder(m_tInfo.vPos.y)); // 다이렉트 이동행렬 함수
	}
	m_tInfo.matWorld = matScale * matTrans;

	D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vLook, &m_tInfo.matWorld);
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
	

}

void CTitle::Render()
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

	float fCenterX = 0;
	float fCenterY = 0;

	//float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	//float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;
	ConvertCenter(m_tObjInfo, *pTexInfo, fCenterX, fCenterY);
	
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	
}

float CTitle::ZOrder(float _y)
{
	return 1 - (_y *0.0001f);
}

void CTitle::ConvertPos(const OBJ_INFO & objInfo, const TEX_INFO & pTexInfo, D3DXVECTOR3 & vPos)
{
}

void CTitle::ConvertCenter(const OBJ_INFO & objInfo, const TEX_INFO & pTexInfo, float & cx, float & cy)
{
	if (objInfo.wstrObjectName.find(L"Tree") != wstring::npos)
	{
		cx = pTexInfo.tImgInfo.Width*0.5f;
		cy = pTexInfo.tImgInfo.Height;
	}
	else if (objInfo.wstrObjectName.find(L"Lamp") != wstring::npos)
	{
		cx = pTexInfo.tImgInfo.Width*0.5f;
		cy = pTexInfo.tImgInfo.Height;
	}
	else
	{
		cx = pTexInfo.tImgInfo.Width*0.5f;
		cy = pTexInfo.tImgInfo.Height*0.5f;
	}

}
