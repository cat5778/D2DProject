#pragma once
#include "GameObject.h"
class CTextBox :
	public CGameObject
{
public:
	CTextBox();
	CTextBox(float fx,float fy, float fzOrder=1,  float fsize=1);
	~CTextBox();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	void SetButtonText(wstring wsText) { m_wsText = wsText; }
private:
	wstring			m_wsText;
	float			m_fXRatio;
	float			m_fYRatio;
	float			m_fZOrder;
	float			m_fSize;
};

