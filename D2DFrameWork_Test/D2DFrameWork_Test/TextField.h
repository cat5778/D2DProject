#pragma once
#include "GameObject.h"
class CColliderBox;

class CTextField :
	public CGameObject
{
public:
	CTextField();
	CTextField(D3DXVECTOR3 vPos);
	~CTextField();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

	CColliderBox*	m_CollBox;
	D3DXVECTOR2		m_vSize;
	bool			m_IsPick;

	

};

