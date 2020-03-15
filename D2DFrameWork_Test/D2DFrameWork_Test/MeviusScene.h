#pragma once
#include "Scene.h"
class CPlayer;
class CImageObject;
class CMevius;
class CMeviusScene :
	public CScene
{
public:
	CMeviusScene();
	~CMeviusScene();

	// CScene��(��) ���� ��ӵ�
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CMeviusScene* Create();

private:
	CObjectMgr*		m_pObjectMgr;
	ColliderMgr*	m_pColliderMgr;
	CPlayer*		m_pPlayer;
	CImageObject*	m_cPentag;
	CMevius			*m_pMevius;
};

