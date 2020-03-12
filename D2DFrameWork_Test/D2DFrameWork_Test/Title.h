#pragma once
#include "GameObject.h"
class CTitle :
	public CGameObject
{
public:
	CTitle();
	CTitle(const OBJ_INFO& objInfo, D3DXVECTOR3 vPos);
	~CTitle();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	float ZOrder(float _y);
	void ConvertPos(const OBJ_INFO& objInfo, const TEX_INFO& pTexInfo, D3DXVECTOR3 &vPos);
	void ConvertCenter(const OBJ_INFO& objInfo, const TEX_INFO& pTexInfo, float &cx, float &cy);
private:
	bool m_isFire;
	

};

