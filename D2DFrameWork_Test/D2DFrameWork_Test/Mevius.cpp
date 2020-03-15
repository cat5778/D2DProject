#include "stdafx.h"
#include "Mevius.h"
#include "GameObject.h"
#include "ColliderBox.h"
#include "ScrollMgr.h"
#include "HPBar.h"
#include "Effect.h"
#include "Projectile.h"
#include "SkillProjectTile.h"
#include "MeviusProjectile.h"
#include "ImageObject.h"
#include "StormProjectile.h"
CMevius::CMevius()
{
}

CMevius::CMevius( D3DXVECTOR3 vPos)
	: m_CollBox(nullptr), m_fAngle{0,90,180,270}
{
	m_tInfo.vPos = vPos;
	m_ApperPos = m_tInfo.vPos;
	m_ApperPos.y += 200;
	m_ApperPos = m_ApperPos - m_tInfo.vPos;
	D3DXVec3Normalize(&m_ApperPos, &m_ApperPos);
	m_fAtkRate = 0;
	m_bIsAttack = false;
	m_bIsCast = false;
	Initialize();
}


CMevius::~CMevius()
{
}

void CMevius::Appear()
{
	
	m_tInfo.vPos += m_ApperPos*200* m_pTimeMgr->GetDelta();
	

}

void CMevius::Idle()
{
	m_tObjInfo.wstrStateKey = L"Mevius";
	
	Animate();
	if (m_bIsAttack)
	{
		for (int i = 0; i < 4; i++)
		{
			m_fAngle[i] += 10;
			m_fRadian = m_fAngle[i] / 180 * PI;
			CMeviusProjectile* tempProject = new CMeviusProjectile(m_tInfo.vPos, m_fRadian, m_tData.fDamage);
			CObjectMgr::GetInstance()->AddObject(OBJECT_PROJECTILE, tempProject);
		}
		m_bIsAttack = false;
	}
	m_fAtkRate += m_pTimeMgr->GetDelta();
	if (0.25f <= m_fAtkRate)
	{
		m_bIsAttack = true;
		m_fAtkRate = 0.f;
	}



}

void CMevius::Attack()
{

	m_tObjInfo.wstrStateKey = L"Mevius_Cast";
	 Animate(true, 0.5f);

	m_fTimer += m_pTimeMgr->GetDelta();
	if (m_fTimer >= 2.0f)
	{
		m_vTargetPos = (*CObjectMgr::GetInstance()->GetObjList(OBJECT_PLAYER).begin())->GetTagInfo().vPos;
		m_vDir = m_vTargetPos - m_tInfo.vPos;
		CProjectile* temp = new CProjectile(WEAPONE_END, m_tInfo.vPos, m_vDir,
			m_tData.fDamage, MONSTER_PROJECTILE_COLLISION, L"Effect", L"Black_Hole");


		CObjectMgr::GetInstance()->AddObject(OBJECT_PROJECTILE, temp);
		m_fTimer = 0.f;
	}
	if (m_bIsAttack)
	{
		for (int i = 0; i < 4; i++)
		{
			m_fAngle[i] += 10;
			m_fRadian = m_fAngle[i] / 180 * PI;
			CMeviusProjectile* tempProject = new CMeviusProjectile(m_tInfo.vPos, m_fRadian, m_tData.fDamage);
			CObjectMgr::GetInstance()->AddObject(OBJECT_PROJECTILE, tempProject);
			if (m_tData.fCurHp <= 70)
			{
				D3DXVECTOR3 tempPos = m_tInfo.vPos;
				tempPos.x -= WINCX*0.5f;
				tempPos.y -= WINCY*0.5f;
				tempPos.x += rand() % WINCX;
				tempPos.y += rand() % WINCY;

				D3DXVECTOR3 convert = tempPos;
				convert.x -= 10;

				CEffect* pStomp = new CEffect(convert, L"Effect", L"Stomp");
				CObjectMgr::GetInstance()->AddObject(OBJECT_EFFECT, pStomp);


				CStormProjectile* pStorm = new CStormProjectile(tempPos,
					m_tData.fDamage, 1.0f, MONSTER_PROJECTILE_COLLISION, L"Effect"
					, L"Storm");
				CObjectMgr::GetInstance()->AddObject(OBJECT_PROJECTILE, pStorm);
			}
		}
		m_bIsAttack = false;

	}
	m_fAtkRate += m_pTimeMgr->GetDelta();
	if (0.25f <= m_fAtkRate)
	{
		m_bIsAttack = true;
		m_fAtkRate = 0.f;
	}
	


}

void CMevius::Move()
{
	if (m_bIsApreDist)
	{
		Appear();
		Timer(m_bIsApreDist, 2, m_fApreDist);
	}
	else
		MeviState(STATE_IDLE);




}


bool CMevius::AttackRange()
{
	return false;
}

void CMevius::BeAttack(float fDamage)
{
	m_tData.fCurHp -= fDamage;
	if (m_tData.fCurHp < 90)
		MeviState(STATE_ATTACK);
	else
		MeviState(STATE_IDLE);
}

int CMevius::Dead()
{
	if (!m_bIsDead)
	{
		wstring temp = L"_Dies";
		m_tObjInfo.wstrStateKey += temp;


		m_bIsDead = true;
	}//cout << "Fungus" << endl;
	return NO_EVENT;
}

void CMevius::MeviState(OBJECT_STATE eState)
{
	if (m_eOldState != eState)
	{
		m_eOldState = eState;
		switch (m_eOldState)
		{
		case STATE_IDLE:
			m_eCurState = STATE_IDLE;
			break;
		case STATE_ATTACK:
			m_eCurState = STATE_ATTACK;
			break;
		case STATE_MOVE:
			m_eCurState = STATE_MOVE;
			break;
		case STATE_BEATTACK:
			m_eCurState = STATE_BEATTACK;
			break;
		case STATE_DEAD:
			m_eCurState = STATE_DEAD;
		break;
		case STATE_END:
			m_eCurState = STATE_END;
			break;
		default:
			break;
		}
		m_tFrame.fCurFrame = 0;

	}
}

void CMevius::StateMachine()
{
	switch (m_eCurState)
	{
	case STATE_IDLE:
		Idle();
		break;
	case STATE_ATTACK:
		Attack();
		break;
	case STATE_MOVE:
		Move();
		break;
	case STATE_BEATTACK:
		break;
	case STATE_DEAD:
		Dead();
		break;
	case STATE_END:
		break;
	default:
		break;
	}
}

HRESULT CMevius::Initialize()
{
	m_tObjInfo.wstrObjectName = L"Mevius";
	m_tObjInfo.wstrStateKey = L"Mevius_Levitating";
	m_tObjInfo.wstrObjectKey = L"Mevius";
	m_tObjInfo.ImageIDX = 0;
	m_tObjInfo.IsAni = true;
	m_tObjInfo.eObjectType = OBJECT_MONSTER;
	m_fApreDist = 700;
	m_eMonType = BOSS1;
	m_fDetectDistance = 400;
	m_fAtkRange = 200;
	m_eType = OBJECT_MONSTER;
	m_fSpeed = 100.f;
	m_tData.fCurEXE = 10;
	m_tData.fHp = 100;
	m_tData.fCurHp = m_tData.fHp;
	m_tData.fDamage = 30;
	m_tData.fOldHp = m_tData.fCurHp;
	m_wsIdleState = m_tObjInfo.wstrStateKey;
	m_bIsCollsion = false;
	m_bIsObjCollision = false;
	m_bIsAttack = false;
	D3DXMatrixIdentity(&m_tInfo.matWorld); // 다이렉트 항등행렬 함수
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_fTimer = 0.f;
	m_bIsDead = false;
	m_bIsInvincible = false;
	m_fKnockTime = 0;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 10.f;
	m_pHPBar = new CHPBar(OBJECT_PLAYER, m_tInfo.vPos, m_tData);
	ChangeState(STATE_IDLE);
	m_wsState = L"";
	m_fApreDist = 0;
	m_bIsApreDist = true;

	m_eCurState = STATE_MOVE;
	m_eOldState = m_eCurState;




	return S_OK;
}

void CMevius::Release()
{
	g_PlayerExe += m_tData.fCurEXE;
	m_CollBox->EraseCollider();
}

int CMevius::Update()
{

	StateMachine();

	cout << m_eCurState<< endl;
	if (m_CollBox != nullptr)
	{
		m_CollBox->SetDamage(m_tData.fDamage);
		m_CollBox->SetvPos(m_tInfo.vPos);

		if (m_CollBox->IsCollsion()
			&& m_CollBox->GetHitColl() == PLAYER_PROJECTILE_COLLISION)
		{
			BeAttack(-m_CollBox->GetGameData().fCurHp);
			m_CollBox->InitCurHP();
			m_CollBox->SetHitColl(COLLSION_END);
			CEffect* temp = new CEffect(m_tInfo.vPos, L"Effect", L"Hit");
			CObjectMgr::GetInstance()->AddObject(OBJECT_EFFECT, temp);
			m_bIsInvincible = true;


		}

		if (m_bIsInvincible)
			KnockBack(m_CollBox->GetvKnock(), 200.f);

		Timer(m_bIsInvincible, 0.1f, m_fKnockTime);
	}


	return NO_EVENT;
}

void CMevius::LateUpdate()
{
	D3DXMATRIX matScale, matTrans;

	D3DXMatrixScaling(&matScale,
		m_tInfo.vSize.x,
		m_tInfo.vSize.y,
		0.f); // 다이렉트 크기행렬 함수
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x,
		m_tInfo.vPos.y - CScrollMgr::GetScrollPos().y,
		ZOrder(m_tInfo.vPos.y)); // 다이렉트 이동행렬 함수

	m_tInfo.matWorld = matScale * matTrans;

	D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vLook, &m_tInfo.matWorld);
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
}

void CMevius::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_tObjInfo.wstrObjectKey,
		m_tObjInfo.wstrStateKey, (int)m_tFrame.fCurFrame);
	m_tFrame.fMaxFrame = pTexInfo->iMaxCnt;

	NULL_CHECK(pTexInfo);

	m_vSize.x = pTexInfo->tImgInfo.Width * 0.5f;
	m_vSize.y = pTexInfo->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(m_vSize.x, m_vSize.y, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	if (m_CollBox == nullptr)
	{
		m_CollBox = new CColliderBox(m_tInfo.vPos, MONSTER_HITBOX_COLLISION, D3DXVECTOR2(50,100));
		m_pColliderMgr->AddObject(MONSTER_HITBOX_COLLISION, m_CollBox);
	}
	if (m_pHPBar != nullptr)
	{
		m_pHPBar->SetData(m_tInfo.vPos, m_tData, 25);
		m_pHPBar->Render();
	}
}
