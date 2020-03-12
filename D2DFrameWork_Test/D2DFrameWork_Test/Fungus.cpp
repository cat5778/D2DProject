#include "stdafx.h"
#include "Fungus.h"


CFungus::CFungus()
{
}

CFungus::CFungus(const OBJ_INFO & objInfo, D3DXVECTOR3 vPos)
{
	m_CollBox=nullptr;
	m_fDetectDistance = 200;
	m_eType = OBJECT_MONSTER;
	m_tObjInfo = objInfo;
	m_tInfo.vPos = vPos;
	m_fSpeed = 100.f;
	Initialize();

	m_tData.fCurEXE = 10;
	m_tData.fHp = 100;
	m_tData.fCurHp = m_tData.fHp;
	m_tData.fDamage = 5;
	m_tData.fOldHp = m_tData.fCurHp;
}


CFungus::~CFungus()
{
}

void CFungus::Attack()
{
	//cout << "FungusAtk" << endl;

}

