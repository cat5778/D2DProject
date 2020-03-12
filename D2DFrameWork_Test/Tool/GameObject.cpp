#include "stdafx.h"
#include "GameObject.h"


CGameObject::CGameObject()
	: m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance())
{
}


CGameObject::~CGameObject()
{
}
