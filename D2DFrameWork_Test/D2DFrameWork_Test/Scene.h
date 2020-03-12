#pragma once

class CScene
{
protected:
	CScene();

public:
	virtual ~CScene();

public:
	virtual int Update() PURE;
	virtual void LateUpdate() PURE;
	virtual void Render() PURE;
public:
	HRESULT ReadData(wstring wstrFilePath);
	OBJECT_TYPE ConvertionWtoE(wstring csobjType);
	//wstring ConvertionEtoW(OBJECT_TYPE eObjectType);

protected:
	virtual HRESULT Initialize() PURE;
	virtual void Release() PURE;

protected:
	CDeviceMgr*		m_pDeviceMgr;
	CTextureMgr*	m_pTextureMgr;
	CTimeMgr*		m_pTimeMgr;
	CSceneMgr*		m_pSceneMgr;
};

