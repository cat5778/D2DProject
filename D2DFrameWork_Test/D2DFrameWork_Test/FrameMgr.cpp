#include "stdafx.h"
#include "FrameMgr.h"

IMPLEMENT_SINGLETON(CFrameMgr)

CFrameMgr::CFrameMgr()
	: m_fTimeCount1(0.f), m_fTimeCount2(0.f),
	m_iFpsCount(0), m_szFPS(L""),
	m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTimeMgr(CTimeMgr::GetInstance())
{
	QueryPerformanceCounter(&m_OldTimeOne);
	QueryPerformanceCounter(&m_OldTimeTwo);
	QueryPerformanceCounter(&m_CurTime);

	QueryPerformanceFrequency(&m_CPUTick);
}


CFrameMgr::~CFrameMgr()
{
}

bool CFrameMgr::LimitFrame(float fFps)
{
	QueryPerformanceCounter(&m_CurTime);

	// ���� �ð��� ���� ������ CPU ������ ����
	if (m_CurTime.QuadPart - m_OldTimeTwo.QuadPart > m_CPUTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CPUTick);
		m_OldTimeTwo = m_CurTime;
	}

	m_fTimeCount1 += float(m_CurTime.QuadPart - m_OldTimeOne.QuadPart) / m_CPUTick.QuadPart;
	m_OldTimeOne = m_CurTime;

	if (1.f / fFps <= m_fTimeCount1)
	{
		m_fTimeCount1 = 0.f;
		return true;
	}

	return false;
}

void CFrameMgr::RenderFPS()
{
	// FPS ���
	++m_iFpsCount;

	m_fTimeCount2 += m_pTimeMgr->GetDelta();

	if (1.f <= m_fTimeCount2) // 1�ʰ� ������.
	{
		swprintf_s(m_szFPS, L"FPS: %d", m_iFpsCount);

		m_fTimeCount2 = 0.f;
		m_iFpsCount = 0;
	}

	// ���̷�Ʈ ��Ʈ ���
	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, 50.f, 50.f, 0.f);

	m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);

	m_pDeviceMgr->GetFont()->DrawText(
		m_pDeviceMgr->GetSprite(), /* ��������Ʈ COM ��ü */
		m_szFPS, /* ����� ���ڿ� */
		lstrlen(m_szFPS), /* ���ڿ� ���� */
		nullptr, /* �簢�� ���� */
		0,
		D3DCOLOR_ARGB(255, 0, 255, 0));
}