#pragma once
#include "Scene.h"
class CPlayer;
class CBossScene :
	public CScene
{
public:
	CBossScene();
	~CBossScene();

	// CScene��(��) ���� ��ӵ�
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual HRESULT Initialize() override;
	virtual void Release() override;


public:
	static CBossScene* Create();

private:
	CObjectMgr*		m_pObjectMgr;
	ColliderMgr*	m_pColliderMgr;
	CPlayer*		m_pPlayer;



};

