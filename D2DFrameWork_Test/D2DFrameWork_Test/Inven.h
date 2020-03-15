#pragma once
#include "GameObject.h"
class CHudButton;
class CInven :
	public CGameObject
{
public:
	CInven();
	~CInven();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
public:
	void On() { m_bIsRender = true; }
	void Off() { m_bIsRender = false; }
	bool GetIsOn() { return m_bIsRender; }
public:
	CHudButton*		m_ItemButton[WEAPONE_END];
	WEAPONE_TYPE	m_eWpType;
	bool			m_bIsRender;

};

