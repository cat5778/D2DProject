#include "stdafx.h"
#include "GameObject.h"
#include "ScrollMgr.h"
#include "Mouse.h"

CGameObject::CGameObject()
	: m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pTimeMgr(CTimeMgr::GetInstance()),
	m_pColliderMgr(ColliderMgr::GetInstance()),
	m_dwDir(0x00000001),m_ColliderBox(nullptr), m_bIsCollsion(false), m_fTimer(0)
{
}


CGameObject::~CGameObject()
{
}

void CGameObject::SetCollision(bool bIsColl, COLLSION_TYPE CollType, float fDamage)
{

	m_bIsCollsion = bIsColl; 
	m_HitCollType = CollType;
	if (false)
	{
		m_dwCollDir=-1;
	}
	 if (fDamage > 0)
		 BeAttack(fDamage);
}

void CGameObject::SetOBJCollision(bool bIsColl, COLLSION_TYPE CollType, float fDamage)
{
	m_bIsObjCollision = bIsColl;
	//m_HitCollType = CollType;
}

void CGameObject::SetvPos(D3DXVECTOR3 vPos, DWORD Dir)
{

	GetImageDir(Dir);
	m_tInfo.vPos = vPos;
	
}

void CGameObject::Timer(bool& SetBool, float fRate, float& fTime)
{
	(SetBool) ? fTime += m_pTimeMgr->GetDelta() : fTime = 0.f;
	if (fTime >= fRate)
	{
		SetBool = false;
		fTime = 0.f;
	}
}

DWORD CGameObject::GetCollDir(D3DXVECTOR3 vCollDir)
{

	D3DXVECTOR3 tempNmlColDir=vCollDir- m_tInfo.vPos;
	D3DXVec3Normalize(&tempNmlColDir, &tempNmlColDir);

	D3DXVECTOR3 temp = m_tInfo.vPos;
	temp.x += 1;
	temp = temp - m_tInfo.vPos;
	D3DXVec3Normalize(&temp, &temp);
	m_fCollRadian =acosf(D3DXVec3Dot(&temp, &tempNmlColDir));
	if (m_tInfo.vPos.y > vCollDir.y)
		m_fCollRadian *= -1.f;
	float fAngle = m_fCollRadian / PI * 180;
	//cout <<"각도="<< fAngle << endl;
	if (fAngle >= -45.f&&fAngle <= 45.f)
		m_dwCollDir = RIGHT;
	else if (fAngle >= 45 && fAngle <= 135)
		m_dwCollDir = DOWN;
	else if ((fAngle >= -180.f&& fAngle <= -135)
		|| (fAngle <= 180.f&&fAngle >= 135))
		m_dwCollDir = LEFT;
	else if (fAngle >= -135.f&&fAngle <= -45)
		m_dwCollDir = UP;
	return m_dwCollDir;
}

bool CGameObject::Animate(bool bISInfinite,float fSpeed)
{
	if (bISInfinite)
	{
		m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDelta()*fSpeed;
		if (m_tFrame.fMaxFrame <= m_tFrame.fCurFrame)
			m_tFrame.fCurFrame = 0.f;
	}
	else
	{
		m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDelta()*fSpeed;
		if (m_tFrame.fMaxFrame <= m_tFrame.fCurFrame)
		{
			m_tFrame.fCurFrame = 0.f;
			return true;
		}
	}
	return false;
}

void CGameObject::UpdateMatWorld(float fScale, float fZLayer,float fAngle)
{

	D3DXMATRIX matScale, matTrans,matRot;
	D3DXMatrixScaling(&matScale,
		m_tInfo.vSize.x*fScale,
		m_tInfo.vSize.y*fScale,
		0.f); // 다이렉트 크기행렬 함수
	if (fZLayer == 1)
	{
		D3DXMatrixTranslation(&matTrans,
			m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x,
			m_tInfo.vPos.y - CScrollMgr::GetScrollPos().y,
			ZOrder(m_tInfo.vPos.y - CScrollMgr::GetScrollPos().y)); // 다이렉트 이동행렬 함수
	}
	else
	{
		D3DXMatrixTranslation(&matTrans,
			m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x,
			m_tInfo.vPos.y - CScrollMgr::GetScrollPos().y,
			fZLayer); // 다이렉트 이동행렬 함수

	}
	//if (fAngle != 0.f)
	D3DXMatrixRotationZ(&matRot, fAngle);
	m_tInfo.matWorld = matScale *matRot* matTrans;

	D3DXVec3TransformNormal(&m_tInfo.vDir,&m_tInfo.vLook, &m_tInfo.matWorld);
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
}


float CGameObject::ZOrder(float _y)
{
	return 1 - (_y *0.00001f);
}

void CGameObject::GetImageDir(DWORD dwDir)
{
	if (m_dwDir != dwDir)
		m_tFrame.fCurFrame = 0;

	m_dwDir = dwDir;
	if (dwDir & DOWN)
		m_wsImgDir = L"_Down";
	else if (dwDir & LEFT)
		m_wsImgDir = L"_Left";
	else if (dwDir & UP)
		m_wsImgDir = L"_Up";
	else if (dwDir & RIGHT)
		m_wsImgDir = L"_Right";

}

void CGameObject::GetAngle(D3DXVECTOR3 targetPos)
{
	D3DXVECTOR3 tempNmlColDir;
	D3DXVec3Normalize(&tempNmlColDir, &targetPos);

	D3DXVECTOR3 temp = m_tInfo.vPos;
	temp.x += 1;
	temp = temp - m_tInfo.vPos;
	D3DXVec3Normalize(&temp, &temp);
	m_fRadian = acosf(D3DXVec3Dot(&temp, &tempNmlColDir));
	if (m_tInfo.vPos.y > targetPos.y)
		m_fRadian *= -1.f;
	//cout << m_tInfo.vPos.y <<" , "<<targetPos.y << endl; 
	//cout << "각도" << m_fRadian / PI * 180 << endl;


}

void CGameObject::ChangeState(OBJECT_STATE eState)
{
	if (m_eOldState != eState)
	{
		m_eOldState = eState;
		switch (eState)
		{
		case STATE_IDLE:
			m_eCurState = STATE_IDLE;
			break;
		case STATE_ATTACK:
			m_eCurState = STATE_ATTACK;
			break;
		case STATE_MOVE:
			m_eCurState = STATE_MOVE;
			break;
		case STATE_BEATTACK:
			m_eCurState = STATE_BEATTACK;
			break;
		case STATE_DEAD:
		{
			m_eCurState = STATE_DEAD;
			m_tFrame.fCurFrame = 0;
		}
			break;
		case STATE_END:
			m_eCurState = STATE_END;
			break;
		default:
			break;
		}
		
	}
}

void CGameObject::KnockBack(D3DXVECTOR3 vKnock, float fPower)
{
	//m_vKnockDir = temp - m_vKnockDir;
	D3DXVECTOR3 temp;
	D3DXVec3Normalize(&temp, &vKnock);

	m_tInfo.vPos += temp*fPower* m_pTimeMgr->GetDelta();
	//m_tInfo.vPos = ;
}

void CGameObject::UpdateRect()
{
	
	m_tRect.left = { m_tInfo.vPos.x - m_vSize.x*0.5f,m_tInfo.vPos.y, 0 };
	m_tRect.top = { m_tInfo.vPos.x, m_tInfo.vPos.y - m_vSize.y*0.5f, 0 };
	m_tRect.right = { m_tInfo.vPos.x + m_vSize.x*0.5f, m_tInfo.vPos.y, 0 };
	m_tRect.bottom = { m_tInfo.vPos.x, m_tInfo.vPos.y + m_vSize.y*0.5f,0 };

}

