#pragma once
#include "Monster.h"


class CMevius :
	public CGameObject
{
public:
	CMevius();
	CMevius( D3DXVECTOR3 vPos);
	~CMevius();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
protected:
	void Appear();
	virtual void Idle();
	virtual void Attack();
	virtual void Move();
	virtual bool AttackRange();
	virtual void BeAttack(float fDamage);
	virtual int Dead();
	void MeviState(OBJECT_STATE eState);
	void StateMachine();
protected:
	CColliderBox*	m_CollBox;
	CHPBar*			m_pHPBar;
	D3DXVECTOR2		m_vSize;
	MONTER_TYPE		m_eMonType;
	float			m_fDetectDistance;
	float			m_fAtkRange;
	int				m_iHP;
	int				m_iCurHP;
	int				m_iExe;
	float			m_fSpeed;
	D3DXVECTOR3		m_vTargetPos;
	float			m_fApreDist;
	bool			m_bIsApreDist;
	D3DXVECTOR3		m_ApperPos;
	float			m_fAtkRate;
	float			m_fAngle[4];
	bool			m_bIsCast;
};