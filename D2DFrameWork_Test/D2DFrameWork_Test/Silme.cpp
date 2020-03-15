#include "stdafx.h"
#include "Silme.h"
#include "Projectile.h"
#include "ColliderBox.h"

CSilme::CSilme()
{
}

CSilme::CSilme(const OBJ_INFO & objInfo, D3DXVECTOR3 vPos)
{
	m_eMonType = SLIME;
	m_CollBox = nullptr;
	m_fDetectDistance = 250;
	m_fAtkRange = 30.f;
	m_eType = OBJECT_MONSTER;
	m_tObjInfo = objInfo;
	m_tInfo.vPos = vPos;
	m_fSpeed = 100.f;
	m_tData.fCurEXE = 10;
	m_tData.fHp = 100;
	m_tData.fCurHp = m_tData.fHp;
	m_tData.fDamage = 10;
	m_tData.fOldHp = m_tData.fCurHp;
	m_wsIdleState = objInfo.wstrStateKey;

	Initialize();
}


CSilme::~CSilme()
{
	
}

void CSilme::Attack()
{
	if (!m_bIsAttack)
	{
		//m_dwDir = RIGHT;
		//GetImageDir
		GetCollDir(m_vTargetPos);
		m_wsState = L"_Attack";
		GetImageDir(m_dwCollDir);
		wstring temp = m_wsIdleState+m_wsState + m_wsImgDir;
		m_tObjInfo.wstrStateKey = temp;
		m_bIsAttack = true;
		m_tFrame.fCurFrame = 0;
		m_fTimer = 0.f;
		m_pSoundMgr->PlaySound(L"Slime_Attack.wav", EFFECT);


	}

	m_fTimer += m_pTimeMgr->GetDelta();
	if (m_fTimer >= 1.0f)
	{
		m_fTimer = 0.f;
		CProjectile* pHitBox = new CProjectile(WEAPONE_END, m_tInfo.vPos, m_vDir,
			m_tData.fDamage, MONSTER_PROJECTILE_COLLISION, L"", L"", false);
		CObjectMgr::GetInstance()->AddObject(OBJECT_PROJECTILE, pHitBox);
		m_bIsAttack = false;
		m_tFrame.fCurFrame = 0;

		ChangeState(STATE_IDLE);

	}

}

int CSilme::Dead()
{

	if (!m_bIsDead)
	{
		m_pSoundMgr->PlaySound(L"Slime_Die.wav", EFFECT);
		m_bIsDead = true;
	}
	return NO_EVENT;
}
