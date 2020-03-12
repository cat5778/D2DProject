#include "stdafx.h"
#include "GameObject.h"
#include "ScrollMgr.h"
#include "Mouse.h"

CGameObject::CGameObject()
	: m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pTimeMgr(CTimeMgr::GetInstance()),
	m_pColliderMgr(ColliderMgr::GetInstance()),
	m_dwDir(0x00000001),m_ColliderBox(nullptr), m_bIsCollsion(false)
{
}


CGameObject::~CGameObject()
{
}

void CGameObject::SetCollision(bool bIsColl, COLLSION_TYPE CollType, float fDamage)
{
	 m_bIsCollsion = bIsColl; 
	 m_HitCollType = CollType;
	 if (fDamage > 0)
		 BeAttack(fDamage);
}

void CGameObject::SetvPos(D3DXVECTOR3 vPos, DWORD Dir)
{

	GetImageDir(Dir);
	m_tInfo.vPos = vPos;
	
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
	m_dwDir = dwDir;
	if (dwDir & DOWN)
		m_wsImgDir = L"Down";
	else if (dwDir & LEFT)
		m_wsImgDir = L"Left";
	else if (dwDir & UP)
		m_wsImgDir = L"Up";
	else if (dwDir & RIGHT)
		m_wsImgDir = L"Right";

}

void CGameObject::GetAngle(D3DXVECTOR3 targetPos)
{
	D3DXVECTOR3 vDir = targetPos - m_tInfo.vPos;
	D3DXVec3Normalize(&vDir, &vDir);

	D3DXVECTOR3 tempPos = m_tInfo.vPos ;
	tempPos.x += 1;
	
	D3DXVECTOR3 LookPos = tempPos - m_tInfo.vPos;
	D3DXVec3Normalize(&LookPos, &LookPos);

	m_fRadian = acosf(D3DXVec3Dot(&LookPos, &vDir));
	
	if (m_tInfo.vPos.y > (CMouse::GetMousePos().y + CScrollMgr::GetScrollPos().y))
		m_fRadian *= -1.f;


}

