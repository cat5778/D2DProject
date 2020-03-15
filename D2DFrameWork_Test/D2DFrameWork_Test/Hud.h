#pragma once
#include "GameObject.h"
class CIcon;
class CHudButton;
class CSkillFrame;
class CInven;
class CHud :
	public CGameObject
{
public:
	CHud();
	CHud(GAME_DATA gameData);
	~CHud();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	
	GAME_DATA GetData();
	SKILL_DATA GetSkillData();
	void SetOnSkillTree();
	void SetOnInven();
	void SetData(GAME_DATA gameData);
	void InitData(GAME_DATA gameData);
	bool IsOff() { return m_bIsEnd; }
	

private:
	CIcon*	m_HudIcon[4];
	CHudButton* m_HudButton;
	CSkillFrame* m_SkillFrame;
	CInven*		 m_Inven;
	GAME_DATA	m_tData;
	bool		m_bIsOn;
	bool		m_bIsOn2;
	float		m_fSpeed;
	float		m_fDamage;
	float		m_fAtkSpeed;
	bool		m_bIsEnd;
	bool		m_bIsEnd2;
	SKILL_DATA  m_tSkillData;
};

