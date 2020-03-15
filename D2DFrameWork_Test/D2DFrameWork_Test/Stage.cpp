#include "stdafx.h"
#include "Stage.h"
#include "Terrain.h"
#include "Player.h"
#include "Choose.h"
#include "Obstacle.h"
#include "TextField.h"
#include "RespawnManager.h"
#include "ScrollMgr.h"
CStage::CStage()
	: m_pObjectMgr(CObjectMgr::GetInstance()),
	m_pCollisionMgr(ColliderMgr::GetInstance())
{
}


CStage::~CStage()
{
	Release();
}

int CStage::Update()
{
	m_pObjectMgr->Update();
	//CScrollMgr::ScrollPos();
	bool bIsChoose=false;
	auto chooseList = m_pObjectMgr->GetObjList(OBJECT_CHOOSE);
	for (auto pChoose : chooseList)
	{
		
		CChoose* temp = dynamic_cast<CChoose*>(pChoose);
		if (temp != nullptr&&temp->GetIsChoose())
		{
				bIsChoose = true;

				break;
		}
		
	}
	auto uiList = m_pObjectMgr->GetObjList(OBJECT_UI);
	for (auto pUI : uiList)
	{
		CTextField* temp = dynamic_cast<CTextField*>(pUI);
		if (temp != nullptr &&temp->m_IsPick&&bIsChoose)
		{
			m_pSoundMgr->PlaySound(L"Amazon_snd.wav", EFFECT);

			m_pSoundMgr->PlaySound(L"Menu_Click.wav", EFFECT);
			m_pSoundMgr->StopSound(BGM);
			m_pSceneMgr->SceneChange(SCENE_STAGE, SCENE_TUTORIAL);
			return CHANGE_SCENE;
		}
	
	}
	m_pCollisionMgr->Update();
	return NO_EVENT;
}

void CStage::LateUpdate()
{
	m_pObjectMgr->LateUpdate();
	m_pCollisionMgr->LateUpdate();
}

void CStage::Render()
{
	m_pObjectMgr->Render();
	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, WINCX*0.38f, WINCY*0.84f, 0.f);

	wstring temp = L"확인";
	m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);
	m_pDeviceMgr->GetFont()->DrawText(
		m_pDeviceMgr->GetSprite(), /* 스프라이트 COM 객체 */
		temp.c_str(), /* 출력할 문자열 */
		temp.size(), /* 문자열 길이 */
		nullptr, /* 사각형 영역 */
		0,
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CStage::Initialize()
{
	LPDIRECT3DDEVICE9 pGraphicDev = m_pDeviceMgr->GetDevice();
	NULL_CHECK_MSG_RETURN(pGraphicDev, L"pGraphicDev is null", E_FAIL);

	// MultiTexture Load
	//HRESULT hr = m_pTextureMgr->LoadTextureFromPathInfo(pGraphicDev, L"../Data/PathInfo.txt");
	//FAILED_CHECK_MSG_RETURN(hr, L"Stage LoadTextureFromPathInfo Failed", E_FAIL);

	// Terrain Object
	HRESULT	hr = m_pObjectMgr->AddObject(OBJECT_TERRAIN, CTerrain::Create(L"CharacterSelcetWater.dat"));
	FAILED_CHECK_MSG_RETURN(hr, L"Terrain Create Failed", E_FAIL);
	CTerrain::GetInstance()->LoadTile(L"../Data/CharacterSelcet.dat");
	// Player Object
	//hr = m_pObjectMgr->AddObject(OBJECT_PLAYER, CPlayer::Create());
	//FAILED_CHECK_MSG_RETURN(hr, L"Player Create Failed", E_FAIL);
	//CharacterSelcetObj
	CRespawnManager::GetInstance()->FileLoad(L"../Data/CharacterSelcetObj.dat");
	
	for (auto Obj : CRespawnManager::GetInstance()->m_mObjects)//리스폰 매니저에서 멀티맵 가져와서 obstacle클래스에 넣은후에 렌더링 확인
	{
		switch (ConvertionWtoE(Obj.first->wstrObjectKey))
		{
		case OBJECT_OBSTACLE:
		{
			CObstacle *temp = new CObstacle(*Obj.first, Obj.second);
			hr = m_pObjectMgr->AddObject(OBJECT_OBSTACLE, temp);
			FAILED_CHECK_MSG_RETURN(hr, Obj.first->wstrObjectName.c_str(), E_FAIL);
		}
			break;
		case OBJECT_UI:
			break;
		case OBJECT_CHOOSE:
		{
			CChoose *temp = new CChoose(*Obj.first, Obj.second);
			hr = m_pObjectMgr->AddObject(OBJECT_CHOOSE, temp);
			FAILED_CHECK_MSG_RETURN(hr, Obj.first->wstrObjectName.c_str(), E_FAIL);
		}
			break;
		case OBJECT_END:
			break;
		default:
			break;
		}

	}
	CTextField* nameText = new CTextField(D3DXVECTOR3(WINCX*0.4f,WINCY*0.85f,0));
	m_pObjectMgr->AddObject(OBJECT_UI, nameText);

	//list<CGameObject*> m_objList=m_pObjectMgr->GetObjList(OBJECT_OBSTACLE);
	//cout << "df" << endl;
	return S_OK;
}

void CStage::Release()
{
	//CTerrain::Release();
	m_pCollisionMgr->DestroyInstance();
	m_pObjectMgr->DestroyInstance();
	CRespawnManager::GetInstance()->DestroyInstance();
	ColliderMgr::GetInstance()->DestroyInstance();
}

CStage* CStage::Create()
{
	CStage* pInstance = new CStage;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}