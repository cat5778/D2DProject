#include "stdafx.h"
#include "MeviusScene.h"
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
#include "ImageObject.h"
#include "Mevius.h"
CMeviusScene::CMeviusScene()
	:m_pObjectMgr(CObjectMgr::GetInstance()),
	m_pColliderMgr(ColliderMgr::GetInstance()), m_pPlayer(nullptr), m_pMevius(nullptr)
{
	
}


CMeviusScene::~CMeviusScene()
{
	Release();
}

int CMeviusScene::Update()
{
	m_pObjectMgr->Update();
	m_pColliderMgr->Update();
	if (m_pPlayer != nullptr&&m_pMevius == nullptr)
	{
		D3DXVECTOR3 temp = m_cPentag->GetTagInfo().vPos - m_pPlayer->GetTagInfo().vPos;
		cout << D3DXVec3Length(&temp) << endl;
		if (D3DXVec3Length(&temp) <= 500)
		{
			//CImageObject* light = new CImageObject(L"Mevius", L"Mevius_Light", D3DXVECTOR3(1270, -115, 0), true, 0, 1, 1, 0.9);
			//light->SetEndTime(2.0f);
			//CObjectMgr::GetInstance()->AddObject(OBJECT_EFFECT, light);
			//m_pMevius = new CMevius(D3DXVECTOR3(1270, -100, 0));


			m_pMevius = new CMevius(D3DXVECTOR3(1270, 300, 0));
			m_pObjectMgr->AddObject(OBJECT_MONSTER, m_pMevius);
		}
	}

	return NO_EVENT;
}

void CMeviusScene::LateUpdate()
{
	m_pObjectMgr->LateUpdate();
	m_pColliderMgr->LateUpdate();
}

void CMeviusScene::Render()
{
	m_pObjectMgr->Render();
	if (CKeyManager::GetInstance()->KeyPressing(KEY_6))
		m_pColliderMgr->Render();
}

HRESULT CMeviusScene::Initialize()
{
	LPDIRECT3DDEVICE9 pGraphicDev = m_pDeviceMgr->GetDevice();
	NULL_CHECK_MSG_RETURN(pGraphicDev, L"pGraphicDev is null", E_FAIL);


	HRESULT	hr = m_pObjectMgr->AddObject(OBJECT_TERRAIN, CTerrain::Create(L"MebiusMap.dat"));
	FAILED_CHECK_MSG_RETURN(hr, L"Terrain Create Failed", E_FAIL);
	CTerrain::GetInstance()->LoadTile(L"../Data/MebiusMap.dat");


	CRespawnManager::GetInstance()->FileLoad(L"../Data/ChacterSelect/MebiusObj.dat");

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

	m_pPlayer = new CPlayer(D3DXVECTOR3(1270, 75, 0));
	m_pObjectMgr->AddObject(OBJECT_PLAYER, m_pPlayer);

//

	m_cPentag = new CImageObject(L"Obstacle", L"Shadow_Realm_Pentagram_Active", D3DXVECTOR3(1270, 375, 0), true, 0, 1, 1, 0.9999);
	CObjectMgr::GetInstance()->AddObject(OBJECT_EFFECT, m_cPentag);

	
	//

	return S_OK;
}

void CMeviusScene::Release()
{
	m_pColliderMgr->DestroyInstance();
	m_pObjectMgr->DestroyInstance();
	CRespawnManager::GetInstance()->DestroyInstance();
	ColliderMgr::GetInstance()->DestroyInstance();
}

CMeviusScene * CMeviusScene::Create()
{
	CMeviusScene* pInstance = new CMeviusScene;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
