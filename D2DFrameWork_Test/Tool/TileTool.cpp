// TileTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "TileTool.h"
#include "afxdialogex.h"
#include "DirectoryMgr.h"
#include "MainFrm.h"
#include "MyForm.h"
#include "ToolView.h"
#include "Terrain.h"
// TileTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(TileTool, CDialog)

TileTool::TileTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_TILETOOL, pParent)
{

}

TileTool::~TileTool()
{
}

void TileTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
}


BEGIN_MESSAGE_MAP(TileTool, CDialog)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDOK, &TileTool::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &TileTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON5, &TileTool::OnBnClickedLoad)
END_MESSAGE_MAP()


// TileTool 메시지 처리기입니다.


void TileTool::OnDropFiles(HDROP hDropInfo)
{

	TCHAR szFileName[MAX_STR] = L"";

	// 드래그 앤 드롭된 대상들 중 인덱스에 해당하는 파일의 이름을 얻어온다.
	//::DragQueryFile(hDropInfo, 1, szFileName, MAX_STR);
	//::AfxMessageBox(szFileName);

	// 두번째인자가 -1일 때 드래그 앤 드롭된 대상들의 전체 개수를 얻어온다.
	int iCount = ::DragQueryFile(hDropInfo, -1, nullptr, 0);

	CString strRelative = L"";
	CString strFileName = L"";

	for (int i = 0; i < iCount; ++i)
	{
		::DragQueryFile(hDropInfo, i, szFileName, MAX_STR);

		strRelative = CDirectoryMgr::ConvertRelativePath(szFileName);
		//cout << strRelative.GetString() << endl;
		strFileName = ::PathFindFileName(szFileName); // 파일명 얻어내는 함수.

		lstrcpy(szFileName, strFileName);
		::PathRemoveExtension(szFileName); // 확장자 제거.

		m_ListBox.AddString(strRelative);
		m_mTilePath.insert(make_pair(strFileName, strRelative));
	}

	//SettingHorizontalScrollSize();

	CDialog::OnDropFiles(hDropInfo);
}


void TileTool::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CMyForm* pFormView = dynamic_cast<CMyForm*>(pFrameWnd->m_SecondSplitter.GetPane(1, 0));
	NULL_CHECK(pFormView);

	//pFormView->m_mTilePath.insert(m_mTilePath.begin(), m_mTilePath.end());
	m_pGraphicDev = CDeviceMgr::GetInstance()->GetDevice();
	
	for (auto tilePath : m_mTilePath)
	{
		if(pFormView->m_mTilePath.find(tilePath.first) == pFormView->m_mTilePath.end())
			pFormView->m_ListBox.AddString(tilePath.first);
		pFormView->m_mTilePath.insert(tilePath);
		CTextureMgr::GetInstance()->LoadTexture(m_pGraphicDev, TEXTURE_SINGLE,
			tilePath.second.GetString(), tilePath.first.GetString());
	}

	CDialog::OnOK();
}


void TileTool::OnBnClickedSave()
{
	CFileDialog Dlg(FALSE, L".dat", L"제목 없음.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat|Text Files(*.txt)|*.txt||", this);

	TCHAR szCurrentDir[MAX_STR] = L"";

	::GetCurrentDirectory(MAX_STR, szCurrentDir);
	::PathRemoveFileSpec(szCurrentDir);
	::PathCombine(szCurrentDir, szCurrentDir, L"Data");

	Dlg.m_ofn.lpstrInitialDir = szCurrentDir;

	if (IDOK == Dlg.DoModal())
	{
		CTerrain::GetInstance()->SaveTile(Dlg.GetPathName());
	}
}


void TileTool::OnBnClickedLoad()
{
	CFileDialog Dlg(TRUE, L".dat", L"제목 없음.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat|Text Files(*.txt)|*.txt||", this);

	TCHAR szCurrentDir[MAX_STR] = L"";

	::GetCurrentDirectory(MAX_STR, szCurrentDir);
	::PathRemoveFileSpec(szCurrentDir);
	::PathCombine(szCurrentDir, szCurrentDir, L"Data");

	Dlg.m_ofn.lpstrInitialDir = szCurrentDir;

	if (IDOK == Dlg.DoModal())
		CTerrain::GetInstance()->LoadTile(Dlg.GetPathName());
}
