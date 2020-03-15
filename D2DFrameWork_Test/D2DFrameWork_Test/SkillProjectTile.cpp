#include "stdafx.h"
#include "SkillProjectTile.h"
#include "Mouse.h"
#include "ScrollMgr.h"
#include "ColliderBox.h"
#include "Effect.h"
#include "StormProjectile.h"
CSkillProjectTile::CSkillProjectTile()
{
}

CSkillProjectTile::CSkillProjectTile(WEAPONE_TYPE eType, D3DXVECTOR3 vPos, D3DXVECTOR3 vDir,
	float fDamage, COLLSION_TYPE eCollType, wstring wstrObjectKey, wstring wstrStateKey, bool IsAni)
	:m_eWpType(eType), m_fSpeed(400), m_IsDead(false), m_fTime(0.f), m_CollBox(nullptr)
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
	Initialize();
}

CSkillProjectTile::CSkillProjectTile(WEAPONE_TYPE eType, D3DXVECTOR3 vPos, float fRadian, float fDamage)
	:m_eWpType(eType), m_fSpeed(400), m_IsDead(false), m_fTime(0.f), m_CollBox(nullptr)
{
	m_fRadian = fRadian;
	m_eProjectileType = PLAYER_PROJECTILE_COLLISION;
	m_tInfo.vPos = vPos;
	m_tData.fDamage = fDamage;
	m_bIsCollsion = false;
	m_tObjInfo.wstrObjectKey = L"Projectile";
	m_tObjInfo.wstrStateKey = L"";
	m_tObjInfo.IsAni = true;
	m_tFrame.fCurFrame = 0;
	Initialize();
}


CSkillProjectTile::~CSkillProjectTile()
{
	Release();
}

HRESULT CSkillProjectTile::Initialize()
{
	ZeroMemory(&m_vDir, sizeof(D3DXVECTOR3));
	D3DXMatrixIdentity(&m_tInfo.matWorld); // 다이렉트 항등행렬 함수
										   //m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
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

	//D3DXVECTOR3 temp = m_tInfo.vPos;
	//temp.x += 1;
	//temp = temp - m_tInfo.vPos;
	//D3DXVec3Normalize(&temp, &temp);

	//m_vDir.x = temp.x + cosf(m_fRadian);
	//m_vDir.y = temp.y + sinf(m_fRadian);
	//D3DXVec3Normalize(&m_vDir, &m_vDir);
	//m_tInfo.vPos 
	return S_OK;
}

void CSkillProjectTile::Release()
{
	if (g_PlayerSkillData.iFinish >= 1)
	{
		int i1 = rand() % 5;
		if (i1<2)
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

int CSkillProjectTile::Update()
{
	if (!m_IsDead)
	{
		Animate();

		if (m_fTime <= 1.0f)
			m_fTime += m_pTimeMgr->GetDelta();
		else
		{
			m_IsDead = true;
			if (m_CollBox != nullptr)
				m_CollBox->EraseCollider();
		}
		//D3DXVECTOR3 vNmalDir;
	
		m_tInfo.vPos.x += cosf(m_fRadian)*m_fSpeed*m_pTimeMgr->GetDelta();
		m_tInfo.vPos.y += sinf(m_fRadian)*m_fSpeed*m_pTimeMgr->GetDelta();


		//m_tInfo.vPos += m_vDir*m_fSpeed* m_pTimeMgr->GetDelta();
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

void CSkillProjectTile::LateUpdate()
{
}

void CSkillProjectTile::Render()
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

	}
}
