#include "stdafx.h"
#include "Player.h"
#include "ScrollMgr.h"
#include "Mouse.h"
#include "Hat.h"
#include "Chest.h"
#include "Leg.h"
#include "ColliderBox.h"
#include "Weapon.h"
#include "HPBar.h"
CPlayer::CPlayer()
	: m_pKeyMgr(CKeyManager::GetInstance()),m_pHat(nullptr),m_pChest(nullptr),
	m_pLeg(nullptr), m_fSpeed(0.f), m_wstrChest(L"Idle"), m_pWeapon(nullptr), m_CollBox(nullptr), m_pHPBar(nullptr)
{
}

CPlayer::CPlayer(D3DXVECTOR3 vPos)
	: m_pKeyMgr(CKeyManager::GetInstance()), m_pHat(nullptr), m_pChest(nullptr),
	m_pLeg(nullptr), m_fSpeed(0.f), m_wstrChest(L"Idle"), m_pWeapon(nullptr),
	m_fAtkSpd(2),m_CollBox(nullptr), m_pHPBar(nullptr)
{
	m_bIsInvincible = false;
	m_tData.fAtkSpeed = 1.0f;
	m_fTimer = 0;
	m_bIsAttack = false;
	m_eType = OBJECT_PLAYER;
	m_bIsObjCollision = false;
	m_bIsCollsion = false;
	m_tInfo.vPos = vPos;
	Initialize();
 
}


CPlayer::~CPlayer()
{
	Release();
}

int CPlayer::Update()
{		
	//
	//D3DXVECTOR3 DirPos = (CMouse::GetMousePos() + CScrollMgr::GetScrollPos()) - m_tInfo.vPos;
	//D3DXVECTOR3 tempPos = m_tInfo.vPos;
	//tempPos.x += 1;
	//D3DXVECTOR3 LookPos = tempPos - m_tInfo.vPos;
	//D3DXVec3Normalize(&DirPos, &DirPos);
	//D3DXVECTOR3 temp;
	//D3DXVec3Normalize(&temp, &LookPos);
	//float m_fRadian = acosf(D3DXVec3Dot(&DirPos,&temp ));
	//if (m_tInfo.vPos.y < (CMouse::GetMousePos().y+ CScrollMgr::GetScrollPos().y))
	//	m_fRadian *= -1.f;
	//cout << m_fRadian / PI * 180 << endl;
	//
	
	KeyInput();
	if (m_pHat != nullptr)
		m_pHat->Update();
	if (m_pChest != nullptr)
		m_pChest->Update();
	if (m_pLeg != nullptr)
		m_pLeg->Update();
	if (m_pWeapon != nullptr)
		m_pWeapon->Update();

	if (m_CollBox != nullptr)
	{
		m_CollBox->SetvPos(m_tInfo.vPos);
		m_bIsCollsion = m_CollBox->IsCollsion();
		m_bIsObjCollision = m_CollBox->IsOBJCollsion();
		if (m_bIsCollsion)
		{
			
			//cout << m_dwCollDir << "  충돌=" << m_bIsCollsion << endl;
			if (m_CollBox->GetHitColl() != COLLSION_END)
			{
				BeAttack(-m_CollBox->GetGameData().fCurHp);
				m_CollBox->SetHitColl(COLLSION_END);
				m_bIsInvincible = true;

			}
		}
		//cout << m_bIsObjCollision << endl;
		if (m_bIsObjCollision)
			m_dwCollDir = m_CollBox->GetdwCollDir();
		else
			m_dwCollDir = -1;

		

		if (m_bIsInvincible)
		{
			KnockBack(m_CollBox->GetvKnock(), 10.f*(-m_CollBox->GetGameData().fCurHp));
			m_CollBox->InitCurHP();
		}
		Timer(m_bIsInvincible, 0.1f, m_fKnockTime);
	}

	return NO_EVENT;
}

void CPlayer::LateUpdate()
{
	D3DXMATRIX matScale, matTrans;

	D3DXMatrixScaling(&matScale, 
		m_tInfo.vSize.x, 
		m_tInfo.vSize.y,
		0.f); // 다이렉트 크기행렬 함수
	D3DXMatrixTranslation(&matTrans, 
		m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x, 
		m_tInfo.vPos.y - CScrollMgr::GetScrollPos().y,
		1.f); // 다이렉트 이동행렬 함수

	m_tInfo.matWorld = matScale * matTrans;
	
	D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vLook, &m_tInfo.matWorld);
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
}

void CPlayer::Render()
{
	if (m_pHat != nullptr)
	{
		m_pHat->SetvPos(m_tInfo.vPos,m_dwDir);
		m_pHat->Render();
	}
	if (m_pChest != nullptr)
	{
		m_pChest->SetvPos(m_tInfo.vPos,m_dwDir);
		m_pChest->Render();
	}

	if (m_pLeg != nullptr)
	{
		m_pLeg->SetvPos(m_tInfo.vPos, m_dwDir);
		m_pLeg->Render();
	}
	if (m_pWeapon != nullptr)
	{
		m_pWeapon->SetvPos(m_tInfo.vPos, m_dwDir);
		m_pWeapon->Render();
	}
	if (m_pHPBar != nullptr)
	{
		m_pHPBar->SetData(m_tInfo.vPos,m_tData,25);
		m_pHPBar->Render();
	}
}

HRESULT CPlayer::Initialize()
{

	m_fSpeed = 300;
	D3DXMatrixIdentity(&m_tInfo.matWorld); // 다이렉트 항등행렬 함수
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_tData.fCurEXE = 0;
	m_tData.fHp = 100;
	m_tData.fCurHp = m_tData.fHp;
	m_tData.fDamage=100;
	m_tData.fOldHp = m_tData.fCurHp;
	
#pragma region InitBody

	if (m_pHat == nullptr)
	{

		OBJ_INFO* temp = new OBJ_INFO;
		temp->wstrObjectName = L"Amzon_Hat_Down";
		temp->wstrObjectKey = L"Hat";
		temp->wstrStateKey = L"Amazon_Hat";
		temp->IsAni = true;
		temp->ImageIDX = 0;
		temp->eObjectType = OBJECT_PLAYER;
		//m_pHat = new CHat(*temp, m_tInfo.matWorld);
		m_pHat = new CHat(*temp, m_tInfo.vPos);
		delete temp;
	}
	if (m_pChest == nullptr)
	{
		OBJ_INFO* temp = new OBJ_INFO;
		wstring tempChest = L"_Chest";
		temp->wstrObjectName = L"Idle_Chest_Down";
		temp->wstrObjectKey = L"Amazon";
		temp->wstrStateKey = /*m_wstrChest+*/tempChest;
		temp->IsAni = true;
		temp->ImageIDX = 0;
		temp->eObjectType = OBJECT_PLAYER;
		//m_pChest = new CChest(*temp, m_tInfo.matWorld);
		m_pChest = new CChest(*temp, m_tInfo.vPos, m_fAtkSpd);
		delete temp;
	}
	if (m_pLeg == nullptr)
	{
		OBJ_INFO* temp = new OBJ_INFO;
		temp->wstrObjectName = L"Leg_Down";
		temp->wstrObjectKey = L"Amazon";
		temp->wstrStateKey = L"Leg";
		temp->IsAni = true;
		temp->ImageIDX = 0;
		temp->eObjectType = OBJECT_PLAYER;
		//m_pChest = new CChest(*temp, m_tInfo.matWorld);
		m_pLeg = new CLeg(*temp, m_tInfo.vPos);
			delete temp;
	}
#pragma endregion
	m_pHPBar = new CHPBar(OBJECT_PLAYER, m_tInfo.vPos,m_tData);
	m_pWeapon = new CWeapon(m_tInfo.vPos,m_fAtkSpd,m_tData.fDamage);

	m_vSize = {22,40};
	m_CollBox = new CColliderBox(m_tInfo.vPos, PLAYER_HITBOX_COLLISION, m_vSize);
	m_pColliderMgr->AddObject(PLAYER_HITBOX_COLLISION, m_CollBox);

	return S_OK;
}

void CPlayer::Release()
{
}

void CPlayer::KeyInput()
{


	if (m_pKeyMgr->KeyPressing(KEY_A)&&m_tInfo.vPos.x>=25)
	{
		m_dwDir = LEFT;
		if (m_dwCollDir == LEFT&&m_bIsObjCollision)
			return;
		m_tInfo.vPos.x -= m_fSpeed * m_pTimeMgr->GetDelta();
	}
	if (m_pKeyMgr->KeyPressing(KEY_D) && m_tInfo.vPos.x <= 1775)
	{
		m_dwDir = RIGHT;
		if (m_dwCollDir == RIGHT&&m_bIsObjCollision)
			return;
		m_tInfo.vPos.x += m_fSpeed * m_pTimeMgr->GetDelta();
	}
	if (m_pKeyMgr->KeyPressing(KEY_S) && m_tInfo.vPos.y <= 1750)
	{
		m_dwDir = DOWN;
		if (m_dwCollDir == DOWN&&m_bIsObjCollision)
			return;
		m_tInfo.vPos.y += m_fSpeed * m_pTimeMgr->GetDelta();
	}
	if (m_pKeyMgr->KeyPressing(KEY_W) && m_tInfo.vPos.y >= 25)
	{
		m_dwDir = UP;
		if (m_dwCollDir == UP&&m_bIsObjCollision)
			return;
		m_tInfo.vPos.y-= m_fSpeed * m_pTimeMgr->GetDelta();
	}
	
	if (m_pKeyMgr->KeyPressing(KEY_LBUTTON))
	{
		if (!m_bIsAttack)
		{
			m_pChest->AttackAni(m_eWpType);
			m_pWeapon->AttackAni(m_eWpType);
			m_bIsAttack = true;
		}
	}
	Timer(m_bIsAttack, m_tData.fAtkSpeed,m_fTimer);
	
	if (m_pKeyMgr->KeyPressing(KEY_1))
		m_eWpType = WEAPONE_FIST;
	if (m_pKeyMgr->KeyPressing(KEY_2))
		m_eWpType = WEAPONE_JAVELIN;
	if (m_pKeyMgr->KeyPressing(KEY_3))
		m_eWpType = WEAPONE_JAVELIN1;
	if (m_pKeyMgr->KeyPressing(KEY_4))
		m_eWpType = WEAPONE_BOW;
	if (m_pKeyMgr->KeyPressing(KEY_5))
		m_eWpType = WEAPONE_BOW1;
	CScrollMgr::FollowCam(m_tInfo.vPos,m_fSpeed);
}

void CPlayer::FollowPath()
{
	//if (m_pAstarMgr->GetBestPath().empty())
	//	return;

	//m_tInfo.vDir = m_pAstarMgr->GetBestPath().front()->vPos - m_tInfo.vPos;

	float fLength = D3DXVec3Length(&m_tInfo.vDir);
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

	float fMoveSpeed = 300.f * m_pTimeMgr->GetDelta();
	m_tInfo.vPos += m_tInfo.vDir * fMoveSpeed;

	if (fLength <= fMoveSpeed) // 일정거리 도달하면
	{
#ifdef _DEBUG
		//m_pAstarMgr->GetBestPath().front()->byDrawID = 2;
#endif

		//m_pAstarMgr->GetBestPath().pop_front();
	}
}

float CPlayer::ZOrder(float _y)
{
	return 1 - (_y *0.0001f);
}

void CPlayer::ConvertPos(const OBJ_INFO & objInfo, const TEX_INFO & pTexInfo, D3DXVECTOR3 & vPos)
{
	D3DXVECTOR3 Temp;
	int iW = 0, iH = 0;
	if (objInfo.wstrObjectName.find(L"TP") != wstring::npos)
	{
		iW = pTexInfo.tImgInfo.Width*0.6f;
		iH = pTexInfo.tImgInfo.Height*0.25f;
		vPos.x = (((int)vPos.x / iW) *iW);
		vPos.y = (((int)vPos.y / iH) *iH);
	}
	else if (objInfo.wstrObjectName.find(L"Horizontal_Wall") != wstring::npos)
	{
		iW = pTexInfo.tImgInfo.Width*0.9f;
		iH = pTexInfo.tImgInfo.Height;
		vPos.x = (((int)vPos.x / iW) *iW);
		vPos.y = (((int)vPos.y / iH) *iH);
	}
	else
	{

	}
	
}

void CPlayer::ConvertCenter(const OBJ_INFO & objInfo, const TEX_INFO & pTexInfo, float & cx, float & cy)
{
	if (objInfo.wstrObjectName.find(L"Tree") != wstring::npos)
	{
		cx = pTexInfo.tImgInfo.Width*0.5f;
		cy = pTexInfo.tImgInfo.Height;
	}
	else if (objInfo.wstrObjectName.find(L"Lamp") != wstring::npos)
	{
		cx = pTexInfo.tImgInfo.Width*0.5f;
		cy = pTexInfo.tImgInfo.Height;
	}
	else
	{
		cx = pTexInfo.tImgInfo.Width*0.5f;
		cy = pTexInfo.tImgInfo.Height*0.5f;
	}
}

CPlayer* CPlayer::Create()
{
	CPlayer* pInstance = new CPlayer;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
