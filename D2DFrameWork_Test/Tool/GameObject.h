#pragma once

class CGameObject
{
protected:
	CGameObject();

public:
	virtual ~CGameObject();

protected:
	virtual HRESULT Initialize() PURE;
	virtual void Release() PURE;

public:
	virtual int Update() PURE;
	virtual void LateUpdate() PURE;
	virtual void Render() PURE;

protected:
	CDeviceMgr*		m_pDeviceMgr;
	CTextureMgr*	m_pTextureMgr;
};

