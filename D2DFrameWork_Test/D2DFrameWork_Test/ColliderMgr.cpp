#include "stdafx.h"
#include "ColliderMgr.h"
#include "ColliderBox.h"
IMPLEMENT_SINGLETON(ColliderMgr)

ColliderMgr::ColliderMgr()
{
}


ColliderMgr::~ColliderMgr()
{
	Release();
}


HRESULT ColliderMgr::AddObject(COLLSION_TYPE eType, CGameObject * pObject)
{
	NULL_CHECK_RETURN(pObject, E_FAIL);
	m_CollList[eType].push_back(pObject);

	return S_OK;
}

void ColliderMgr::DeleteObject(COLLSION_TYPE eType)
{
	for (int i = 0; i < COLLSION_END; ++i)
	{
		for (auto pObject : m_CollList[i])
		{

			if (dynamic_cast<CColliderBox*>(pObject)->IsDead())
			{
				delete pObject;
				pObject = nullptr;
			}
		}
	}
}

int ColliderMgr::Update()
{
	list<CGameObject*>::iterator iter_begin, iter_end;

	for (int i = 0; i < COLLSION_END; ++i)
	{
		iter_begin = m_CollList[i].begin();
		iter_end = m_CollList[i].end();

		for (; iter_begin != iter_end; )
		{
			int iEvent = (*iter_begin)->Update();

			if (DEAD_OBJ == iEvent)
			{
				SafeDelete(*iter_begin);
				iter_begin = m_CollList[i].erase(iter_begin);
			}
			else
				++iter_begin;
		}
	}
	CheckCollision(PLAYER_HITBOX_COLLISION, OBJECT_HITBOX_COLLISION);
	HitCollision(PLAYER_PROJECTILE_COLLISION, MONSTER_HITBOX_COLLISION);
	CheckCollision(MONSTER_HITBOX_COLLISION, OBJECT_HITBOX_COLLISION);
	HitCollision(MONSTER_PROJECTILE_COLLISION, PLAYER_HITBOX_COLLISION);
	return NO_EVENT;
}

void ColliderMgr::LateUpdate()
{
	for (int i = 0; i < COLLSION_END; ++i)
	{
		for (auto pObject : m_CollList[i])
		{

			pObject->LateUpdate();
		}
	}
}

void ColliderMgr::Render()
{
	for (int i = 0; i < COLLSION_END; ++i)
	{
		for (auto pObject : m_CollList[i])
		{

			if(!dynamic_cast<CColliderBox*>(pObject)->IsDead())
				dynamic_cast<CColliderBox*>(pObject)->RenderLine();
		}
	}
}

void ColliderMgr::HitCollision(COLLSION_TYPE sourType, COLLSION_TYPE destType)
{
	for (auto pSource : m_CollList[sourType])
	{
		pSource->SetCollision(false);
		for (auto pDest : m_CollList[destType])
		{
			if (!dynamic_cast<CColliderBox*>(pDest)->GetActice())
				continue;
			
			D3DXVECTOR3 temp = (pSource->GetTagInfo().vPos - pDest->GetTagInfo().vPos);
			//D3DXVECTOR3 vSize = { pSource->GetvSize().x+ pDest->GetvSize().x,pSource->GetvSize().y+ pDest->GetvSize().y,0 };
			D3DXVECTOR2 vSize = (pSource->GetvSize() + pDest->GetvSize());
			float fDistance = fabsf(D3DXVec3Length(&temp));
			float fSize = fabsf(D3DXVec2Length(&vSize));
			if (fDistance <= fSize*0.5f)
			{
				pSource->SetCollision(true);
				pDest->SetCollision(true,dynamic_cast<CColliderBox*>(pSource)->GetCollType(), pSource->GetGameData().fDamage);
				pDest->SetKnockDir(pSource->GetvDir());
			}
			else
			{
				pDest->SetCollision(false);
			}
		}
	}
}

void ColliderMgr::CheckCollision(COLLSION_TYPE sourType, COLLSION_TYPE destType)
{
	for (auto pSource : m_CollList[sourType])
	{
		pSource->SetOBJCollision(false);
		for (auto pDest : m_CollList[destType])
		{
			if (pSource == pDest)
				continue;
			D3DXVECTOR3 temp = (pSource->GetTagInfo().vPos - pDest->GetTagInfo().vPos);
			D3DXVECTOR2 vSize = (pSource->GetvSize() + pDest->GetvSize());
			float fDistance = fabsf(D3DXVec3Length(&temp));
			float fSize = fabsf(D3DXVec2Length(&vSize));
			if (fDistance <= fSize*0.5f)
			{
				pSource->SetOBJCollision(true,destType);
				pDest->SetOBJCollision(true,sourType);
				pSource->GetCollDir(pDest->GetTagInfo().vPos);
				//cout << "Ãæµ¹" << endl;
			}
			else
			{
				pDest->SetOBJCollision(false, destType);
			}
		}
	}
}

void ColliderMgr::Release()
{
	for (int i = 0; i < COLLSION_END; ++i)
	{
		for_each(m_CollList[i].begin(), m_CollList[i].end(), SafeDelete<CGameObject*>);
		m_CollList[i].clear();
	}
}
