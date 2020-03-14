#include "stdafx.h"
#include "Monster.h"
#include "GameObject.h"
#include "ColliderBox.h"
#include "ScrollMgr.h"
#include "HPBar.h"
#include "Effect.h"
#include "SkillProjectTile.h"
CMonster::CMonster()
	:m_CollBox(nullptr)
{
	
}

CMonster::CMonster(const OBJ_INFO & objInfo, D3DXVECTOR3 vPos, MONTER_TYPE eType)
	: m_CollBox(nullptr), m_eMonType(eType)
{
	
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
	m_bIsCollsion = false;
	m_bIsObjCollision = false;
	//m_wsIdleState=m_tObjInfo.wstrStateKey;
	m_bIsAttack = false;
	m_eType = OBJECT_MONSTER;
	D3DXMatrixIdentity(&m_tInfo.matWorld); // 다이렉트 항등행렬 함수
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_fTimer = 0.f;
	m_bIsDead = false;
	m_bIsInvincible = false;
	m_fKnockTime = 0;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 10.f;
	m_pHPBar = new CHPBar(OBJECT_PLAYER, m_tInfo.vPos, m_tData);
	ChangeState(STATE_IDLE);
	m_wsState = L"";
	return S_OK;
}

void CMonster::Release()
{
	g_PlayerExe += m_tData.fCurEXE;
	m_CollBox->EraseCollider();
}

int CMonster::Update()
{
	
	if (m_eCurState == STATE_DEAD)
	{
		m_CollBox->SetActice(false);
		m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDelta();

		if (m_tFrame.fMaxFrame <= m_tFrame.fCurFrame)
			return DEAD_OBJ;
	}
	else
		Animate();
	
	StateMachine();
	DetectTarget();
	
	if (m_CollBox != nullptr)
	{
		m_CollBox->SetDamage(m_tData.fDamage);
		m_CollBox->SetvPos(m_tInfo.vPos);

		if (m_CollBox->IsCollsion() 
			&& m_CollBox->GetHitColl()== PLAYER_PROJECTILE_COLLISION)
		{
			BeAttack(-m_CollBox->GetGameData().fCurHp);
			m_CollBox->InitCurHP();
			m_CollBox->SetHitColl(COLLSION_END);
			CEffect* temp = new CEffect(m_tInfo.vPos, L"Effect", L"Hit");
			CObjectMgr::GetInstance()->AddObject(OBJECT_EFFECT, temp);
			m_bIsInvincible = true;


		}

		if (m_bIsInvincible)
			KnockBack(m_CollBox->GetvKnock(), 200.f);

		Timer(m_bIsInvincible, 0.1f, m_fKnockTime);
	}


	return NO_EVENT;
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
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_tObjInfo.wstrObjectKey,
		m_tObjInfo.wstrStateKey, (int)m_tFrame.fCurFrame);
	m_tFrame.fMaxFrame = pTexInfo->iMaxCnt;

	NULL_CHECK(pTexInfo);
	
	m_vSize.x= pTexInfo->tImgInfo.Width * 0.5f;
	m_vSize.y= pTexInfo->tImgInfo.Height * 0.5f;
	
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

void CMonster::Idle()
{
	if (m_eMonType == MONK|| m_eMonType == PRIEST)
	{
		GetCollDir(m_vTargetPos);
		GetImageDir(m_dwCollDir);
		m_tObjInfo.wstrStateKey = m_wsIdleState + m_wsImgDir;
	}
	else
		m_tObjInfo.wstrStateKey = m_wsIdleState;
	
}

void CMonster::Attack()
{
	cout << "MonAttack" << endl;
}

void CMonster::Move()
{
	if (m_eMonType == MONK||m_eMonType == PRIEST)
	{
		GetCollDir(m_vTargetPos);
		GetImageDir(m_dwCollDir);
		m_tObjInfo.wstrStateKey = m_wsIdleState + m_wsImgDir;
	}
	else
		m_tObjInfo.wstrStateKey = m_wsIdleState;
	if (m_CollBox->IsOBJCollsion())
	{
		//wcout << m_tObjInfo.wstrStateKey.c_str()  << endl;
		return;
	}



	D3DXVECTOR3 temPos;
	D3DXVec3Normalize(&temPos,&m_vDir);
	m_tInfo.vPos += temPos*m_fSpeed* m_pTimeMgr->GetDelta();

}

bool CMonster::DetectTarget()
{
	if (m_tData.fCurHp <= 0.f)
	{
		ChangeState(STATE_DEAD);
		return false;
	}
	if (!CObjectMgr::GetInstance()->GetObjList(OBJECT_PLAYER).empty())
	{
		if (!m_bIsAttack)
		{
			m_wsState = L"";
			m_vTargetPos = (*CObjectMgr::GetInstance()->GetObjList(OBJECT_PLAYER).begin())->GetTagInfo().vPos;
			m_vDir = m_vTargetPos - m_tInfo.vPos;
			if (D3DXVec3Length(&m_vDir) <= m_fDetectDistance)
			{
				if (D3DXVec3Length(&m_vDir) <= m_fAtkRange)
					ChangeState(STATE_ATTACK);
				else
				{
					ChangeState(STATE_MOVE);
					//m_tFrame.fCurFrame = 0;
				}
				return true;
			}
			else
				ChangeState(STATE_IDLE);
		}
	
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

int CMonster::Dead()
{
	cout << "MonDead" << endl;
	return NO_EVENT;
}

void CMonster::StateMachine()
{
	switch (m_eCurState)
	{
	case STATE_IDLE:
		Idle();
		break;
	case STATE_ATTACK:
		Attack();
		break;
	case STATE_MOVE:
		Move();
		break;
	case STATE_BEATTACK:
		break;
	case STATE_DEAD:
		Dead();
		break;
	case STATE_END:
		break;
	default:
		break;
	}
}

