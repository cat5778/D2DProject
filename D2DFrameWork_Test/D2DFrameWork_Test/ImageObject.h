#pragma once
#include "GameObject.h"

class CColliderBox;
class CImageObject :
	public CGameObject
{
public:
	CImageObject();
	CImageObject(wstring wstrObjectKey, wstring wstrStateKey, D3DXVECTOR3 vPos, bool IsAni = 1.0f, int ImageIdx = 0,
		float fXScale =1.f,float fYScale=1.f,float fzOrder=1.0f,int iAlpha=255);
	~CImageObject();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	void SetCollider(D3DXVECTOR2 _vSIze);

private:
	float			m_fzOrder;
	float			m_fXScale;
	float			m_fYScale;
	int				m_iAlpha;
	CColliderBox*	m_CollBox;
};

