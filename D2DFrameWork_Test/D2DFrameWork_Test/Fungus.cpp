#include "stdafx.h"
#include "Fungus.h"
#include "Projectile.h"
#include "ColliderBox.h"
CFungus::CFungus()
{
}

CFungus::CFungus(const OBJ_INFO & objInfo, D3DXVECTOR3 vPos)
{
	m_eMonType = FUNGUS;
	m_CollBox=nullptr;
	m_fDetectDistance = 400;
	m_fAtkRange = 200;
	m_eType = OBJECT_MONSTER;
	m_tObjInfo = objInfo;
	m_tInfo.vPos = vPos;
	m_fSpeed = 100.f;
	m_tData.fCurEXE = 10;
	m_tData.fHp = 100;
	m_tData.fCurHp = m_tData.fHp;
	m_tData.fDamage = 30;
	m_tData.fOldHp = m_tData.fCurHp;
	m_wsIdleState = objInfo.wstrStateKey;
	Initialize();
}


CFungus::~CFungus()
{
}

void CFungus::Attack()
{

	m_fTimer += m_pTimeMgr->GetDelta();
	if (m_fTimer >= 2.0f)
	{
		CProjectile* temp = new CProjectile(WEAPONE_END, m_tInfo.vPos, m_vDir,
			m_tData.fDamage,MONSTER_PROJECTILE_COLLISION,L"Effect",L"Black_Hole");
		CObjectMgr::GetInstance()->AddObject(OBJECT_PROJECTILE, temp);
		m_fTimer = 0.f;
		m_pSoundMgr->PlaySound(L"Chaos_Bolt.wav", EFFECT);

	}


}

int CFungus::Dead()
{

	if (!m_bIsDead)
	{
		m_pSoundMgr->PlaySound(L"Enemy_Huge_Explosion.wav", EFFECT);
		wstring temp = L"_Dies";
		m_tObjInfo.wstrStateKey += temp;
	

		m_bIsDead = true;
	}//cout << "Fungus" << endl;
	return NO_EVENT;
}

