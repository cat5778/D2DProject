#pragma once
#include "GameObject.h"
class CIcon;
class CHudButton;
class CSkillFrame;
class CHud :
	public CGameObject
{
public:
	CHud();
	~CHud();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

	void InitData(int iLevel, float fSpd, float fDmg, float fAtkSpd);
private:
	CIcon*	m_HudIcon[4];
	CHudButton* m_HudButton;
	CSkillFrame* m_SkillFrame;

	int			m_iLevel;
	float		m_fSpeed;
	float		m_fDamage;
	float		m_fAtkSpeed;
};

