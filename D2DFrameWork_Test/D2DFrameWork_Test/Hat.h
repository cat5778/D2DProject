#pragma once
#include "GameObject.h"
class CHat :
	public CGameObject
{
public:
	CHat();
	CHat(const OBJ_INFO & objInfo, D3DXMATRIX mat_World);
	CHat(const OBJ_INFO & objInfo, D3DXVECTOR3 vPos);
	~CHat();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
public:
	void SetMatorld(D3DXMATRIX matWorld) { m_tInfo.matWorld = matWorld; }
	void SetvPos(D3DXVECTOR3 vPos, DWORD Dir=DOWN);

};

