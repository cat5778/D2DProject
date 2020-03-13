#pragma once
#include "GameObject.h"
class CProjectile;
class CWeapon :
	public CGameObject
{
public:
	CWeapon();
	CWeapon(D3DXVECTOR3 vPos, float fAtkSpeed,float fDamage=-1.0f);
	~CWeapon();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;


public:
	WEAPONE_TYPE GetWeaponType() { return m_eWpType; }
	void SetWeaponType(WEAPONE_TYPE eType) { m_eWpType = eType; }
	void AttackAni(WEAPONE_TYPE eType);
	void SetAtkRate(float fAtkRate) { m_fAtkRate = fAtkRate; }
private:
	void ConvertPos();

private:
	WEAPONE_TYPE		m_eWpType;
	wstring				m_wstrState;
	list<CProjectile*>  m_pPTlist;
	
	float				m_fAtkRate;
	bool				m_bIsAtkEnd;
	D3DXVECTOR3			m_tAtkVec;
};

