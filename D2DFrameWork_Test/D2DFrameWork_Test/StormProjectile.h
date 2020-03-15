#pragma once
#include "GameObject.h"
class CColliderBox;
class CStormProjectile :
	public CGameObject
{
public:
	CStormProjectile();
	CStormProjectile(D3DXVECTOR3 vPos, float fDamage = -1.f,float fTime=1.0f,
		COLLSION_TYPE eCollType = PLAYER_PROJECTILE_COLLISION,
		wstring wstrObjectKey = L"Projectile", wstring wstrStateKey = L"", bool IsAni = true);
	~CStormProjectile();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	WEAPONE_TYPE	m_eWpType;
	CColliderBox*	m_CollBox;
	D3DXVECTOR2		m_vSize;
	float			m_fSpeed;
	float			m_fTime;
	bool			m_IsDead;
	float			m_fDeleteTime;

};

