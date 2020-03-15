#include "stdafx.h"
#include "StormProjectile.h"
#include "ColliderBox.h"
#include "Mouse.h"
#include "ScrollMgr.h"
CStormProjectile::CStormProjectile()
{
}

CStormProjectile::CStormProjectile(D3DXVECTOR3 vPos, float fDamage, float fTime, COLLSION_TYPE eCollType, wstring wstrObjectKey, wstring wstrStateKey, bool IsAni)
	:m_IsDead(false), m_fTime(0.f), m_CollBox(nullptr)
{
	m_fDeleteTime = fTime;
	m_eProjectileType = eCollType;
	m_tInfo.vPos = vPos;
	m_tData.fDamage = fDamage;
	m_bIsCollsion = false;
	m_tObjInfo.wstrObjectKey = wstrObjectKey;
	m_tObjInfo.wstrStateKey = wstrStateKey;
	m_tObjInfo.IsAni = IsAni;
	m_tFrame.fCurFrame = 0;
	Initialize();

}


CStormProjectile::~CStormProjectile()
{
}

HRESULT CStormProjectile::Initialize()
{
	D3DXMatrixIdentity(&m_tInfo.matWorld); // 다이렉트 항등행렬 함수
	m_tInfo.vDir = { 0.f,0.f,0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	return S_OK;
}

void CStormProjectile::Release()
{
}

int CStormProjectile::Update()
{
	if (!m_IsDead)
	{

		Animate();
		if (m_fTime <= m_fDeleteTime)
			m_fTime += m_pTimeMgr->GetDelta();
		else
		{
			m_IsDead = true;
			if (m_CollBox != nullptr)
				m_CollBox->EraseCollider();
		}
		if (m_CollBox != nullptr)
		{
			m_CollBox->SetDamage(m_tData.fDamage);
			m_bIsCollsion = m_CollBox->IsCollsion();
			m_CollBox->SetvPos(m_tInfo.vPos);

		}
		return NO_EVENT;
	}

	return DEAD_OBJ;
}

void CStormProjectile::LateUpdate()
{
}

void CStormProjectile::Render()
{
	if (!m_IsDead)
	{
		UpdateMatWorld(1.0f, ZOrder(m_tInfo.vPos.y), m_fRadian);
		//cout << m_fRadian << endl;
		if (m_tObjInfo.IsAni)
		{
			const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_tObjInfo.wstrObjectKey,
				m_tObjInfo.wstrStateKey, m_tFrame.fCurFrame);
			NULL_CHECK(pTexInfo);

			m_tFrame.fMaxFrame = pTexInfo->iMaxCnt;
			if (m_eProjectileType == MONSTER_PROJECTILE_COLLISION)
			{
				m_vSize.x = pTexInfo->tImgInfo.Width *  0.25f;
				m_vSize.y = pTexInfo->tImgInfo.Height * 0.05f;
			}
			else
			{
				m_vSize.x = pTexInfo->tImgInfo.Width * 0.25f;
				m_vSize.y = pTexInfo->tImgInfo.Height * 0.05f;
			}
			m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
			m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(m_vSize.x, m_vSize.y, 0.f),
				nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
			if (m_CollBox == nullptr)
			{
				m_vDir = { 0.f,0.f,0.f };
				m_CollBox = new CColliderBox(m_tInfo.vPos, m_eProjectileType, D3DXVECTOR2(50,50), m_vDir);
				m_pColliderMgr->AddObject(m_eProjectileType, m_CollBox);
			}
		}
		else
		{
			if (m_CollBox == nullptr)
			{
				m_vDir = { 0.f,0.f,0.f };
				m_CollBox = new CColliderBox(m_tInfo.vPos, m_eProjectileType, m_vSize, m_vDir);
				m_pColliderMgr->AddObject(m_eProjectileType, m_CollBox);
			}
		}
		//if (CKeyManager::GetInstance()->KeyPressing(KEY_6)&& m_CollBox != nullptr)
		//{
		//m_CollBox->RenderLine();
		//}
	}
}
