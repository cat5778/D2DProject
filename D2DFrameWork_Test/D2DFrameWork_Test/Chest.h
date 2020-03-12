#pragma once
#include "GameObject.h"
class CChest :
	public CGameObject
{
public:
	CChest();
	CChest(const OBJ_INFO & objInfo, D3DXMATRIX mat_World);
	CChest(const OBJ_INFO & objInfo, D3DXVECTOR3 vPos, float fAtkSpeed);
	~CChest();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
public:
	void SetMatorld(D3DXMATRIX matWorld) { m_tInfo.matWorld = matWorld; }
	void AttackAni(WEAPONE_TYPE eType);

private:
	wstring				m_wstrWeapon;
	bool				m_bIsAtkEnd;
	WEAPONE_TYPE		m_eWpType;
	float				m_fAtkSpeed;
};

