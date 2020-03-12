#pragma once
#include "Scene.h"
class CTitle;
class CGameButton;
class CLogo : public CScene
{
private:
	CLogo();

public:
	virtual ~CLogo();


public:
	// CScene을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CLogo* Create();
private:
	CObjectMgr*			m_pObjectMgr;
	CKeyManager*			m_pKeyMgr;
	CGameButton*		    m_Button;
};

