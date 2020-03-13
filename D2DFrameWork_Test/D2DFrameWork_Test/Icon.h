#pragma once
#include "GameObject.h"
class CIcon :
	public CGameObject
{
public:
	CIcon();
	CIcon(wstring wstrObjKey, wstring wstrStateKey,int iImgKey, float fxRatio, float fyRatio);
	~CIcon();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	


private:
	float m_fXRatio;
	float m_fYRatio;
};

