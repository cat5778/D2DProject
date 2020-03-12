#include "stdafx.h"
#include "Obstacle.h"
#include "ScrollMgr.h"

CObstacle::CObstacle()
{
}

CObstacle::CObstacle(const  OBJ_INFO& objInfo, D3DXVECTOR3 vPos)
{
	m_tObjInfo = objInfo;
	m_tInfo.vPos = vPos;
	Initialize();
}


CObstacle::~CObstacle()
{
}

HRESULT CObstacle::Initialize()
{
	// 행렬 초기화 -> 항등행렬(I)
	//CMathMgr::MyMatrixIdentity(&m_tInfo.matWorld);
	D3DXMatrixIdentity(&m_tInfo.matWorld); // 다이렉트 항등행렬 함수
	CheckType();
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 0.f, -1.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 10.f;

	return S_OK;
}

void CObstacle::Release()
{
}

int CObstacle::Update()
{
	if (m_tObjInfo.IsAni)
	{
		m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDelta()/* * 20.f*/;
		if (m_tFrame.fMaxFrame <= m_tFrame.fCurFrame)
			m_tFrame.fCurFrame = 0.f;
	}
	return 0;
}

void CObstacle::LateUpdate()
{
	D3DXMATRIX matScale, matTrans;

	D3DXMatrixScaling(&matScale,
		m_tInfo.vSize.x,
		m_tInfo.vSize.y,
		0.f); // 다이렉트 크기행렬 함수
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x,
		m_tInfo.vPos.y - CScrollMgr::GetScrollPos().y,
		ZOrder()); // 다이렉트 이동행렬 함수

	m_tInfo.matWorld = matScale * matTrans;

	D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vLook, &m_tInfo.matWorld);
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
}

void CObstacle::Render()
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

float CObstacle::ZOrder()
{
	switch (m_eType)
	{
	case OBSTACLE_TREE:
		return 1 - ((m_tInfo.vPos.y - 40) *0.00001f);
		break;
	case OBSTACLE_GRASS:
		return 1 - ((m_tInfo.vPos.y-20 ) * 0.00001f);
		break;
	case OBSTACLE_BUILDING:
		break;
	case OBSTACLE_TP:
		break;
	case OBSTACLE_FENCE:
		return 1 - ((m_tInfo.vPos.y - 20) * 0.00001f);
		break;
	case OBSTACLE_LAMP:
		return 1 - ((m_tInfo.vPos.y - 35) * 0.00001f);
		break;
	case OBSTACLE_END:
		return 1 - (m_tInfo.vPos.y *0.00001f);
		break;
	default:
		return 1 - (m_tInfo.vPos.y *0.00001f);
		break;
	}
}

void CObstacle::ConvertPos(const OBJ_INFO & objInfo, const TEX_INFO & pTexInfo, D3DXVECTOR3 & vPos)
{
	D3DXVECTOR3 Temp;
	int iW = 0, iH = 0;
	if (objInfo.wstrObjectName.find(L"TP") != wstring::npos)
	{
		m_eType = OBSTACLE_TP;

		iW = pTexInfo.tImgInfo.Width*0.6f;
		iH = pTexInfo.tImgInfo.Height*0.25f;
		vPos.x = (((int)vPos.x / iW) *iW);
		vPos.y = (((int)vPos.y / iH) *iH);
	}
	else if (objInfo.wstrObjectName.find(L"Horizontal_Wall") != wstring::npos)
	{
		m_eType = OBSTACLE_TP;
		iW = pTexInfo.tImgInfo.Width*0.9f;
		iH = pTexInfo.tImgInfo.Height;
		vPos.x = (((int)vPos.x / iW) *iW);
		vPos.y = (((int)vPos.y / iH) *iH);
	}
	else if (objInfo.wstrObjectName.find(L"Vertical_Wall") != wstring::npos)
	{
		m_eType = OBSTACLE_TP;
		iW = pTexInfo.tImgInfo.Width*0.75;
		iH = pTexInfo.tImgInfo.Height*0.8;
		vPos.x = (((int)vPos.x / iW) *iW);
		vPos.y = (((int)vPos.y / iH) *iH);
	}
	else if (objInfo.wstrObjectName.find(L"Wooden_Wall") != wstring::npos)
	{
		m_eType = OBSTACLE_TP;
		iW = pTexInfo.tImgInfo.Width;
		iH = pTexInfo.tImgInfo.Height*0.5;
		vPos.x = (((int)vPos.x / iW) *iW);
		vPos.y = (((int)vPos.y / iH) *iH);
	}
	else if (objInfo.wstrStateKey.find(L"Tree") != wstring::npos)
	{
		m_eType = OBSTACLE_TREE;
	}
	/*else
	{
	iW = pTexInfo.tImgInfo.Width;
	iH = pTexInfo.tImgInfo.Height;
	vPos.x = (((int)vPos.x / iW) *iW);
	vPos.y = (((int)vPos.y / iH) *iH);
	}*/

}

void CObstacle::ConvertCenter(const OBJ_INFO & objInfo, const TEX_INFO & pTexInfo, float & cx, float & cy)
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

void CObstacle::CheckType()
{

	if (m_tObjInfo.wstrStateKey.find(L"TP") != wstring::npos)
	{
		m_eType = OBSTACLE_TP;

	}
	else if (m_tObjInfo.wstrStateKey.find(L"Horizontal_Wall") != wstring::npos)
	{
		m_eType = OBSTACLE_TP;

	}
	else if (m_tObjInfo.wstrStateKey.find(L"Vertical_Wall") != wstring::npos)
	{
		m_eType = OBSTACLE_TP;

	}
	else if (m_tObjInfo.wstrStateKey.find(L"Fence") != wstring::npos)
	{
		m_eType = OBSTACLE_FENCE;
	}
	else if (m_tObjInfo.wstrStateKey.find(L"Tree") != wstring::npos)
	{
		m_eType = OBSTACLE_TREE;
	}
	else if (m_tObjInfo.wstrStateKey.find(L"Plant") != wstring::npos)
	{
		m_eType = OBSTACLE_GRASS;
	}
	else if (m_tObjInfo.wstrStateKey.find(L"Prop") != wstring::npos)
	{
		m_eType = OBSTACLE_GRASS;
	}
	else if (m_tObjInfo.wstrStateKey.find(L"Lamp") != wstring::npos)
	{
		m_eType = OBSTACLE_LAMP;
	}
	else 
	{
	
		m_eType = OBSTACLE_END;
	}
}
