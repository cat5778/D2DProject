#pragma once
#include "GameObject.h"

class CGaugeFill;
class CHPBar :
	public CGameObject
{
public:
	CHPBar();
	CHPBar(OBJECT_TYPE eType, D3DXVECTOR3 vPos, GAME_DATA data);
	~CHPBar();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
public:
	
	void SetData(D3DXVECTOR3 vPos, GAME_DATA data,float fY = 0);
	void UpdateRect();
	void CreateFill();
private:
	CGaugeFill* m_CGugeFill[FILL_END];
	wstring		m_wstrType;
	float		m_fHPRatio;
	RECT		m_tRect;
};

