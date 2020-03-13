#include "stdafx.h"
#include "Tutorial.h"
#include "Terrain.h"
#include "Choose.h"
#include "Obstacle.h"
#include "TextField.h"
#include "RespawnManager.h"
#include "ScrollMgr.h"
#include "Player.h"
#include "Monster.h"
#include "Fungus.h"
#include "Silme.h"
#include "Monk.h"
#include "Priest.h"
CTutorial::CTutorial()
	:m_pObjectMgr(CObjectMgr::GetInstance()),
	m_pColliderMgr(ColliderMgr::GetInstance())
{
}


CTutorial::~CTutorial()
{
	Release();
}

int CTutorial::Update()
{
	m_pObjectMgr->Update();
	m_pColliderMgr->Update();
	//CScrollMgr::ScrollPos();
	//cout << m_pObjectMgr->GetObjList(OBJECT_MONSTER).size() << endl;
	return NO_EVENT;
}

void CTutorial::LateUpdate()
{
	m_pObjectMgr->LateUpdate();
	m_pColliderMgr->LateUpdate();
}


void CTutorial::Render()
{
	m_pObjectMgr->Render();
	if (CKeyManager::GetInstance()->KeyPressing(KEY_6))
		m_pColliderMgr->Render();

}

HRESULT CTutorial::Initialize()
{
	LPDIRECT3DDEVICE9 pGraphicDev = m_pDeviceMgr->GetDevice();
	NULL_CHECK_MSG_RETURN(pGraphicDev, L"pGraphicDev is null", E_FAIL);

	
	HRESULT	hr = m_pObjectMgr->AddObject(OBJECT_TERRAIN, CTerrain::Create(L"TutorialWater.dat"));
	FAILED_CHECK_MSG_RETURN(hr, L"Terrain Create Failed", E_FAIL);
	CTerrain::GetInstance()->LoadTile(L"../Data/CharacterSelcet.dat");
	

	CRespawnManager::GetInstance()->FileLoad(L"../Data/ChacterSelect/TutorialObj.dat");

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
		case OBJECT_MONSTER:
		{
			if ((Obj.first)->wstrObjectName.find(L"Fungus") != wstring::npos)
			{
				CFungus *temp = new CFungus(*Obj.first, Obj.second);
				hr = m_pObjectMgr->AddObject(OBJECT_MONSTER, temp);
			}
			else if ((Obj.first)->wstrObjectName.find(L"Slime") != wstring::npos)
			{
				CSilme *temp = new CSilme(*Obj.first, Obj.second);
				hr = m_pObjectMgr->AddObject(OBJECT_MONSTER, temp);
			}
			else if ((Obj.first)->wstrObjectName.find(L"Monk") != wstring::npos)
			{
				Monk *temp = new Monk(*Obj.first, Obj.second);
				hr = m_pObjectMgr->AddObject(OBJECT_MONSTER, temp);
			}
			else if ((Obj.first)->wstrObjectName.find(L"Priest") != wstring::npos)
			{
				Priest *temp = new Priest(*Obj.first, Obj.second);
				hr = m_pObjectMgr->AddObject(OBJECT_MONSTER, temp);
			}
			FAILED_CHECK_MSG_RETURN(hr, Obj.first->wstrObjectName.c_str(), E_FAIL);
		}
		case OBJECT_END:
			break;
		default:
			break;
		}

	}
	CPlayer* pPlayer = new CPlayer(D3DXVECTOR3(300,200,0));
	m_pObjectMgr->AddObject(OBJECT_PLAYER, pPlayer);

	return S_OK;

}

void CTutorial::Release()
{
}

CTutorial * CTutorial::Create()
{
	CTutorial* pInstance = new CTutorial;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
