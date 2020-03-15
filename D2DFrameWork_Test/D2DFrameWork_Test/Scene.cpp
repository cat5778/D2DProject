#include "stdafx.h"
#include "Scene.h"


CScene::CScene()
	: m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pTimeMgr(CTimeMgr::GetInstance()),
	m_pSceneMgr(CSceneMgr::GetInstance()),
	m_pSoundMgr(CSoundMgr::GetInstance())
{
}


CScene::~CScene()
{
}

HRESULT CScene::ReadData(wstring wstrFilePath)
{
	return S_OK;
}

OBJECT_TYPE CScene::ConvertionWtoE(wstring wsObjType)
{
	if (wsObjType.compare(L"Building") == 0)
		return OBJECT_BUILDING;
	else if (wsObjType.compare(L"Tree") == 0)
		return  OBJECT_TREE;
	else if (wsObjType.compare(L"Choose") == 0)
		return  OBJECT_CHOOSE;
	else if (wsObjType.compare(L"Grass") == 0)
		return  OBJECT_GRASS;
	else if (wsObjType.compare(L"NPC") == 0)
		return OBJECT_NPC;
	else if (wsObjType.compare(L"Monster") == 0)
		return OBJECT_MONSTER;
	else if (wsObjType.compare(L"Player") == 0)
		return OBJECT_PLAYER;
	else if (wsObjType.compare(L"Trap") == 0)
		return OBJECT_TRAP;
	else if (wsObjType.compare(L"Obstacle") == 0)
		return OBJECT_OBSTACLE;
	else if (wsObjType.compare(L"Tile") == 0)
		return OBJECT_TERRAIN;
	else if (wsObjType.compare(L"Title") == 0)
		return OBJECT_TITLE;
}

//wstring CScene::ConvertionEtoW(OBJECT_TYPE eObjectType)
//{
//	switch (eObjectType)
//	{
//	case OBJECT_TERRAIN:
//		
//		break;
//	case OBJECT_OBSTACLE:
//		break;
//	case OBJECT_TRAP:
//		break;
//	case OBJECT_BUILDING:
//		break;
//	case OBJECT_MONSTER:
//		break;
//	case OBJECT_NPC:
//		break;
//	case OBJECT_GRASS:
//		break;
//	case OBJECT_TREE:
//		break;
//	case OBJECT_PLAYER:
//		break;
//	case OBJECT_EFFECT:
//		break;
//	case OBJECT_UI:
//		break;
//	case OBJECT_BUTTON:
//		break;
//	case OBJECT_CHOOSE:
//		break;
//	case OBJECT_TITLE:
//		break;
//	case OBJECT_END:
//		break;
//	default:
//		break;
//	}
//}
