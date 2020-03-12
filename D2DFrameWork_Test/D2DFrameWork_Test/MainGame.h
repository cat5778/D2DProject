#pragma once
class CMainGame
{
private:
	CMainGame();

public:
	~CMainGame();

public:	
	void Update();
	void LateUpdate();
	void Render();
	void RenderScroll();
private:
	HRESULT Initialize();
	void Release();

public:
	static CMainGame* Create();
private:
	CKeyManager*		m_pKeyMgr;
	CDeviceMgr*			m_pDeviceMgr;
	CTextureMgr*		m_pTextureMgr;
	CObjectMgr*			m_pObjectMgr;
	CTimeMgr*			m_pTimeMgr;
	CSceneMgr*			m_pSceneMgr;
	CFrameMgr*			m_pFrameMgr;
	int					m_iEvent;
	HDC m_hDC;

private:
	TCHAR			m_szScrollPos[MIN_STR];

};

