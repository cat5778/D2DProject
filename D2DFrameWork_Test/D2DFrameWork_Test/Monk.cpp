#include "stdafx.h"
#include "Monk.h"
#include "Projectile.h"
#include "ColliderBox.h"


Monk::Monk()
{
}

Monk::Monk(const OBJ_INFO & objInfo, D3DXVECTOR3 vPos)
{
	m_tObjInfo = objInfo;
	m_wsIdleState = objInfo.wstrObjectName;
	m_eMonType = MONK;
	m_CollBox = nullptr;
	m_fDetectDistance = 250;
	m_fAtkRange = 60.f;
	m_eType = OBJECT_MONSTER;
	m_tInfo.vPos = vPos;
	m_fSpeed = 170.f;
	m_tData.fCurEXE = 60;
	m_tData.fHp = 200;
	m_tData.fCurHp = m_tData.fHp;
	m_tData.fDamage = 35.f;
	m_tData.fOldHp = m_tData.fCurHp;

	m_eCurState = STATE_IDLE;
	m_eOldState = STATE_IDLE;
	
	Initialize();
}


Monk::~Monk()
{
}

void Monk::Attack()
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
		CProjectile* pHitBox = new CProjectile(WEAPONE_END, m_tInfo.vPos, m_vDir,
			m_tData.fDamage, MONSTER_PROJECTILE_COLLISION, L"", L"", false);
		CObjectMgr::GetInstance()->AddObject(OBJECT_PROJECTILE, pHitBox);
		m_bIsAttack = false;
		m_pSoundMgr->PlaySound(L"Zombie_Attack.wav", EFFECT);

		m_tFrame.fCurFrame = 0;
		ChangeState(STATE_IDLE);

	}
}

int Monk::Dead()
{
	if (!m_bIsDead)
	{
		m_pSoundMgr->PlaySound(L"Monk_Die.wav", EFFECT);

		GetCollDir(m_vTargetPos);
		m_wsState = L"_Dies";
		GetImageDir(m_dwCollDir);
		wstring temp = m_wsIdleState + m_wsState + m_wsImgDir;
		m_tObjInfo.wstrStateKey = temp;
		m_bIsDead = true;
	}
	return NO_EVENT;
}
