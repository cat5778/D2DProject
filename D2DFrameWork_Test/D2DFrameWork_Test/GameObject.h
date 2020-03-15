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
	GAME_DATA GetGameData() { return m_tData; }
	D3DXVECTOR2 GetvSize() { return m_vSize; }
	D3DXVECTOR3 GetvDir() { return m_vDir; }
	void SetDir(D3DXVECTOR3 vec) { m_vDir = vec; }
	D3DXVECTOR3 GetvKnock() { return m_vKnockDir; }
	COLLSION_TYPE GetHitColl() { return m_HitCollType; }
	void BeAttack(float fDamage) { m_tData.fCurHp -= fDamage;/*ChangeState(STATE_BEATTACK);*/ }
	void InitCurHP() { m_tData.fCurHp = 0; }

	void SetKnockDir(D3DXVECTOR3 vDir) { m_vKnockDir=vDir; }
	void SetDamage(float fDamage) { m_tData.fDamage=fDamage; }
	void SetHitColl(COLLSION_TYPE eType) {m_HitCollType = eType;}
	bool IsCollsion() { return m_bIsCollsion; }
	bool IsOBJCollsion() { return m_bIsObjCollision; }
	void SetCollision(bool bIsColl, COLLSION_TYPE CollType = COLLSION_END, float fDamage = -1.0f);
	void SetOBJCollision(bool bIsColl, COLLSION_TYPE CollType = COLLSION_END, float fDamage = -1.0f);
	void SetvPos(D3DXVECTOR3 vPos, DWORD Dir = DOWN);
	void Timer(bool& SetBool,float fRate,float& fTime);
	float GetRadian() { return m_fRadian; }
	DWORD GetCollDir(D3DXVECTOR3 vCollPos);
	float GetCollRadian() { return m_fCollRadian; }
	void GetAngle(D3DXVECTOR3 targetPos);
	DWORD GetdwCollDir() {	return m_dwCollDir;}
protected:
	bool Animate(bool bISInfinite =true,float fSpeed=1.f);
	void UpdateMatWorld(float fScale=1, float fZLayer=1, float fAngle=0.f);
	float ZOrder(float _y);
	void GetImageDir(DWORD dwDir);
	void ChangeState(OBJECT_STATE eState);
	void KnockBack(D3DXVECTOR3 vKnock, float fPower);
	void UpdateRect();
protected:
	CDeviceMgr*		m_pDeviceMgr;
	CTextureMgr*	m_pTextureMgr;
	ColliderMgr*	m_pColliderMgr;
	CTimeMgr*		m_pTimeMgr;
	OBJ_INFO		m_tObjInfo;
	TAG_INFO		m_tInfo;
	FRAME			m_tFrame;
	wstring			m_wsImgDir;
	wstring			m_wsState;
	wstring			m_wsIdleState;
	DWORD			m_dwDir;
	DWORD			m_dwCollDir;
	CColliderBox*	m_ColliderBox;
	D3DXVECTOR2		m_vSize;
	float			m_fRadian;
	float			m_fCollRadian;
	OBJECT_STATE	m_eCurState;
	OBJECT_STATE	m_eOldState;
	vRect			m_tRect;
	bool			m_bIsObjCollision;
	bool			m_bIsCollsion;
	GAME_DATA		m_tData;
	COLLSION_TYPE	m_HitCollType;
	COLLSION_TYPE	m_eProjectileType;
	float			m_fTimer;
	bool			m_bIsDead;
	OBJECT_TYPE		m_eType;
	D3DXVECTOR3		m_vDir;
	D3DXVECTOR3		m_vKnockDir;
	bool			m_bIsInvincible;
	float			m_fKnockTime;
	bool			m_bIsAttack;

	
};

