#include "stdafx.h"
#include "Weapon.h"
#include "Projectile.h"
#include "Mouse.h"
#include "ScrollMgr.h"
CWeapon::CWeapon()
	:m_wstrState(L"Idle_")
{
	m_tObjInfo.wstrObjectName = L"Weapon";
	m_tObjInfo.wstrObjectKey = L"Item";
	m_tObjInfo.wstrStateKey = L"Weapon_Javelin_Pickup_Unique";
	m_tObjInfo.IsAni = false;
	m_tObjInfo.ImageIDX = 0;
	m_tObjInfo.eObjectType = OBJECT_WEAPON;
	m_tFrame.fCurFrame = 0;
	Initialize();
}

CWeapon::CWeapon(D3DXVECTOR3 vPos,float fAtkSpeed, float fDamage)
	:m_wstrState(L"Idle_"),m_fAtkSpeed(fAtkSpeed), m_bIsAtkEnd(false)
{
	m_dwDir = DOWN;
	m_tObjInfo.wstrObjectName = L"Weapon";
	m_tObjInfo.wstrObjectKey = L"Item";
	m_tObjInfo.wstrStateKey = L"Weapon_Javelin_Pickup_Unique";
	m_tObjInfo.IsAni = true;
	m_tObjInfo.ImageIDX = 0;
	m_tObjInfo.eObjectType = OBJECT_WEAPON;
	m_tFrame.fCurFrame = 0;
	m_tInfo.vPos = vPos;
	m_tData.fDamage = fDamage;
	Initialize();
}


CWeapon::~CWeapon()
{
}

HRESULT CWeapon::Initialize()
{
	D3DXMatrixIdentity(&m_tInfo.matWorld); // 다이렉트 항등행렬 함수
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 1.f, 1.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	return S_OK;
}

void CWeapon::Release()
{
}

int CWeapon::Update()
{
	m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDelta()*m_fAtkSpeed;

	if (!m_bIsAtkEnd &&(int)(m_tFrame.fMaxFrame -m_tFrame.fCurFrame)==2
		&&m_wstrState.compare(L"Attack_") == 0)
	{
		
		m_tAtkVec = (CMouse::GetMousePos() + CScrollMgr::GetScrollPos()) - m_tInfo.vPos;
		//D3DXVec3Normalize(&m_tAtkVec, &m_tAtkVec);
		CProjectile* temp = new CProjectile(m_eWpType, m_tInfo.vPos, m_tAtkVec,m_tData.fDamage);
		CObjectMgr::GetInstance()->AddObject(OBJECT_PROJECTILE, temp);
		m_bIsAtkEnd = true;
	}
	if (m_tFrame.fMaxFrame <= m_tFrame.fCurFrame)
	{
		m_tFrame.fCurFrame = 0.f;
		m_wstrState = L"Idle_";
		m_bIsAtkEnd = false;
	
	}
	return 0;
}

void CWeapon::LateUpdate()
{
}

void CWeapon::Render()
{
	ConvertPos();
	UpdateMatWorld(1.0f, ZOrder(m_tInfo.vPos.y));
	wstring wstrStateKey = m_wstrState + m_tObjInfo.wstrStateKey + m_wsImgDir;
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_tObjInfo.wstrObjectKey,
		wstrStateKey, m_tFrame.fCurFrame);
	NULL_CHECK(pTexInfo);
	m_tFrame.fMaxFrame = pTexInfo->iMaxCnt;
	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CWeapon::AttackAni(WEAPONE_TYPE eType)
{
	m_eWpType = eType;
	m_tFrame.fCurFrame = 0;
	m_wstrState = L"Attack_";
	switch (eType)
	{//요기
	case WEAPONE_FIST:
		m_tObjInfo.wstrStateKey = L"";
		break;
	case WEAPONE_JAVELIN:
		m_tObjInfo.wstrStateKey = L"Javelin1_";
		break;
	case WEAPONE_JAVELIN1:
		m_tObjInfo.wstrStateKey = L"Jevelin2_";
		break;
	case WEAPONE_BOW:
		m_tObjInfo.wstrStateKey = L"Bow1_";
		break;
	case WEAPONE_BOW1:
		m_tObjInfo.wstrStateKey = L"Bow2_";
		break;
	case WEAPONE_END:
		break;
	default:
		m_tObjInfo.wstrStateKey = L"Javelin1_";

		break;
	}
}

void CWeapon::ConvertPos()
{
	if (m_eWpType == WEAPONE_JAVELIN || m_eWpType == WEAPONE_JAVELIN1)
	{
	//cout << m_eWpType << endl;
		if (m_dwDir == UP)
		{
			m_tInfo.vPos.x += 4;
			m_tInfo.vPos.y -= 8;

		}
		else if (m_dwDir == LEFT)
		{
			m_tInfo.vPos.y -= 2;
			m_tInfo.vPos.x -= 6;

		}
		else if (m_dwDir == RIGHT)
		{
			m_tInfo.vPos.y -= 2;
			m_tInfo.vPos.x += 6;

		}
		else
		{
			m_tInfo.vPos.y += 15;
			m_tInfo.vPos.x -= 2;

		}
	}

	if (m_eWpType == WEAPONE_BOW || m_eWpType == WEAPONE_BOW1)
	{
		if (m_dwDir == UP)
		{
			m_tInfo.vPos.x += 4;
			m_tInfo.vPos.y -= 8;

		}
		else if (m_dwDir == LEFT)
		{
			//m_tInfo.vPos.y -= 2;
			m_tInfo.vPos.x -= 9;

		}
		else if (m_dwDir == RIGHT)
		{
			//m_tInfo.vPos.y -= 2;
			m_tInfo.vPos.x += 9;

		}
		else
		{
			m_tInfo.vPos.y += 12;
			//m_tInfo.vPos.x -= 2;

		}
	}

}
