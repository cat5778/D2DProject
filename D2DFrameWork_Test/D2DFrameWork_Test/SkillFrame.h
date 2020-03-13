#pragma once
#include "GameObject.h"
class CHudButton;
class CSkillLine;
class CTextBox;
class CSkillFrame :
	public CGameObject
{
public:
	CSkillFrame();
	~CSkillFrame();

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
	void InitData(int iLevel,float fSpd, float fDmg, float fAtkSpd);
	void LevelUp();
	//void DrawStat(D3DXMATRIX mat_world,wstring wstrStat, float fx, float fy);
private:
	CHudButton* m_SkillButton[9];
	CSkillLine* m_SkillLine;
	bool		m_bIsRender;
	int			m_iSkillPoint[9];

	wstring		m_wsText;
	int			m_iPoint;

	int			m_iLevel;
	float		m_fSpeed;
	float		m_fDamage;
	float		m_fAtkSpeed;

	wstring		m_wsLevel;
	wstring		m_wsDamage;
	wstring		m_wsAtkSpeed;
	wstring		m_wsSpeed;

private:
	CTextBox*	m_TextBox[4];

};

