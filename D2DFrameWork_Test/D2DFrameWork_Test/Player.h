#pragma once

#include "GameObject.h"
class CHat;
class CChest;
class CLeg;
class CWeapon;
class CColliderBox;
class CHPBar;
class CHud;
class CPlayer : public CGameObject
{
public:
	CPlayer();
	CPlayer(D3DXVECTOR3 vPos);

public:
	~CPlayer();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

private:
	void KeyInput();
	void FollowPath();
private:
	float ZOrder(float _y);
	void ConvertPos(const OBJ_INFO& objInfo, const TEX_INFO& pTexInfo, D3DXVECTOR3 &vPos);
	void ConvertCenter(const OBJ_INFO& objInfo, const TEX_INFO& pTexInfo, float &cx, float &cy);

public:
	static CPlayer* Create();

private:
	CKeyManager*			m_pKeyMgr;
	CHat *					m_pHat;
	CChest*					m_pChest;
	CLeg*					m_pLeg;
	CWeapon*				m_pWeapon;
	CColliderBox*			m_CollBox;
	CHPBar*					m_pHPBar;

	
	wstring					m_wstrChest;
	WEAPONE_TYPE			m_eWpType;
	float					m_fSpeed;
	float					m_fAtkSpd;
	bool					m_bIsAttack;
	CHud*					m_pHud;
	//CAstarMgr*		m_pAstarMgr;
};

