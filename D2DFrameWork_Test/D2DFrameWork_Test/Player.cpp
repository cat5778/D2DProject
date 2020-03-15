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
#include "Hud.h"
#include "Projectile.h"
#include "SkillProjectTile.h"
#include "Effect.h"
#include "ImageObject.h"
#include "StormProjectile.h"
float g_PlayerExe = 0;
GAME_DATA g_PlayerData;
SkillData g_PlayerSkillData;
CPlayer::CPlayer()
	: m_pKeyMgr(CKeyManager::GetInstance()), m_pHat(nullptr), m_pChest(nullptr),
	m_pLeg(nullptr), m_fSpeed(0.f), m_wstrChest(L"Idle"), m_pWeapon(nullptr), m_CollBox(nullptr), m_pHPBar(nullptr), m_bIsDash(false)
{

}

CPlayer::CPlayer(D3DXVECTOR3 vPos)
	: m_pKeyMgr(CKeyManager::GetInstance()), m_pHat(nullptr), m_pChest(nullptr),
	m_pLeg(nullptr), m_fSpeed(0.f), m_wstrChest(L"Idle"), m_pWeapon(nullptr),
	m_CollBox(nullptr), m_pHPBar(nullptr), m_pHud(nullptr), m_bIsDash(false), m_fDashTime(0), m_fDash(1), m_iMulLv(1)
{
	ZeroMemory(&m_tSkillData, sizeof(SKILL_DATA));
	m_bIsInvincible = false;
	m_fTimer = 0;
	m_bIsAttack = false;
	m_eType = OBJECT_PLAYER;
	m_bIsObjCollision = false;
	m_bIsCollsion = false;
	m_tInfo.vPos = vPos;
	m_eWpType = WEAPONE_BOW;
	srand((unsigned)time(NULL));
	Initialize();
 
}

CPlayer::CPlayer(D3DXVECTOR3 vPos, GAME_DATA gameData)
	: m_pKeyMgr(CKeyManager::GetInstance()), m_pHat(nullptr), m_pChest(nullptr),
	m_pLeg(nullptr), m_fSpeed(0.f), m_wstrChest(L"Idle"), m_pWeapon(nullptr),
	m_CollBox(nullptr), m_pHPBar(nullptr), m_pHud(nullptr), m_bIsDash(false), m_fDashTime(0), m_fDash(1), m_iMulLv(1)
{
	ZeroMemory(&m_tSkillData, sizeof(SKILL_DATA));
	m_bIsInvincible = false;
	m_fTimer = 0;
	m_bIsAttack = false;
	m_eType = OBJECT_PLAYER;
	m_bIsObjCollision = false;
	m_bIsCollsion = false;
	m_tInfo.vPos = vPos;
	m_eWpType = WEAPONE_BOW;
	srand((unsigned)time(NULL));

	D3DXMatrixIdentity(&m_tInfo.matWorld); // 다이렉트 항등행렬 함수
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };


	m_tData=g_PlayerData;
	m_tSkillData = g_PlayerSkillData;
	if (m_pHud == nullptr)
	{
		m_pHud = new CHud(m_tData);

		CObjectMgr::GetInstance()->AddObject(OBJECT_UI, m_pHud);
	}
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
		m_pChest = new CChest(*temp, m_tInfo.vPos, m_tData.fstdAtkspd);
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
	m_pHPBar = new CHPBar(OBJECT_PLAYER, m_tInfo.vPos, m_tData);
	m_pWeapon = new CWeapon(m_tInfo.vPos, m_tData.fstdAtkspd, m_tData.fDamage);

	m_vSize = { 22,40 };
	m_CollBox = new CColliderBox(m_tInfo.vPos, PLAYER_HITBOX_COLLISION, m_vSize);
	m_pColliderMgr->AddObject(PLAYER_HITBOX_COLLISION, m_CollBox);

	m_StepColl = new CColliderBox(m_tInfo.vPos, PLAYER_STEP_COLLISION, D3DXVECTOR2(5, 5));
	m_pColliderMgr->AddObject(PLAYER_STEP_COLLISION, m_StepColl);

}


CPlayer::~CPlayer()
{
	Release();
}

int CPlayer::Update()
{		

	MultiShot();
	LevelUp();
	Dash();
	KeyInput();
	if (m_pHat != nullptr)
		m_pHat->Update();
	if (m_pChest != nullptr)
	{
		m_pChest->SetAtkAniRate(m_tData.fAtkSpeed * m_tData.fstdAtkspd);
		m_pChest->Update();
	}
	if (m_pLeg != nullptr)
		m_pLeg->Update();
	if (m_pWeapon != nullptr)
	{
		m_pWeapon->SetAtkRate(m_tData.fAtkSpeed * m_tData.fstdAtkspd);
		m_pWeapon->Update();
	}
	if (m_CollBox != nullptr)
	{
		m_CollBox->SetvPos(m_tInfo.vPos);
		D3DXVECTOR3 vConvert = m_tInfo.vPos;
		vConvert.y = m_tInfo.vPos.y + 18;
		m_StepColl->SetvPos(vConvert);
		m_bIsCollsion = m_CollBox->IsCollsion();

		//m_bIsObjCollision = m_CollBox->IsOBJCollsion();
		m_bIsObjCollision = m_StepColl->IsOBJCollsion();
		if (m_bIsCollsion)
		{

			if (m_CollBox->GetHitColl() != COLLSION_END)
			{
				BeAttack(-m_CollBox->GetGameData().fCurHp);
				m_CollBox->SetHitColl(COLLSION_END);
				CEffect* temp = new CEffect(m_tInfo.vPos, L"Effect", L"Hit_Blood");
				CObjectMgr::GetInstance()->AddObject(OBJECT_EFFECT, temp);
				m_bIsInvincible = true;

			}
		}
		//if (m_bIsObjCollision)
		//	m_dwCollDir = m_CollBox->GetdwCollDir();
		//else
		//	m_dwCollDir = -1;

		if (m_bIsObjCollision)
			m_dwCollDir = m_StepColl->GetdwCollDir();
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

	D3DXMatrixIdentity(&m_tInfo.matWorld); // 다이렉트 항등행렬 함수
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };


	m_tData.fstdAtkspd = 1;
	m_tData.fAtkSpeed = 1;
	m_tData.fCurEXE = 0;
	m_tData.fExe = 100;
	m_tData.iLevel = 1;
	m_tData.fHp = 100;
	m_tData.fSpeed = 150;
	m_tData.fCurHp = m_tData.fHp;
	m_tData.fDamage = 5;
	m_tData.fOldHp = m_tData.fCurHp;



	if (m_pHud==nullptr)
	{
		m_pHud = new CHud(m_tData);

		CObjectMgr::GetInstance()->AddObject(OBJECT_UI, m_pHud);
	}
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
		m_pChest = new CChest(*temp, m_tInfo.vPos, m_tData.fstdAtkspd);
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
	m_pWeapon = new CWeapon(m_tInfo.vPos, m_tData.fstdAtkspd,m_tData.fDamage);

	m_vSize = {22,40};
	m_CollBox = new CColliderBox(m_tInfo.vPos, PLAYER_HITBOX_COLLISION, m_vSize);
	m_pColliderMgr->AddObject(PLAYER_HITBOX_COLLISION, m_CollBox);

	m_StepColl = new CColliderBox(m_tInfo.vPos, PLAYER_STEP_COLLISION, D3DXVECTOR2(10,10));
	m_pColliderMgr->AddObject(PLAYER_STEP_COLLISION, m_StepColl);
	//CScrollMgr::SetCamPos(m_tInfo.vPos);
	return S_OK;
}

void CPlayer::Release()
{
}

void CPlayer::KeyInput()
{
	if (m_pKeyMgr->KeyDown(KEY_T))
	{
		m_pHud->SetOnSkillTree();
		m_tData=m_pHud->GetData();
		g_PlayerData = m_tData;
		m_tSkillData = m_pHud->GetSkillData();
		g_PlayerSkillData = m_tSkillData;

	}
	if (m_pKeyMgr->KeyDown(KEY_O))
	{
		CEffect* temp = new CEffect(m_tInfo.vPos, L"Effect", L"Stomp",0.11f);
		CObjectMgr::GetInstance()->AddObject(OBJECT_EFFECT, temp);
		D3DXVECTOR3 convert = m_tInfo.vPos;
		convert.y -= 200;
		CEffect* temp2 = new CEffect(convert, L"Effect", L"Storm", 0.109f);
		CObjectMgr::GetInstance()->AddObject(OBJECT_EFFECT, temp2);
		convert.y += 200;
		CEffect* temp3 = new CEffect(convert, L"Effect", L"Level_Up", 0.999999f);
		CObjectMgr::GetInstance()->AddObject(OBJECT_EFFECT, temp3);
		m_tData.iLevel++;
		m_pHud->SetData(m_tData);
	}
	if (m_pKeyMgr->KeyPressing(KEY_A)&&m_tInfo.vPos.x>=25)
	{
		m_dwDir = LEFT;
		if (m_dwCollDir == LEFT&&m_bIsObjCollision)
			return;
		m_tInfo.vPos.x -= m_tData.fSpeed * m_pTimeMgr->GetDelta()*m_fDash;
	}
	if (m_pKeyMgr->KeyPressing(KEY_D) && m_tInfo.vPos.x <= 1775)
	{
		m_dwDir = RIGHT;
		if (m_dwCollDir == RIGHT&&m_bIsObjCollision)
			return;
		m_tInfo.vPos.x += m_tData.fSpeed * m_pTimeMgr->GetDelta()*m_fDash;
	}
	if (m_pKeyMgr->KeyPressing(KEY_S) && m_tInfo.vPos.y <= 1750)
	{
		m_dwDir = DOWN;
		if (m_dwCollDir == DOWN&&m_bIsObjCollision)
			return;
		m_tInfo.vPos.y += m_tData.fSpeed * m_pTimeMgr->GetDelta()*m_fDash;
	}
	if (m_pKeyMgr->KeyPressing(KEY_W) && m_tInfo.vPos.y >= 25)
	{
		m_dwDir = UP;
		if (m_dwCollDir == UP&&m_bIsObjCollision)
			return;
		m_tInfo.vPos.y-= m_tData.fSpeed * m_pTimeMgr->GetDelta()*m_fDash;
	}
	
	if (m_pKeyMgr->KeyPressing(KEY_LBUTTON))
	{
		
		if (!m_bIsAttack)
		{
			m_pWeapon->SetDamage(m_tData.fDamage);
			m_pChest->AttackAni(m_eWpType);
			m_pWeapon->AttackAni(m_eWpType);
			m_bIsAttack = true;


			

		}
	}
	Timer(m_bIsAttack, m_tData.fstdAtkspd/ m_tData.fAtkSpeed,m_fTimer);
	//2/

	//if (m_pKeyMgr->KeyPressing(KEY_1))
	//	m_eWpType = WEAPONE_FIST;
	//if (m_pKeyMgr->KeyPressing(KEY_2))
	//{
	//	m_eWpType = WEAPONE_JAVELIN;
	//	m_tData.fstdAtkspd = 1.f;
	//}
	//if (m_pKeyMgr->KeyPressing(KEY_3))
	//{
	//	m_eWpType = WEAPONE_JAVELIN1;
	//	m_tData.fstdAtkspd += 2.f;

	//}
	//if (m_pKeyMgr->KeyPressing(KEY_4))
	//{
	//	m_eWpType = WEAPONE_BOW;
	//	m_tData.fstdAtkspd +=3.f;
	//}
	//if (m_pKeyMgr->KeyPressing(KEY_5))
	//{
	//	m_eWpType = WEAPONE_BOW1;
	//	m_tData.fstdAtkspd += 2.f;
	//}
	
	CScrollMgr::FollowCam(m_tInfo.vPos, m_tData.fSpeed*m_fDash);
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

void CPlayer::Dash()
{
	
	if (m_pKeyMgr->KeyDown(KEY_1) && !m_bIsDash)
	{
	
		m_fDash = 5.0f;
		m_bIsDash = true;
	}
	Timer(m_bIsDash,0.25f,m_fDashTime);
	if (!m_bIsDash)
		m_fDash = 1.0f;
}

void CPlayer::MultiShot()
{
	if (m_pKeyMgr->KeyDown(KEY_2)&& m_tSkillData.iMulptLv)
	{
		float fangle = 0;
		float radian=0;
		int iRate = (m_tSkillData.iMulptLv * 10);
		int iAngle= 360/ iRate;
		for (int i = 0; i < iRate; i++)
		{
			fangle = iAngle*i;
			radian = fangle / 180 * PI;
			CSkillProjectTile* tempProject =
				new CSkillProjectTile(m_eWpType, m_tInfo.vPos, radian, m_tData.fDamage);
			CObjectMgr::GetInstance()->AddObject(OBJECT_PROJECTILE, tempProject);

		}
		
	}

}

void CPlayer::LevelUp()
{
	m_tData.fCurEXE += g_PlayerExe;
	g_PlayerExe = 0;
	if (m_tData.fExe <= m_tData.fCurEXE)
	{
		CEffect* temp = new CEffect(m_tInfo.vPos, L"Effect", L"Stomp", 0.11f);
		CObjectMgr::GetInstance()->AddObject(OBJECT_EFFECT, temp);
		D3DXVECTOR3 convert = m_tInfo.vPos;
		convert.y -= 200;
		CEffect* temp2 = new CEffect(convert, L"Effect", L"Storm", 0.109f);
		CObjectMgr::GetInstance()->AddObject(OBJECT_EFFECT, temp2);
		convert.y += 200;
		CEffect* temp3 = new CEffect(convert, L"Effect", L"Level_Up", 0.999999f);
		CObjectMgr::GetInstance()->AddObject(OBJECT_EFFECT, temp3);
		m_tData.iLevel++;
		m_tData.fCurEXE -= m_tData.fExe;
		m_pHud->SetData(m_tData);
		
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
