#pragma once
#include "GameObject.h"
class CColliderBox :
	public CGameObject
{
public:
	CColliderBox();
	CColliderBox(D3DXVECTOR3 &vPos, COLLSION_TYPE eType, D3DXVECTOR2 vSize, D3DXVECTOR3 vDir = {0,0,0}, bool infinite = true);
	~CColliderBox();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	void RenderLine();
public:
	bool GetActice() { return m_bIsActive; }
	void SetActice(bool active) { m_bIsActive=active; }
	COLLSION_TYPE GetCollType() {return m_eCollType;}
	bool IsPicking(const D3DXVECTOR3 & vPos);
	void EraseCollider() { m_bIsDeadObj = true; }
	bool IsDead() { return m_bIsDeadObj; }
private:
	void ButtonCollision();
	void HitBoxCollision();
private:
	COLLSION_TYPE	m_eCollType;
	bool			m_bIsActive;
	bool			m_bIsInfinite;
	bool			m_bIsDeadObj;
	

};

