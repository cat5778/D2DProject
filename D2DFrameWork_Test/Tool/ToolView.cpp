
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif
#include "MainFrm.h"
#include "ToolDoc.h"
#include "ToolView.h"
#include "Terrain.h"
#include "MiniView.h"
#include "MyForm.h"
#include "RespawnManager.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolView

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CToolView 생성/소멸

HWND g_hWnd;


CToolView::CToolView()
	: m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pFormView(nullptr)
{

}

CToolView::~CToolView()
{
	CTerrain::GetInstance()->Release();
	CTerrain::GetInstance()->DestroyInstance();
	m_pTextureMgr->DestroyInstance();
	m_pDeviceMgr->DestroyInstance();
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{

	return CScrollView::PreCreateWindow(cs);
}

// CToolView 그리기

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_pDeviceMgr->Render_Begin();
	Render();
	CTerrain::GetInstance()->Render();
	CRespawnManager::GetInstance()->Render();
	m_pDeviceMgr->Render_End();
	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CMiniView* pMiniView = dynamic_cast<CMiniView*>(pFrameWnd->m_SecondSplitter.GetPane(0, 0));
	NULL_CHECK(pMiniView);
	


	
	pMiniView->Invalidate(FALSE); // 미니뷰 갱신
	
	



}


// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기


void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	g_hWnd = m_hWnd;

	// 스크롤 범위 지정
	int iCX = 100 * TILECX;
	int iCY = 100* TILECY;

	CScrollView::SetScrollSizes(MM_TEXT, CSize(iCX, iCY));

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK_MSG(pFrameWnd, L"pFrameWnd is null");

	RECT rcFrame = {};

	// 윈도우의 크기(테두리 + 클라이언트 영역) 얻어오는 함수.(스크린 좌표 기준)
	pFrameWnd->GetWindowRect(&rcFrame);
	::SetRect(&rcFrame, 0, 0, rcFrame.right - rcFrame.left, rcFrame.bottom - rcFrame.top);

	RECT rcView = {};

	// 윈도우의 클라이언트 영역(테두리를 포함하지 않은) 크기 얻어오는 함수. (클라이언트 좌표 기준)
	GetClientRect(&rcView);

	// 현재 프레임윈도우와 View 사이의 갭들을 구한다.
	int iGapX = rcFrame.right - rcView.right;
	int iGapY = rcFrame.bottom - rcView.bottom;

	// 프레임윈도우의 크기를 새로 설정.
	pFrameWnd->SetWindowPos(nullptr, 0, 0, WINCX + iGapX, WINCY + iGapY, SWP_NOZORDER | SWP_NOMOVE);



	HRESULT hr = m_pDeviceMgr->InitDevice(MODE_WIN);
	FAILED_CHECK_MSG(hr, L"InitDevice Failed");

	LPDIRECT3DDEVICE9 pGraphicDev = m_pDeviceMgr->GetDevice();
	NULL_CHECK_MSG(pGraphicDev, L"pGraphicDev is null");
	
	hr = m_pTextureMgr->LoadTextureFrom
	
	
	
	
	
	(pGraphicDev, L"../Data/PathInfo.txt");
	//FAILED_CHECK_MSG_RETURN(hr, L"LoadTextureFromPathInfo Failed", E_FAIL);


	hr = m_pTextureMgr->LoadTexture(pGraphicDev, TEXTURE_SINGLE,
		L"../Image/Tile/Tileset_Fields_bg_tileset.png", L"DefalutTile1");
	FAILED_CHECK_MSG(hr, L"Tile Texture 111Load Failed");


	CTerrain::GetInstance()->Initialize();
	CTerrain::GetInstance()->m_pView = this;
	CRespawnManager::GetInstance()->m_pView = this;
}

D3DXVECTOR3 CToolView::ConvertPos(D3DXVECTOR3 Pos)
{
	D3DXVECTOR3 tempPos;
	tempPos.x = ((int)Pos.x / TILECX)*TILECX;
	tempPos.y = ((int)Pos.y / TILECY)*TILECY;
	return tempPos;

}

void CToolView::Render()
{
	if (m_pFormView == nullptr)
	{
		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		NULL_CHECK(pFrameWnd);

		m_pFormView = dynamic_cast<CMyForm*>(pFrameWnd->m_SecondSplitter.GetPane(1, 0));
		NULL_CHECK(m_pFormView);
	}

	D3DXMATRIX matScale, matTrans;
	float fCenterX = 0.f, fCenterY = 0.f;
	const TEX_INFO* pTexInfo = nullptr;
	pTexInfo = CTextureMgr::GetInstance()->GetTexInfo(m_pFormView->m_Obj.first, m_pFormView->m_Obj.second, m_pFormView->m_byImageIDX);
	NULL_CHECK(pTexInfo);
	if(pTexInfo != nullptr)
	{
		CRespawnManager::GetInstance()->ConvertPos(*(*m_pFormView->m_CurObjItr), *pTexInfo, m_vPoint);

		D3DXMatrixScaling(&matScale,1, 1, 1);
		D3DXMatrixTranslation(&matTrans,
			m_vPoint.x - this->GetScrollPos(0),
			m_vPoint.y - this->GetScrollPos(1),
			m_vPoint.z = CRespawnManager::GetInstance()->ZOrder(m_vPoint.y));

		CRespawnManager::GetInstance()->ConvertCenter(*(*m_pFormView->m_CurObjItr), *pTexInfo, fCenterX, fCenterY);
		//fCenterX = pTexInfo->tImgInfo.Width *0.5f;
		//fCenterY = pTexInfo->tImgInfo.Height *0.5f;
		CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&(matScale * matTrans));
		CDeviceMgr::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
				&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(140, 255, 255, 255));
	}
	//&D3DXVECTOR3(fCenterX, fCenterY, 0.f)



}


void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	D3DXVECTOR3 vPoint =
	{
		(float)point.x + CScrollView::GetScrollPos(0),
		(float)point.y + CScrollView::GetScrollPos(1),
		0.f
	};

	
	switch (m_pFormView->m_eObjType)
	{
	case OBJECT_TERRAIN:
		CTerrain::GetInstance()->TileChange(vPoint, m_pFormView->m_byTileNum, m_pFormView->m_byTileOption, m_pFormView->m_TilePath.first.GetString());
		break;
	default:

		D3DXVECTOR3 vPos;
		vPos = vPoint;
		OBJ_INFO *objTemp = new OBJ_INFO;
		*objTemp = *(*m_pFormView->m_CurObjItr);
		objTemp->ImageIDX= m_pFormView->m_byImageIDX;
		if (CRespawnManager::GetInstance()->CheckObject(*objTemp, vPos))
			CRespawnManager::GetInstance()->m_mObjects.insert(make_pair(objTemp, vPos));
		else
		{
			delete objTemp;
			objTemp = nullptr;
		}
		break;
	}
	Invalidate(FALSE);

	CScrollView::OnLButtonDown(nFlags, point);
}


void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	const TEX_INFO* pTexInfo = nullptr;
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		D3DXVECTOR3 vPoint =
		{
			(float)point.x + CScrollView::GetScrollPos(0),
			(float)point.y + CScrollView::GetScrollPos(1),
			0.f
		};

	
		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		NULL_CHECK(pFrameWnd);

		CMyForm* m_pFormView = dynamic_cast<CMyForm*>(pFrameWnd->m_SecondSplitter.GetPane(1, 0));
		NULL_CHECK(m_pFormView);

		switch (m_pFormView->m_eObjType)
		{
		case OBJECT_TERRAIN:
			CTerrain::GetInstance()->TileChange(vPoint, m_pFormView->m_byTileNum, m_pFormView->m_byTileOption, m_pFormView->m_TilePath.first.GetString());
			break;
		default:
			D3DXVECTOR3 vPos;
			vPos = vPoint;
			OBJ_INFO *objTemp = new OBJ_INFO;
			*objTemp = *(*m_pFormView->m_CurObjItr);
			objTemp->ImageIDX = m_pFormView->m_byImageIDX;
			if (CRespawnManager::GetInstance()->CheckObject(*objTemp, vPos))
				CRespawnManager::GetInstance()->m_mObjects.insert(make_pair(objTemp, vPos));
			else
			{
				delete objTemp;
				objTemp = nullptr;
			}
			break;
		}
	}
	else
	{
		switch (m_pFormView->m_eObjType)
		{
		case OBJECT_TERRAIN:
			break;
		default:
				m_vPoint =
				{
					(float)point.x + CScrollView::GetScrollPos(0),
					(float)point.y + CScrollView::GetScrollPos(1),
					0.f
				};

			break;
		}

	}
		// 화면 갱신 (WM_PAINT 발생)
		Invalidate(FALSE);

	CScrollView::OnMouseMove(nFlags, point);
}


void CToolView::OnRButtonDown(UINT nFlags, CPoint point)
{

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		D3DXVECTOR3 vPoint =
		{
			(float)point.x + CScrollView::GetScrollPos(0),
			(float)point.y + CScrollView::GetScrollPos(1),
			0.f
		};
	
		switch (m_pFormView->m_eObjType)
		{
		case OBJECT_TERRAIN:
			break;

		default:
			m_vPoint =
			{
				(float)point.x + CScrollView::GetScrollPos(0),
				(float)point.y + CScrollView::GetScrollPos(1),
				0.f
			};
			CRespawnManager::GetInstance()->RemoveObj(m_vPoint);
			break;
		}
	}
	Invalidate(FALSE);
	//cout << "Object 갯수=" << CRespawnManager::GetInstance()->m_mObjects.size() << endl;
	CScrollView::OnRButtonDown(nFlags, point);

}

