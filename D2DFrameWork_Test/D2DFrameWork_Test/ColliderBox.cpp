#include "stdafx.h"
#include "ColliderBox.h"
#include "ScrollMgr.h"

CColliderBox::CColliderBox()
{
}

CColliderBox::CColliderBox(D3DXVECTOR3 &vPos, COLLSION_TYPE eType, D3DXVECTOR2 vSize, D3DXVECTOR3 vDir, bool infinite)
	:m_eCollType(eType), m_bIsInfinite(infinite), m_bIsDeadObj(false)
{
	ZeroMemory(&m_tData, sizeof(GAME_DATA));
	m_vDir = vDir;
	m_HitCollType = COLLSION_END;
	m_bIsCollsion = false;
	m_bIsObjCollision = false;
	m_vSize=vSize;
	m_tInfo.vPos = vPos;
	Initialize();
	
}



CColliderBox::~CColliderBox()
{
}

HRESULT CColliderBox::Initialize()
{
	m_bIsInfinite ? m_bIsActive = true : m_bIsActive = false;
	// ��� �ʱ�ȭ -> �׵����(I)
	//CMathMgr::MyMatrixIdentity(&m_tInfo.matWorld);
	D3DXMatrixIdentity(&m_tInfo.matWorld); // ���̷�Ʈ �׵���� �Լ�

	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 0.f, -1.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };

	return S_OK;
}

void CColliderBox::Release()
{
}

int CColliderBox::Update()
{
	if (m_bIsDeadObj)
		return DEAD_OBJ;
	
	
	
	return NO_EVENT;
}

void CColliderBox::LateUpdate()
{

}

void CColliderBox::Render()
{

	//m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
}

void CColliderBox::RenderLine()
{
	UpdateMatWorld(1.0f, 0.1f);
	D3DXMATRIX matScale, matTrans;

	D3DXMatrixScaling(&matScale,
		m_tInfo.vSize.x,
		m_tInfo.vSize.y,
		0.f); // ���̷�Ʈ ũ����� �Լ�
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x ,
		m_tInfo.vPos.y ,
		0); // ���̷�Ʈ �̵���� �Լ�

	m_tInfo.matWorld = matScale * matTrans;

	D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vLook, &m_tInfo.matWorld);
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
	//m_pDeviceMgr->GetSprite()->End();
	D3DXVECTOR2 vPos;
	vPos.x = m_tInfo.vPos.x- CScrollMgr::GetScrollPos().x;
	vPos.y = m_tInfo.vPos.y- CScrollMgr::GetScrollPos().y;

	D3DXVECTOR2 vPoint[5] =
	{
		D3DXVECTOR2(vPos.x - m_vSize.x*0.5f, vPos.y - m_vSize.y*0.5f),
		D3DXVECTOR2(vPos.x - m_vSize.x*0.5f, vPos.y + m_vSize.y*0.5f),
		D3DXVECTOR2(vPos.x + m_vSize.x*0.5f, vPos.y + m_vSize.y*0.5f),
		D3DXVECTOR2(vPos.x + m_vSize.x*0.5f, vPos.y - m_vSize.y*0.5f),
		D3DXVECTOR2(vPos.x - m_vSize.x*0.5f, vPos.y - m_vSize.y*0.5f)

	};

	m_pDeviceMgr->GetLine()->SetWidth(5.f); // ���� ����
	m_pDeviceMgr->GetLine()->Draw(vPoint, 5, D3DCOLOR_ARGB(255, 255, 255, 0));
}

void CColliderBox::ButtonCollision()
{

}

void CColliderBox::HitBoxCollision()
{
}

bool CColliderBox::IsPicking(const D3DXVECTOR3 & vPos)
{
		
	D3DXVECTOR3 vPoint[4] =
	{
		D3DXVECTOR3(m_tInfo.vPos.x + m_vSize.x* 0.5f-CScrollMgr::GetScrollPos().x, m_tInfo.vPos.y - m_vSize.y* 0.5f-CScrollMgr::GetScrollPos().y, 0.f),
		D3DXVECTOR3(m_tInfo.vPos.x + m_vSize.x* 0.5f-CScrollMgr::GetScrollPos().x, m_tInfo.vPos.y + m_vSize.y* 0.5f-CScrollMgr::GetScrollPos().y, 0.f),
		D3DXVECTOR3(m_tInfo.vPos.x - m_vSize.x* 0.5f-CScrollMgr::GetScrollPos().x, m_tInfo.vPos.y + m_vSize.y* 0.5f-CScrollMgr::GetScrollPos().y, 0.f),
		D3DXVECTOR3(m_tInfo.vPos.x - m_vSize.x* 0.5f-CScrollMgr::GetScrollPos().x, m_tInfo.vPos.y - m_vSize.y* 0.5f-CScrollMgr::GetScrollPos().y, 0.f)
	};

	// �� ���������κ��� �ð�������� ���⺤�͵��� ����.
	D3DXVECTOR3 vDir[4] =
	{
		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3],
	};

	// ������ ���� ���⺤�͵��� �� �������͵��� ���Ѵ�.
	D3DXVECTOR3 vNormal[4] = {};

	for (int i = 0; i < 4; ++i)
		D3DXVec3Cross(&vNormal[i], &D3DXVECTOR3(0.f, 0.f, -1.f), &vDir[i]);

	// �� �������������� ���콺��ġ���� ���⺤�͸� ���Ѵ�.
	D3DXVECTOR3 vMouseDir[4] =
	{
		vPos - vPoint[0],
		vPos - vPoint[1],
		vPos - vPoint[2],
		vPos - vPoint[3]
	};

	// vMouseDir�� vNormal�� ���� �����ؼ� ��� �а�(����)�� ������ TRUE!
	for (int i = 0; i < 4; ++i)
	{
		if (0 < D3DXVec3Dot(&vNormal[i], &vMouseDir[i]))
			return false;
	}
	return true;



}
