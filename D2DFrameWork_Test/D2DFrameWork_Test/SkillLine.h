#pragma once
#include "GameObject.h"
class CSkillLine :
	public CGameObject
{
public:
	CSkillLine();
	~CSkillLine();

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
};

