#pragma once
#include "GameObject.h"
class CColliderBox;

class CGameButton :
	public CGameObject
{
public:
	CGameButton();
	CGameButton(const OBJ_INFO& objInfo, D3DXVECTOR3 vPos);
	~CGameButton();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	bool GetIsPick() { return m_IsPick; }
	void SetButtonText(wstring wsText) {m_wsText = wsText;}
private:
	CColliderBox*	m_CollBox;
	D3DXVECTOR2		m_vSize;
	bool			m_IsPick;
	wstring			m_wsText;
};

