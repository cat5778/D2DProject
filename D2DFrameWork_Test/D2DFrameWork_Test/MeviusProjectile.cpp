#include "stdafx.h"
#include "MeviusProjectile.h"
#include "Mouse.h"
#include "ScrollMgr.h"
#include "ColliderBox.h"
#include "Effect.h"

CMeviusProjectile::CMeviusProjectile()
{
}



CMeviusProjectile::CMeviusProjectile(D3DXVECTOR3 vPos, float fRadian, float fDamage)
	: m_fSpeed(300), m_IsDead(false), m_fTime(0.f), m_CollBox(nullptr)
{
	m_fRadian = fRadian;
	m_eProjectileType = MONSTER_PROJECTILE_COLLISION;
	m_tInfo.vPos = vPos;
	m_tData.fDamage = fDamage;
	m_bIsCollsion = false;
	m_tObjInfo.wstrObjectKey = L"Mevius";
	m_tObjInfo.wstrStateKey = L"Spike_Ball";
	m_tObjInfo.IsAni = true;
	m_tFrame.fCurFrame = 0;
	Initialize();
}

CMeviusProjectile::~CMeviusProjectile()
{
}

HRESULT CMeviusProjectile::Initialize()
{
	ZeroMemory(&m_vDir, sizeof(D3DXVECTOR3));
	D3DXMatrixIdentity(&m_tInfo.matWorld); // 다이렉트 항등행렬 함수
										   //m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	return S_OK;
}

void CMeviusProjectile::Release()
{
}

int CMeviusProjectile::Update()
{
	if (!m_IsDead)
	{
		Animate();

		if (m_fTime <= 2.0f)
			m_fTime += m_pTimeMgr->GetDelta();
		else
		{
			m_IsDead = true;
			if (m_CollBox != nullptr)
				m_CollBox->EraseCollider();
		}

		m_tInfo.vPos.x += cosf(m_fRadian)*m_fSpeed*m_pTimeMgr->GetDelta();
		m_tInfo.vPos.y += sinf(m_fRadian)*m_fSpeed*m_pTimeMgr->GetDelta();

		if (m_CollBox != nullptr)
		{
			m_CollBox->SetDamage(m_tData.fDamage);
			m_bIsCollsion = m_CollBox->IsCollsion();
			m_CollBox->SetvPos(m_tInfo.vPos);
			if (m_bIsCollsion)
			{
				m_CollBox->EraseCollider();
				return DEAD_OBJ;
			}
		}
		return NO_EVENT;
	}
}

void CMeviusProjectile::LateUpdate()
{
}

void CMeviusProjectile::Render()
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

			m_vSize.x = pTexInfo->tImgInfo.Width *  0.25f;
			m_vSize.y = pTexInfo->tImgInfo.Height * 0.25f;


			m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
			m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(m_vSize.x, m_vSize.y, 0.f),
				nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
			if (m_CollBox == nullptr)
			{
				m_CollBox = new CColliderBox(m_tInfo.vPos, m_eProjectileType, m_vSize, m_vDir);
				m_pColliderMgr->AddObject(m_eProjectileType, m_CollBox);
			}
		}

	}
}
