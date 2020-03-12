#pragma once
#include "GameObject.h"
class CGaugeFill :
	public CGameObject
{
public:
	CGaugeFill();
	CGaugeFill(FILL_TYPE eType,D3DXVECTOR3 vPos,RECT rt);
	~CGaugeFill();


	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	
	void UpdateData(D3DXVECTOR3 vPos, RECT rect);
private:
	FILL_TYPE m_eFill;
	RECT	  m_tRect;
};

