#include "stdafx.h"
#include "Logo.h"
#include "RespawnManager.h"
#include "Choose.h"
#include "Obstacle.h"
#include "Title.h"
#include "GameButton.h"
CLogo::CLogo()
	:m_pObjectMgr(CObjectMgr::GetInstance()),
	m_pKeyMgr(CKeyManager::GetInstance())
{
}


CLogo::~CLogo()
{
	Release();
}

int CLogo::Update()
{
	m_pObjectMgr->Update();
	//cout << g_fWheel << endl;

	if (m_Button->GetIsPick())
	{
		m_pSceneMgr->SceneChange(SCENE_LOGO, SCENE_STAGE);
		//m_pSceneMgr->SceneChange(SCENE_LOGO, SCENE_TUTORIAL);
		//m_pSceneMgr->SceneChange(SCENE_LOGO, SCENE_BOSS);
		//m_pSceneMgr->SceneChange(SCENE_LOGO, SCENE_MEVIUS);
		return CHANGE_SCENE;
	}

	return NO_EVENT;
}

void CLogo::LateUpdate()
{
	m_pObjectMgr->LateUpdate();

}

void CLogo::Render()
{
	m_pObjectMgr->Render();
}

HRESULT CLogo::Initialize()
{
	LPDIRECT3DDEVICE9 pGraphicDev = m_pDeviceMgr->GetDevice();
	NULL_CHECK_MSG_RETURN(pGraphicDev, L"pGraphicDev is null", E_FAIL);

	// MultiTexture Load
	HRESULT hr = m_pTextureMgr->LoadTextureFromPathInfo(pGraphicDev, L"../Data/PathInfo.txt");
	FAILED_CHECK_MSG_RETURN(hr, L"Stage LoadTextureFromPathInfo Failed", E_FAIL);

	CRespawnManager::GetInstance()->FileLoad(L"../Data/Logo.dat");

	for (auto Obj : CRespawnManager::GetInstance()->m_mObjects)//리스폰 매니저에서 멀티맵 가져와서 obstacle클래스에 넣은후에 렌더링 확인
	{
		switch (ConvertionWtoE(Obj.first->wstrObjectKey))
		{
		case OBJECT_TITLE:
			CTitle *temp = new CTitle(*Obj.first, Obj.second);
			hr = m_pObjectMgr->AddObject(OBJECT_CHOOSE, temp);
			FAILED_CHECK_MSG_RETURN(hr, Obj.first->wstrObjectName.c_str(), E_FAIL);
			break;

		}
	}

	//Button
	OBJ_INFO* temp = new OBJ_INFO;
	temp->wstrObjectName = L"Logo";
	temp->wstrObjectKey = L"Title";
	//temp->wstrStateKey = L"Menu_Button_Main";
	temp->wstrStateKey = L"Menu_Button_Main";
	temp->IsAni = false;
	temp->ImageIDX = 0;
	temp->eObjectType = OBJECT_BUTTON;
	D3DXVECTOR3 tempPos = { 640,700,0 };
	m_Button = new CGameButton(*temp, tempPos);
	m_Button->SetButtonText(L"Game Start");
	delete temp;

	m_pObjectMgr->AddObject(OBJECT_BUTTON, m_Button);
	////
	return S_OK;
}

void CLogo::Release()
{
	m_pObjectMgr->DestroyInstance();
	CRespawnManager::GetInstance()->DestroyInstance();
}

CLogo* CLogo::Create()
{
	CLogo* pInstance = new CLogo;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
