#include "stdafx.h"
#include "Monster.h"
#include "GameObject.h"
#include "ColliderBox.h"
#include "ScrollMgr.h"
#include "HPBar.h"
CMonster::CMonster()
	:m_eType(OBJECT_MONSTER), m_CollBox(nullptr)
{
}

CMonster::CMonster(const OBJ_INFO & objInfo, D3DXVECTOR3 vPos)
	:m_eType(OBJECT_MONSTER), m_CollBox(nullptr)
{
	m_bIsCollsion = false;
	m_tObjInfo = objInfo;
	m_tInfo.vPos = vPos;
	Initialize();
}


CMonster::~CMonster()
{
	Release();
}

HRESULT CMonster::Initialize()
{
	D3DXMatrixIdentity(&m_tInfo.matWorld); // 다이렉트 항등행렬 함수
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 0.f, -1.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 10.f;
	m_pHPBar = new CHPBar(OBJECT_PLAYER, m_tInfo.vPos, m_tData);
	return S_OK;
}

void CMonster::Release()
{
}

int CMonster::Update()
{

	if (m_CollBox != nullptr)
	{
		m_CollBox->SetDamage(m_tData.fDamage);
		m_CollBox->SetvPos(m_tInfo.vPos);
		if (m_CollBox->IsCollsion() && m_CollBox->GetHitColl()== PLAYER_PROJECTILE_COLLISION)
		{
			cout<<"충돌"<<endl;
			m_tData.fCurHp -= 10;
			m_CollBox->SetHitColl(COLLSION_END);
		}
	}
	if (m_tObjInfo.IsAni)
	{
		m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDelta()/* * 20.f*/;
		if (m_tFrame.fMaxFrame <= m_tFrame.fCurFrame)
			m_tFrame.fCurFrame = 0.f;
	}
	
	if (DetectTarget())
	{
		if (AttackRange())
		{
			Attack();
		}
		else
		{
			if (!m_CollBox->IsCollsion())
				Move();
		}
	}
	return 0;
}

void CMonster::LateUpdate()
{
	D3DXMATRIX matScale, matTrans;

	D3DXMatrixScaling(&matScale,
		m_tInfo.vSize.x,
		m_tInfo.vSize.y,
		0.f); // 다이렉트 크기행렬 함수
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x,
		m_tInfo.vPos.y - CScrollMgr::GetScrollPos().y,
		ZOrder(m_tInfo.vPos.y)); // 다이렉트 이동행렬 함수

	m_tInfo.matWorld = matScale * matTrans;

	D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vLook, &m_tInfo.matWorld);
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
}

void CMonster::Render()
{
	const TEX_INFO* pTexInfo;
	if (m_tObjInfo.IsAni)
	{
		pTexInfo = m_pTextureMgr->GetTexInfo(m_tObjInfo.wstrObjectKey, m_tObjInfo.wstrStateKey, (int)m_tFrame.fCurFrame);
		m_tFrame.fMaxFrame = pTexInfo->iMaxCnt;
	}
	else
		pTexInfo = m_pTextureMgr->GetTexInfo(m_tObjInfo.wstrObjectKey, m_tObjInfo.wstrStateKey, m_tObjInfo.ImageIDX);

	NULL_CHECK(pTexInfo);
	//float fCenterX = 0;
	//float fCenterY = 0;
	m_vSize.x= pTexInfo->tImgInfo.Width * 0.5f;
	m_vSize.y= pTexInfo->tImgInfo.Height * 0.5f;
	//ConvertCenter(m_tObjInfo, *pTexInfo, fCenterX, fCenterY);

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(m_vSize.x, m_vSize.y, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	if (m_CollBox == nullptr)
	{
		m_CollBox = new CColliderBox(m_tInfo.vPos, MONSTER_HITBOX_COLLISION, m_vSize);
		m_pColliderMgr->AddObject(MONSTER_HITBOX_COLLISION, m_CollBox);
	}
	if (m_pHPBar != nullptr)
	{
		m_pHPBar->SetData(m_tInfo.vPos, m_tData, 25);
		m_pHPBar->Render();
	}
	//if (CKeyManager::GetInstance()->KeyPressing(KEY_6) && m_CollBox != nullptr)
	//{
	//	m_CollBox->RenderLine();
	//}
}

void CMonster::Attack()
{
}

void CMonster::Move()
{
	D3DXVECTOR3 temPos;
	D3DXVec3Normalize(&temPos,&m_vDir);
	m_tInfo.vPos += temPos*m_fSpeed* m_pTimeMgr->GetDelta();

}

bool CMonster::DetectTarget()
{
	if (!CObjectMgr::GetInstance()->GetObjList(OBJECT_PLAYER).empty())
	{

		auto playerPos = (*CObjectMgr::GetInstance()->GetObjList(OBJECT_PLAYER).begin())->GetTagInfo().vPos;
		m_vDir = playerPos - m_tInfo.vPos;
		if (D3DXVec3Length(&m_vDir) <= m_fDetectDistance)
			return true;
	}
	return false;
}

bool CMonster::AttackRange()
{
	if (!CObjectMgr::GetInstance()->GetObjList(OBJECT_PLAYER).empty())
	{
		auto playerPos = (*CObjectMgr::GetInstance()->GetObjList(OBJECT_PLAYER).begin())->GetTagInfo().vPos;
		m_vDir = playerPos - m_tInfo.vPos;
		if (D3DXVec3Length(&m_vDir) <= m_fAtkRange)
			return true;
	}
	return false;
}

