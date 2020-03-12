
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "Tool.h"
#include "MainFrm.h"
#include "MyForm.h"
#include "ToolView.h"
#include "MiniView.h"
#include "PreView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	//{
	//	TRACE0("도구 모음을 만들지 못했습니다.\n");
	//	return -1;      // 만들지 못했습니다.
	//}

	//if (!m_wndStatusBar.Create(this))
	//{
	//	TRACE0("상태 표시줄을 만들지 못했습니다.\n");
	//	return -1;      // 만들지 못했습니다.
	//}
	//m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	m_MainSplitter.CreateStatic(this, 1, 2);
	m_MainSplitter.CreateView(0, 1, RUNTIME_CLASS(CToolView), CSize(WINCX, 900), pContext);
	m_MainSplitter.SetColumnInfo(0,450, 450);
	//m_MainSplitter.SetRowInfo(0, 300, 300);

	
	m_SecondSplitter.CreateStatic(&m_MainSplitter, 2, 1, WS_CHILD | WS_VISIBLE, m_MainSplitter.IdFromRowCol(0, 0));
	m_SecondSplitter.CreateView(0, 0, RUNTIME_CLASS(CMiniView), CSize(300, 300), pContext);
	m_SecondSplitter.SetColumnInfo(0, 400, 400);
	m_SecondSplitter.CreateView(1, 0, RUNTIME_CLASS(CMyForm), CSize(300, 300), pContext);
	//m_SecondSplitter.CreateView(2, 0, RUNTIME_CLASS(CPreView), CSize(200, 300), pContext);
	//m_SecondSplitter.SetRowInfo(2, 400, 400);


	//TODO: 요기하는중!
	//m_ThirdSplitter.CreateStatic(&m_SecondSplitter, 2, 1, WS_CHILD | WS_VISIBLE, m_SecondSplitter.IdFromRowCol(1, 0));
	//m_ThirdSplitter.CreateView(0, 0, RUNTIME_CLASS(CMyForm), CSize(400, 400), pContext);
	//m_ThirdSplitter.CreateView(1, 0, RUNTIME_CLASS(CPreView), CSize(300, 300), pContext);
	//m_ThirdSplitter.SetRowInfo(0, 400, 400);
	//m_ThirdSplitter.SetRowInfo(1, 300, 300);


	//m_ThirdSplitter.CreateView(0,1,)
	return TRUE;
}
