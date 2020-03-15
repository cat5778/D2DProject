#pragma once
#include "GameObject.h"

class CMeviusProjectile :
	public CGameObject
{
public:
	CMeviusProjectile();

	CMeviusProjectile(D3DXVECTOR3 vPos, float fRadian, float fDamage = -1.f);
	~CMeviusProjectile();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	CColliderBox*	m_CollBox;
	D3DXVECTOR2		m_vSize;
	float			m_fSpeed;
	float			m_fTime;
	bool			m_IsDead;
};

