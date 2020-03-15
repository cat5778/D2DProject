#pragma once
class CScrollMgr
{
private:
	CScrollMgr();
	~CScrollMgr();

public:
	static const D3DXVECTOR3& GetScrollPos();
	static const D3DXVECTOR3& GetCenterPos();
	static const void ScrollPos(float fSpeed = 100);
	static const void SetCamPos(D3DXVECTOR3 vPos);
	static const void FollowCam(D3DXVECTOR3 vPos, float fSpeed);
public:
	static void MoveScrollPos(const D3DXVECTOR3& vMove);
private:
	static D3DXVECTOR3		m_vScrollPos;
	static D3DXVECTOR3		m_vScreenCenter;
	static D3DXVECTOR3		m_CenterPos;
	CDeviceMgr*		m_pDeviceMgr;
public:

};

