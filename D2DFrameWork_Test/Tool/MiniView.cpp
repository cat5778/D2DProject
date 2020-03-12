// MiniView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MiniView.h"
#include "MainFrm.h"
#include "Terrain.h"
#include "Tool.h"

// CMiniView

IMPLEMENT_DYNCREATE(CMiniView, CView)

CMiniView::CMiniView()
	:m_pDeviceMgr(CDeviceMgr::GetInstance())
{

}

CMiniView::~CMiniView()
{
}

BEGIN_MESSAGE_MAP(CMiniView, CView)
END_MESSAGE_MAP()


// CMiniView 그리기입니다.

void CMiniView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	m_pDeviceMgr->Render_Begin();

	CTerrain::GetInstance()->MiniView();

	m_pDeviceMgr->Render_End(m_hWnd);
	 
}


// CMiniView 진단입니다.

#ifdef _DEBUG
void CMiniView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMiniView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMiniView 메시지 처리기입니다.


void CMiniView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	/*CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);
*/
	/*CToolView* pView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK(pView);
*/
	//m_pTerrain = pView->m_pTerrain;
	//NULL_CHECK(m_pTerrain);


}
