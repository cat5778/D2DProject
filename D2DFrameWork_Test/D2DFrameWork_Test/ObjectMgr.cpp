#include "stdafx.h"
#include "ObjectMgr.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CObjectMgr)

CObjectMgr::CObjectMgr()
{
}


CObjectMgr::~CObjectMgr()
{
	Release();
}

CGameObject* CObjectMgr::GetTerrain() const
{
	if(m_ObjectList[OBJECT_TERRAIN].empty())
		return nullptr;

	return m_ObjectList[OBJECT_TERRAIN].front();
}

HRESULT CObjectMgr::DeleteObject(OBJECT_TYPE eType)
{
	if (!m_ObjectList[eType].empty())
	{
		auto itr = m_ObjectList[eType].begin();
		delete *itr;
		m_ObjectList[eType].erase(itr);
	}
	return S_OK;
}

HRESULT CObjectMgr::AddObject(OBJECT_TYPE eType, CGameObject* pObject)
{
	NULL_CHECK_RETURN(pObject, E_FAIL);
	m_ObjectList[eType].push_back(pObject);

	return S_OK;
}

int CObjectMgr::Update()
{
	list<CGameObject*>::iterator iter_begin, iter_end;

	for(int i = 0; i < OBJECT_END; ++i)
	{
		iter_begin = m_ObjectList[i].begin();
		iter_end = m_ObjectList[i].end();

		for (; iter_begin != iter_end; )
		{
			int iEvent = (*iter_begin)->Update();

			if (DEAD_OBJ == iEvent)
			{
				SafeDelete(*iter_begin);
				iter_begin = m_ObjectList[i].erase(iter_begin);
			}
			else
				++iter_begin;
		}
	}	

	return NO_EVENT;
}

void CObjectMgr::LateUpdate()
{
	for (int i = 0; i < OBJECT_END; ++i)
	{
		for (auto pObject : m_ObjectList[i])
			pObject->LateUpdate();
	}	
}

void CObjectMgr::Render()
{
	for (int i = 0; i < OBJECT_END; ++i)
	{
		for (auto pObject : m_ObjectList[i])
			pObject->Render();
	}
}

void CObjectMgr::Release()
{
	for (int i = 0; i < OBJECT_END; ++i)
	{
		for_each(m_ObjectList[i].begin(), m_ObjectList[i].end(), SafeDelete<CGameObject*>);
		m_ObjectList[i].clear();
	}
}
