#include "stdafx.h"
#include "TextField.h"
#include "ColliderBox.h"
#include "Choose.h"
#include "Mouse.h"
CTextField::CTextField()
	: m_vSize(0, 0), m_CollBox(nullptr), m_IsPick(false)
{
}

CTextField::CTextField(D3DXVECTOR3 vPos)
	: m_vSize(0, 0), m_CollBox(nullptr), m_IsPick(false)
{
	//m_tObjInfo = objInfo;

	m_tInfo.vPos = vPos;
	Initialize();


}


CTextField::~CTextField()
{
}

HRESULT CTextField::Initialize()
{
	// 행렬 초기화 -> 항등행렬(I)
	//CMathMgr::MyMatrixIdentity(&m_tInfo.matWorld);
	D3DXMatrixIdentity(&m_tInfo.matWorld); // 다이렉트 항등행렬 함수

	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 0.f, -1.f, 0.f };
	m_tInfo.vSize = { 1.3f, 2.5f, 0.f };

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 10.f;
	m_tObjInfo.ImageIDX = 0;
	return S_OK;
}

void CTextField::Release()
{
}

int CTextField::Update()
{
	if (m_CollBox != nullptr)
	{
		//m_CollBox->SetvPos(m_tInfo.vPos);
		m_CollBox->Update();
	}
	
	//auto Chooselist = CObjectMgr::GetInstance()->GetObjList(OBJECT_CHOOSE);
	//for (auto pChoose : Chooselist)
	//{
	//	if (dynamic_cast<CChoose*>(pChoose)->GetIsChoose())
	//	{
	//		//m_IsPick = true;
	//			break;
	//	}
	//	//m_IsPick = false;
	//}


	return 0;
}

void CTextField::LateUpdate()
{
}

void CTextField::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"UI",L"Text_Field", m_tObjInfo.ImageIDX);
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
		m_tInfo.vPos.x,
		m_tInfo.vPos.y,
		0.1); // 다이렉트 이동행렬 함수
	m_tInfo.matWorld = matScale * matTrans;

	D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vLook, &m_tInfo.matWorld);
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);



	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	if (m_vSize.x != 0 && m_CollBox == nullptr)
	{
		m_CollBox = new CColliderBox(m_tInfo.vPos, BUTTON_COLLISION, m_vSize);
	}
	if (m_CollBox != nullptr&&m_CollBox->IsPicking(CMouse::GetMousePos()))
	{
		//m_CollBox->Render();
		bool bIsChoose = false;
		auto chooseList = CObjectMgr::GetInstance()->GetObjList(OBJECT_CHOOSE);
		for (auto pChoose : chooseList)
		{
			CChoose* temp = dynamic_cast<CChoose*>(pChoose);
			if (temp != nullptr&&temp->GetIsChoose())
			{
				bIsChoose = true;
				break;
			}

		}
		if (CKeyManager::GetInstance()->KeyDown(KEY_LBUTTON)&& bIsChoose)
		{
			m_IsPick = true;
			m_tObjInfo.ImageIDX = 1;
		}
	}

	//if(m_IsPick)
	//{
	//	m_tObjInfo.ImageIDX = 1;
	//}
	//else
	//{
	//	m_tObjInfo.ImageIDX = 0;

	//}

}
