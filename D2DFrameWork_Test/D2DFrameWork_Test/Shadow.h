#pragma once
#include "GameObject.h"
class CShadow :
	public CGameObject
{
public:
	CShadow();
	CShadow(D3DXVECTOR3 vPos);
	~CShadow();

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
};

