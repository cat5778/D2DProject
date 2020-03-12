#pragma once
class CColliderBox;
class CGameObject
{
protected:
	CGameObject();

public:
	virtual ~CGameObject();

protected:
	virtual HRESULT Initialize() PURE;
	virtual void Release() PURE;

public:
	virtual int Update() PURE;
	virtual void LateUpdate() PURE;
	virtual void Render() PURE;
public:
	OBJ_INFO GetObjInfo() { return m_tObjInfo; }
	TAG_INFO GetTagInfo() { return m_tInfo; }
	D3DXVECTOR2 GetvSize() { return m_vSize; }
	bool IsCollsion() { return m_bIsCollsion; }
	void SetCollision(bool bIsColl, COLLSION_TYPE CollType = COLLSION_END, float fDamage = -1.0f);
	void SetvPos(D3DXVECTOR3 vPos, DWORD Dir = DOWN);
	void BeAttack(float fDamage) { m_tData.fCurHp -= fDamage;ChangeState(STATE_BEATTACK); }
	GAME_DATA GetGameData() { return m_tData; }
	void InitCurHP() { m_tData.fCurHp = 0; }
	void Timer(bool bIsActive);
	float GetRadian() { return m_fRadian; }

	D3DXVECTOR3 GetvDir() { return m_vDir; }
	D3DXVECTOR3 GetvKnock() { return m_vKnockDir; }
	void SetKnockDir(D3DXVECTOR3 vDir) { m_vKnockDir=vDir; }

	void SetDamage(float fDamage) { m_tData.fDamage=fDamage; }
	COLLSION_TYPE GetHitColl() { return m_HitCollType; }
	void SetHitColl(COLLSION_TYPE eType) {m_HitCollType = eType;}

protected:
	bool Animate(bool bISInfinite =true,float fSpeed=1.f);
	void UpdateMatWorld(float fScale=1, float fZLayer=1, float fAngle=0.f);
	float ZOrder(float _y);
	void GetImageDir(DWORD dwDir);
	void GetAngle(D3DXVECTOR3 targetPos);
	void ChangeState(OBJECT_STATE eState);
	void KnockBack(D3DXVECTOR3 vKnock, float fPower);
protected:
	CDeviceMgr*		m_pDeviceMgr;
	CTextureMgr*	m_pTextureMgr;
	ColliderMgr*	m_pColliderMgr;
	CTimeMgr*		m_pTimeMgr;
	OBJ_INFO		m_tObjInfo;
	TAG_INFO		m_tInfo;
	FRAME			m_tFrame;
	wstring			m_wsImgDir;
	DWORD			m_dwDir;
	CColliderBox*	m_ColliderBox;
	D3DXVECTOR2		m_vSize;
	float			m_fRadian;
	bool			m_bIsCollsion;
	GAME_DATA		m_tData;
	COLLSION_TYPE	m_HitCollType;
	OBJECT_STATE	m_eCurState;
	OBJECT_STATE	m_eOldState;
	COLLSION_TYPE	m_eProjectileType;
	float			m_fTimer;
	bool			m_bIsDead;
	OBJECT_TYPE		m_eType;
	D3DXVECTOR3		m_vDir;
	D3DXVECTOR3		m_vKnockDir;

	
};

