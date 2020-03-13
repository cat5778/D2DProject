#include "stdafx.h"
#include "Priest.h"
#include "Projectile.h"
#include "ColliderBox.h"

Priest::Priest()
{
}

Priest::Priest(const OBJ_INFO & objInfo, D3DXVECTOR3 vPos)
{
	m_tObjInfo = objInfo;
	m_wsIdleState = objInfo.wstrObjectName;
	m_eMonType = PRIEST;
	m_CollBox = nullptr;
	m_fDetectDistance = 400;
	m_fAtkRange = 300.f;
	m_eType = OBJECT_MONSTER;
	m_tInfo.vPos = vPos;
	m_fSpeed = 150.f;
	m_tData.fCurEXE = 30;
	m_tData.fHp = 150;
	m_tData.fCurHp = m_tData.fHp;
	m_tData.fDamage = 35.f;
	m_tData.fOldHp = m_tData.fCurHp;

	m_eCurState = STATE_IDLE;
	m_eOldState = STATE_IDLE;

	Initialize();
}


Priest::~Priest()
{
}

void Priest::Attack()
{
	if (!m_bIsAttack)
	{
		GetCollDir(m_vTargetPos);
		m_wsState = L"_Attack";
		GetImageDir(m_dwCollDir);
		wstring temp = m_wsIdleState + m_wsState + m_wsImgDir;
		m_tObjInfo.wstrStateKey = temp;
		m_bIsAttack = true;
		m_tFrame.fCurFrame = 0;
		m_fTimer = 0.f;
	}
	m_fTimer += m_pTimeMgr->GetDelta();
	if (m_fTimer >= 1.0f)
	{
		m_fTimer = 0.f;
		CProjectile* temp = new CProjectile(WEAPONE_END, m_tInfo.vPos, m_vDir,
			m_tData.fDamage, MONSTER_PROJECTILE_COLLISION, L"Effect", L"Quest_Black_Hole");
		CObjectMgr::GetInstance()->AddObject(OBJECT_PROJECTILE, temp);
		m_bIsAttack = false;
		ChangeState(STATE_IDLE);

	}
}

int Priest::Dead()
{
	if (!m_bIsDead)
	{
		GetCollDir(m_vTargetPos);
		m_wsState = L"_Dies";
		GetImageDir(m_dwCollDir);
		wstring temp = m_wsIdleState + m_wsState + m_wsImgDir;
		m_tObjInfo.wstrStateKey = temp;
		m_bIsDead = true;
	}
	return NO_EVENT;
}
