#pragma once
#include "GameObject.h"
class CLight :
	public CGameObject
{
public:
	CLight();
	CLight(LIGHT_TYPE eType, D3DXVECTOR3 vPos);
	~CLight();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	void GetTexture();
private:
	LIGHT_TYPE m_eLightType;
};

