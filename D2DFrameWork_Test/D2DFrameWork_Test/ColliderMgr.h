#pragma once
class CGameObject;
class ColliderMgr
{
	DECLARE_SINGLETON(ColliderMgr)
private:
	ColliderMgr();
	~ColliderMgr();
	
public:
	list<CGameObject*> GetObjList(COLLSION_TYPE eType) { return m_CollList[eType]; }
	HRESULT AddObject(COLLSION_TYPE eType, CGameObject* pObject);
	void DeleteObject(COLLSION_TYPE eType);
	int Update();
	void LateUpdate();
	void Render();
	void CheckCollision(COLLSION_TYPE sourType, COLLSION_TYPE destType);
	void HitCollision(COLLSION_TYPE sourType, COLLSION_TYPE destType);


private:
	void Release();

private:
	list<CGameObject*>	m_CollList[COLLSION_END];

};

