#pragma once
#include "GameObject.h"
class CColliderBox;
class CHPBar;
class CMonster :
	public CGameObject
{
public:
	CMonster();
	CMonster(const OBJ_INFO& objInfo, D3DXVECTOR3 vPos);
	~CMonster();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
protected:
	virtual void Attack();
	virtual void Move();
	virtual bool DetectTarget();
	virtual bool AttackRange();

protected:
	CColliderBox*	m_CollBox;
	CHPBar*			m_pHPBar;
	D3DXVECTOR2		m_vSize;
	D3DXVECTOR3		m_vDir;
	OBJECT_TYPE		m_eType;
	float			m_fDetectDistance;
	float			m_fAtkRange;
	int				m_iHP;
	int				m_iCurHP;
	int				m_iExe;
	float			m_fSpeed;
};

