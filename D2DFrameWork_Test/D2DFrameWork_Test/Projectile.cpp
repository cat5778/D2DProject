#include "stdafx.h"
#include "Projectile.h"
#include "Mouse.h"
#include "ScrollMgr.h"
#include "ColliderBox.h"
#include "SkillProjectTile.h"
#include "Effect.h"
#include "StormProjectile.h"
CProjectile::CProjectile()
{
	
}

CProjectile::CProjectile(WEAPONE_TYPE eType, D3DXVECTOR3 vPos, D3DXVECTOR3 vDir,float fDamage,
	COLLSION_TYPE eCollType, wstring wstrObjectKey, wstring wstrStateKey, bool IsAni)
	:m_eWpType(eType), m_fSpeed(400), m_IsDead(false),m_fTime(0.f), m_CollBox(nullptr)
{
	m_eProjectileType = eCollType;
	m_tInfo.vPos = vPos;
	m_tData.fDamage = fDamage;
	m_bIsCollsion = false;
	m_tObjInfo.wstrObjectKey = wstrObjectKey;
	m_tObjInfo.wstrStateKey = wstrStateKey;
	m_tObjInfo.IsAni = IsAni;
	m_tFrame.fCurFrame = 0;
	m_vDir = vDir;
	
	switch (m_eProjectileType)
	{
	case PLAYER_PROJECTILE_COLLISION:
		PlayerAngle();
		break;
	case MONSTER_PROJECTILE_COLLISION:
		MonsterAngle();
		break;
	case COLLSION_END:
		break;
	default:
		break;
	}
	//m_vDir = (CMouse::GetMousePos() + CScrollMgr::GetScrollPos()) - m_tInfo.vPos;
	//D3DXVec3Normalize(&m_vDir, &m_vDir);
	//D3DXVECTOR3 tempPos = m_tInfo.vPos;
	//tempPos.x += 1;
	//D3DXVECTOR3 LookPos = tempPos - m_tInfo.vPos;
	//D3DXVec3Normalize(&LookPos, &LookPos);
	//m_fRadian = acosf(D3DXVec3Dot(&LookPos, &m_vDir));
	//if (m_tInfo.vPos.y > (CMouse::GetMousePos().y + CScrollMgr::GetScrollPos().y))
	//	m_fRadian *= -1.f;
	Initialize();
}


CProjectile::~CProjectile()
{
	Release();
}

HRESULT CProjectile::Initialize()
{
	D3DXMatrixIdentity(&m_tInfo.matWorld); // 다이렉트 항등행렬 함수
	//m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };

	
	return S_OK;
}

void CProjectile::Release()
{
	if (g_PlayerSkillData.iBuffLv>=1)
	{
		int i1 = rand() % 5;
		if (m_eProjectileType == PLAYER_PROJECTILE_COLLISION&&i1<2)
		{
			float iAngle = rand() % 36 + 1;

			iAngle *= 10;
			float fRadian = iAngle / 180 * PI;

			CSkillProjectTile* temp = new CSkillProjectTile(m_eWpType, m_tInfo.vPos, fRadian, m_tData.fDamage);
			CObjectMgr::GetInstance()->AddObject(OBJECT_PROJECTILE, temp);

			if (g_PlayerSkillData.iThunderLv >= 1)
			{
				D3DXVECTOR3 convert = m_tInfo.vPos;
				convert.x -= 10;
				CEffect* pStomp = new CEffect(convert, L"Effect", L"Stomp");
				CObjectMgr::GetInstance()->AddObject(OBJECT_EFFECT, pStomp);

				CStormProjectile* pStorm = new CStormProjectile(m_tInfo.vPos,
					m_tData.fDamage, 1.0f, PLAYER_PROJECTILE_COLLISION, L"Effect"
					, L"Storm");
				CObjectMgr::GetInstance()->AddObject(OBJECT_PROJECTILE, pStorm);
			}
		}

	}
}

int CProjectile::Update()
{
	if (!m_IsDead)
	{
		
		Animate();
		//cout << m_tFrame.fCurFrame << endl;
		if (m_fTime <= 1.0f)
			m_fTime += m_pTimeMgr->GetDelta();
		else
		{
			m_IsDead = true;
			if(m_CollBox!=nullptr)
				m_CollBox->EraseCollider();
		}
		D3DXVECTOR3 vNmalDir;
		D3DXVec3Normalize(&vNmalDir, &m_vDir);
		m_tInfo.vPos += vNmalDir*m_fSpeed* m_pTimeMgr->GetDelta();
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


	return DEAD_OBJ;
}

void CProjectile::LateUpdate()
{
}

void CProjectile::Render()
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
				m_vSize.y = pTexInfo->tImgInfo.Height * 0.25f;
			}
			else
			{
				m_vSize.x = pTexInfo->tImgInfo.Width *  0.25f;
				m_vSize.y = pTexInfo->tImgInfo.Height * 0.25f;
			}
			m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
			m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(m_vSize.x, m_vSize.y, 0.f),
				nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
			if (m_CollBox == nullptr)
			{
				m_CollBox = new CColliderBox(m_tInfo.vPos, m_eProjectileType, m_vSize, m_vDir);
				m_pColliderMgr->AddObject(m_eProjectileType, m_CollBox);
			}
		}
		else
		{
			if (m_CollBox == nullptr)
			{
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

void CProjectile::PlayerAngle()
{
	switch (m_eWpType)
	{
	case WEAPONE_FIST:
		m_tObjInfo.wstrStateKey = L"";
		break;
	case WEAPONE_JAVELIN:
		m_tObjInfo.wstrStateKey = L"Javelin_1";
		break;
	case WEAPONE_JAVELIN1:
		m_tObjInfo.wstrStateKey = L"Javelin_2";
		break;
	case WEAPONE_BOW:
	case WEAPONE_BOW1:
		m_tObjInfo.wstrStateKey = L"Arrow";
		break;
	case WEAPONE_END:
		break;
	default:
		m_tObjInfo.wstrStateKey = L"";
		break;
	}
	//m_vDir = (CMouse::GetMousePos() + CScrollMgr::GetScrollPos()) - m_tInfo.vPos;
	//D3DXVec3Normalize(&m_vDir, &m_vDir);
	D3DXVECTOR3 vNmalDir;
	D3DXVec3Normalize(&vNmalDir, &m_vDir);
	D3DXVECTOR3 tempPos = m_tInfo.vPos;
	tempPos.x += 1;
	D3DXVECTOR3 LookPos = tempPos - m_tInfo.vPos;
	D3DXVec3Normalize(&LookPos, &LookPos);
	m_fRadian = acosf(D3DXVec3Dot(&LookPos, &vNmalDir));
	if (m_tInfo.vPos.y > (CMouse::GetMousePos().y + CScrollMgr::GetScrollPos().y))
		m_fRadian *= -1.f;
}

void CProjectile::MonsterAngle()
{

	//D3DXVec3Normalize(&m_vDir, &m_vDir);
	//D3DXVec3Normalize(&m_vDir, &m_vDir);
	D3DXVECTOR3 vNmalDir;
	D3DXVec3Normalize(&vNmalDir, &m_vDir);
	D3DXVECTOR3 tempPos = m_tInfo.vPos;
	tempPos.x += 1;
	D3DXVECTOR3 LookPos = tempPos - m_tInfo.vPos;
	D3DXVec3Normalize(&LookPos, &LookPos);
	m_fRadian = acosf(D3DXVec3Dot(&LookPos, &vNmalDir));
	if (m_tInfo.vPos.y > vNmalDir.y)
		m_fRadian *= -1.f;
}
