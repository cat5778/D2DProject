#include "stdafx.h"
#include "ScrollMgr.h"

#include "Mouse.h"
#include "TimeMgr.h"
D3DXVECTOR3 CScrollMgr::m_vScrollPos = { 0.f, 0.f, 0.f };
D3DXVECTOR3 CScrollMgr::m_vScreenCenter = { WINCX*0.5f, WINCY*0.5f, 0 };
D3DXVECTOR3 CScrollMgr::m_CenterPos = { WINCX*0.5f, WINCY*0.5f, 0 };
CScrollMgr::CScrollMgr()
	:m_pDeviceMgr(CDeviceMgr::GetInstance())
{
}


CScrollMgr::~CScrollMgr()
{
}

const D3DXVECTOR3& CScrollMgr::GetScrollPos()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_vScrollPos;
}

const D3DXVECTOR3& CScrollMgr::GetCenterPos()
{
	return m_CenterPos;
}

const void CScrollMgr::ScrollPos(float fSpeed)
{
	D3DXVECTOR3 ScrollPos = CScrollMgr::GetScrollPos();
	D3DXVECTOR3 MousePos = CMouse::GetMousePos();

	float fMoveSpeed = fSpeed * CTimeMgr::GetInstance()->GetDelta();
	
	if (CKeyManager::GetInstance()->KeyPressing(KEY_A) && ScrollPos.x>0)
		CScrollMgr::MoveScrollPos(D3DXVECTOR3(-fMoveSpeed, 0.f, 0.f));
	if (CKeyManager::GetInstance()->KeyPressing(KEY_W) && ScrollPos.y>0)
		CScrollMgr::MoveScrollPos(D3DXVECTOR3(0.f, -fMoveSpeed, 0.f));
	if (CKeyManager::GetInstance()->KeyPressing(KEY_D) && ScrollPos.x<500)
		CScrollMgr::MoveScrollPos(D3DXVECTOR3(fMoveSpeed, 0.f, 0.f));
	if (CKeyManager::GetInstance()->KeyPressing(KEY_S) && ScrollPos.y<1000)
		CScrollMgr::MoveScrollPos(D3DXVECTOR3(0.f, fMoveSpeed, 0.f));

}

const void CScrollMgr::SetCamPos(D3DXVECTOR3 vPos)
{
	MoveScrollPos(vPos*0.5);
}

const void CScrollMgr::FollowCam(D3DXVECTOR3 vPos,float fSpeed)
{
	float fMoveSpeed = fSpeed * CTimeMgr::GetInstance()->GetDelta();

	if (CKeyManager::GetInstance()->KeyPressing(KEY_A) && vPos.x<= m_CenterPos.x&& vPos.x>=WINCX*0.5f)
		CScrollMgr::MoveScrollPos(D3DXVECTOR3(-fMoveSpeed, 0.f, 0.f));
	if (CKeyManager::GetInstance()->KeyPressing(KEY_D) && vPos.x >= m_CenterPos.x&& vPos.x <= WINCX*0.9f)
		CScrollMgr::MoveScrollPos(D3DXVECTOR3(+fMoveSpeed, 0.f, 0.f));
	if (CKeyManager::GetInstance()->KeyPressing(KEY_S) && vPos.y >= m_CenterPos.y&&vPos.y<=WINCY*1.75f)
		CScrollMgr::MoveScrollPos(D3DXVECTOR3(0.f, fMoveSpeed, 0.f));
	if (CKeyManager::GetInstance()->KeyPressing(KEY_W) && vPos.y <= m_CenterPos.y&& vPos.y >= WINCY*0.5f)
		CScrollMgr::MoveScrollPos(D3DXVECTOR3(0.f, -fMoveSpeed, 0.f));
}

void CScrollMgr::MoveScrollPos(const D3DXVECTOR3& vMove)
{
	m_vScrollPos += vMove;
	m_CenterPos = m_vScrollPos + m_vScreenCenter;
}

