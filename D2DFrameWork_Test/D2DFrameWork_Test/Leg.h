#pragma once
#include "GameObject.h"
class CLeg :
	public CGameObject
{
public:
	CLeg();
	CLeg(const OBJ_INFO & objInfo, D3DXMATRIX mat_World);
	CLeg(const OBJ_INFO & objInfo, D3DXVECTOR3 vPos);
	~CLeg();

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	void SetMatorld(D3DXMATRIX matWorld) { m_tInfo.matWorld = matWorld; }
	void SetvPos(D3DXVECTOR3 vPos, DWORD Dir = DOWN);

};

