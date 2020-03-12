#include "stdafx.h"
#include "Chest.h"


CChest::CChest()
{
}

CChest::CChest(const OBJ_INFO & objInfo, D3DXMATRIX mat_World)
	:m_wstrWeapon(L"Idle"), m_bIsAtkEnd(true)
{
	m_tFrame.fCurFrame = 0;
	m_tObjInfo = objInfo;
	m_tInfo.matWorld = mat_World;
	Initialize();

}

CChest::CChest(const OBJ_INFO & objInfo, D3DXVECTOR3 vPos,float fAtkSpeed)
	:m_wstrWeapon(L"Idle"), m_bIsAtkEnd(true), m_fAtkSpeed(fAtkSpeed)
{
	m_tFrame.fCurFrame = 0;
	m_tObjInfo = objInfo;
	m_tInfo.vPos = vPos;
	Initialize();

}


CChest::~CChest()
{
	
}

HRESULT CChest::Initialize()
{
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 0.f, -1.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	return S_OK;
}

void CChest::Release()
{
}

int CChest::Update()
{

	m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDelta()*m_fAtkSpeed;
	if (m_tFrame.fMaxFrame <= m_tFrame.fCurFrame)
	{
		m_tFrame.fCurFrame = 0.f;
		m_bIsAtkEnd = false;
		m_wstrWeapon = L"Idle";
	}
	return 0;
}

void CChest::LateUpdate()
{

}

void CChest::Render()
{
	UpdateMatWorld(1.0f, ZOrder(m_tInfo.vPos.y-12));
	wstring wstrStateKey = m_wstrWeapon+m_tObjInfo.wstrStateKey + m_wsImgDir;
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_tObjInfo.wstrObjectKey,
		wstrStateKey, m_tFrame.fCurFrame);
	NULL_CHECK(pTexInfo);
	m_tFrame.fMaxFrame = pTexInfo->iMaxCnt;
	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CChest::AttackAni(WEAPONE_TYPE eType)
{
	m_eWpType=eType;
	m_bIsAtkEnd = true;
	m_tFrame.fCurFrame = 0.f;
	switch (eType)
	{
	case WEAPONE_FIST:
		m_wstrWeapon = L"Fist";
		break;
	case WEAPONE_JAVELIN:
	case WEAPONE_JAVELIN1:
		m_wstrWeapon = L"Javelin";
		break;
	case WEAPONE_BOW:
	case WEAPONE_BOW1:
		m_wstrWeapon = L"Bow";
		break;

	case WEAPONE_END:
		break;
	default:
		break;
	}

}
