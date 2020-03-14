#pragma once
#include "GameObject.h"
class CEffect :
	public CGameObject
{
public:
	CEffect();
	CEffect(D3DXVECTOR3 vPos, wstring wstrObjKey, wstring wstrStateKey, float Zorder = 1.f);
	~CEffect();

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	float	m_fZOrder;
};

