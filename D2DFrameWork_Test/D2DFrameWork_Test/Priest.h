#pragma once
#include "Monster.h"
class Priest :
	public CMonster
{
public:
	Priest();
	Priest(const OBJ_INFO& objInfo, D3DXVECTOR3 vPos);
	~Priest();
private:
	virtual void Attack() override;
	virtual int Dead()override;
};

