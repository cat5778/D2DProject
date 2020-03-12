#pragma once
#include "Scene.h"
class CPlayer;
class CTutorial :
	public CScene
{
public:
	CTutorial();
	~CTutorial();

	// CScene��(��) ���� ��ӵ�
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CTutorial* Create();

private:
	CObjectMgr*		m_pObjectMgr;
	ColliderMgr*	m_pColliderMgr;
	//CPlayer*		m_pPlayer;


};

