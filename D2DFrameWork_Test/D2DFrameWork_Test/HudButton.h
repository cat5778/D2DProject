#pragma once
#include "GameObject.h"
class CHudButton :
	public CGameObject
{
public:
	CHudButton();
	CHudButton(wstring wstrObjKey, wstring wstrStateKey, int iImgKey, float fxRatio, float fyRatio, float zOrder= 0.09f,D3DXVECTOR2 ButtonSize = { 38,38 });
	~CHudButton();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;


public:
	void SetButtonText(wstring wsText) { m_wsText = wsText; }
	bool GetIsPick() { return m_IsPick; }
	void SetTextPos(D3DXVECTOR2 vPos);
	bool RadioButton();
	
	
private:
	bool			m_bIsRadioButton;
	CColliderBox*	m_CollBox;
	bool			m_IsPick;
	wstring			m_wsText;
	float			m_fXRatio;
	float			m_fYRatio;
	float			m_fZOrder;
	D3DXVECTOR3		m_vTextPos;
};