#pragma once
#include "Monster.h"
class CFungus :
	public CMonster
{
public:
	CFungus();
	CFungus(const OBJ_INFO& objInfo, D3DXVECTOR3 vPos);
	~CFungus();

private:
	virtual void Attack() override;
	virtual int Dead()override;
};

