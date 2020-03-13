#pragma once
#include "Monster.h"
class CSilme :
	public CMonster
{
public:
	CSilme();
	CSilme(const OBJ_INFO& objInfo, D3DXVECTOR3 vPos);
	~CSilme();


private:
	virtual void Attack() override;
	virtual int Dead()override;
};

