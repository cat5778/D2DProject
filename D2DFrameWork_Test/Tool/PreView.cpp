// PreView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "PreView.h"


// CPreView

IMPLEMENT_DYNCREATE(CPreView, CView)

CPreView::CPreView()
	:m_pDeviceMgr(CDeviceMgr::GetInstance())
{

}

CPreView::~CPreView()
{
}

BEGIN_MESSAGE_MAP(CPreView, CView)
END_MESSAGE_MAP()


// CPreView 그리기입니다.

void CPreView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
	m_pDeviceMgr->Render_Begin();


	m_pDeviceMgr->Render_End(m_hWnd);


}


// CPreView 진단입니다.

#ifdef _DEBUG
void CPreView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CPreView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPreView 메시지 처리기입니다.


void CPreView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.


}
