#pragma once
#include "GameObject.h"
class CColliderBox;

class CObstacle :
	public CGameObject
{
public:
	CObstacle();
	CObstacle(const OBJ_INFO& objInfo, D3DXVECTOR3 vPos);
	~CObstacle();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	float ZOrder();
	void ConvertPos(const OBJ_INFO& objInfo, const TEX_INFO& pTexInfo, D3DXVECTOR3 &vPos);
	void ConvertCenter(const OBJ_INFO& objInfo, const TEX_INFO& pTexInfo, float &cx, float &cy);
	void CheckType();
	void ConvertSize();
private:
	OBSTACLE_TYPE	m_eType;
	D3DXVECTOR3		m_VCollPos;
	CColliderBox*	m_CollBox;
	bool			m_bIsHorizon;


};
