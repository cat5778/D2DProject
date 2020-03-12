#pragma once
#include "GameObject.h"
class CColliderBox;
class CShadow;
class CLight;
class CChoose :
	public CGameObject
{
public:
	CChoose();
	CChoose(const OBJ_INFO& objInfo, D3DXVECTOR3 vPos);

	~CChoose();

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
public:
	bool GetIsChoose() { return m_bIsChose; }

private:
	//float ZOrder(float _y);
	D3DXVECTOR3 ConvertShadow();

private:
	CColliderBox*	m_CollBox;
	CLight*			m_Light;
	CShadow*		m_Shadow;
	bool			m_bIsChose;
};

