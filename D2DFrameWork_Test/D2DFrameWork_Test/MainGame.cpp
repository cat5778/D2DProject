#include "stdafx.h"
#include "MainGame.h"
#include "ScrollMgr.h"
#include "Mouse.h"
CMainGame::CMainGame()
	: 
	m_pKeyMgr(CKeyManager::GetInstance()),
	m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pObjectMgr(CObjectMgr::GetInstance()),
	m_pTimeMgr(CTimeMgr::GetInstance()),
	m_pSceneMgr(CSceneMgr::GetInstance()),
	m_pFrameMgr(CFrameMgr::GetInstance()),
	m_iEvent(NO_EVENT)
{
}


CMainGame::~CMainGame()
{
	Release();
}

HRESULT CMainGame::Initialize()
{
	CSoundMgr::GetInstance()->Initialize();

	//m_hDC = GetDC(g_hWnd);
	//m_pDevice->InitDevice(MODE_WIN);
	HRESULT hr = m_pDeviceMgr->InitDevice(MODE_FULL);
	FAILED_CHECK_MSG_RETURN(hr, L"InitDevice Failed", E_FAIL);

	hr = m_pSceneMgr->SceneChange(SCENE_END, SCENE_LOGO);
	FAILED_CHECK_MSG_RETURN(hr, L"Logo Change Failed", E_FAIL);

	
	return S_OK;
}

void CMainGame::Update()
{
	//if (CKeyManager::GetInstance()->KeyUp(KEY_1))
	//{
	//	POINT pt = {};
	//	GetCursorPos(&pt);
	//	ScreenToClient(g_hWnd, &pt);
	//	
	//}

	m_pTimeMgr->Update();
	m_pKeyMgr->Update();
	m_iEvent = m_pSceneMgr->Update();

}

void CMainGame::LateUpdate()
{
	if (CHANGE_SCENE == m_iEvent)
		return;
	CSoundMgr::GetInstance()->UpdateSound();
	m_pSceneMgr->LateUpdate();
}

void CMainGame::Render()
{

	//if (CHANGE_SCENE == m_iEvent)
	//	return;


	CDeviceMgr::GetInstance()->Render_Begin();
	m_pSceneMgr->Render();
	//RenderScroll();
	//m_pFrameMgr->RenderFPS();
	CDeviceMgr::GetInstance()->Render_End();

}

void CMainGame::RenderScroll()
{
	D3DXVECTOR3 ScrollPos = CScrollMgr::GetScrollPos();
	swprintf_s(m_szScrollPos, L"ScrollPos X=: %d  , Y= %d ", (int)ScrollPos.x, (int)ScrollPos.y);

	// 다이렉트 폰트 출력
	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, 50.f, 10.f, 0.f);

	m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);
	m_pDeviceMgr->GetFont()->DrawText(
		m_pDeviceMgr->GetSprite(), /* 스프라이트 COM 객체 */
		m_szScrollPos, /* 출력할 문자열 */
		lstrlen(m_szScrollPos), /* 문자열 길이 */
		nullptr, /* 사각형 영역 */
		0,
		D3DCOLOR_ARGB(255, 0, 255, 0));

	D3DXVECTOR3 MousePos = CMouse::GetMousePos();
	MousePos += ScrollPos;
	swprintf_s(m_szScrollPos, L"MousePos X=: %d  , Y= %d", (int)MousePos.x, (int)MousePos.y);
	// 다이렉트 폰트 출력
	D3DXMatrixTranslation(&matTrans, 50.f, 30.f, 0.f);

	m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);
	m_pDeviceMgr->GetFont()->DrawText(
		m_pDeviceMgr->GetSprite(), /* 스프라이트 COM 객체 */
		m_szScrollPos, /* 출력할 문자열 */
		lstrlen(m_szScrollPos), /* 문자열 길이 */
		nullptr, /* 사각형 영역 */
		0,
		D3DCOLOR_ARGB(255, 0, 255, 0));


}

void CMainGame::Release()
{
	CSoundMgr::GetInstance()->DestroyInstance();
	m_pFrameMgr->DestroyInstance();
	m_pSceneMgr->DestroyInstance();
	m_pTimeMgr->DestroyInstance();
	m_pObjectMgr->DestroyInstance();
	m_pKeyMgr->DestroyInstance();
	m_pTextureMgr->DestroyInstance();
	m_pDeviceMgr->DestroyInstance();

}

CMainGame* CMainGame::Create()
{
	CMainGame* pInstance = new CMainGame;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
