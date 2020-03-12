#pragma once
#include "GameObject.h"
class CColliderBox;
class CProjectile :
	public CGameObject
{
public:
	CProjectile();
	CProjectile(WEAPONE_TYPE eType,D3DXVECTOR3 vPos, D3DXVECTOR3 vDir,float fDamage=-1.f, 
		COLLSION_TYPE eCollType= PLAYER_PROJECTILE_COLLISION,wstring wstrObjectKey= L"Projectile", wstring wstrStateKey=L"");
	~CProjectile();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	void PlayerAngle();
	void MonsterAngle();

private:
	WEAPONE_TYPE	m_eWpType;

	CColliderBox*	m_CollBox;
	D3DXVECTOR2		m_vSize;
	float			m_fSpeed;
	float			m_fTime;
	bool			m_IsDead;
	


	
};

