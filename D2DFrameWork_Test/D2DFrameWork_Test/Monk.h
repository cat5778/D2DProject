#pragma once
#include "Monster.h"
class Monk :
	public CMonster
{
public:
	Monk();
	Monk(const OBJ_INFO& objInfo, D3DXVECTOR3 vPos);
	~Monk();

private:
	virtual void Attack() override;
	virtual int Dead()override;
};

