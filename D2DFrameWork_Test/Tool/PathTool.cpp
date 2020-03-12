// PathTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "PathTool.h"
#include "afxdialogex.h"

#include "DirectoryMgr.h"
// CPathTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPathTool, CDialog)

CPathTool::CPathTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PATHTOOL, pParent)
{

}

CPathTool::~CPathTool()
{
	for_each(m_PathInfoList.begin(), m_PathInfoList.end(), SafeDelete<PATH_INFO*>);
	m_PathInfoList.clear();

}

void CPathTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
}

void CPathTool::SettingHorizontalScrollSize()
{
	CString strName = L"";

	int iSrcCX = 0, iDstCX = 0;

	CDC* pDC = m_ListBox.GetDC();

	for (int i = 0; i < m_ListBox.GetCount(); ++i)
	{
		m_ListBox.GetText(i, strName);

		// 현재 문자열의 길이를 픽셀 단위로 환산.
		iSrcCX = pDC->GetTextExtent(strName).cx;

		if (iDstCX < iSrcCX)
			iDstCX = iSrcCX;
	}

	m_ListBox.ReleaseDC(pDC);

	// 현재 리스트박스가 가지고 있는 수평 스크롤 범위보다 클 때
	if (iDstCX > m_ListBox.GetHorizontalExtent())
		m_ListBox.SetHorizontalExtent(iDstCX); // 수평 스크롤 크기 갱신.
}


BEGIN_MESSAGE_MAP(CPathTool, CDialog)
	ON_BN_CLICKED(IDOK, &CPathTool::OnBnClickedSave)
	ON_BN_CLICKED(IDCANCEL, &CPathTool::OnBnClickedLoad)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CPathTool 메시지 처리기입니다.
void CPathTool::OnDropFiles(HDROP hDropInfo)
{

	int iCount = ::DragQueryFile(hDropInfo, -1, nullptr, 0);

	TCHAR szFileName[MAX_STR] = L"";

	for (int i = 0; i < iCount; ++i)
	{
		::DragQueryFile(hDropInfo, i, szFileName, MAX_STR);
		CDirectoryMgr::ExtractPathInfo(szFileName, m_PathInfoList);
	}

	wstring wstrCombined = L"";
	TCHAR szCount[MIN_STR] = L"";

	for (auto pPathInfo : m_PathInfoList)
	{
		// 정수 -> 문자
		_itow_s(pPathInfo->iImgCount, szCount, 10);

		wstrCombined = pPathInfo->wstrObjectKey + L"|" + pPathInfo->wstrStateKey + L"|"
			+ szCount + L"|" + pPathInfo->wstrRelative;

		m_ListBox.AddString(wstrCombined.c_str());
	}

	SettingHorizontalScrollSize();
	CDialog::OnDropFiles(hDropInfo);
}


void CPathTool::OnBnClickedSave()
{

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(FALSE, L".txt", L"제목 없음.txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt|Data Files(*.dat)|*.dat||", this);

	TCHAR szCurrentDir[MAX_STR] = L"";

	::GetCurrentDirectory(MAX_STR, szCurrentDir);
	::PathRemoveFileSpec(szCurrentDir);
	::PathCombine(szCurrentDir, szCurrentDir, L"Data");

	Dlg.m_ofn.lpstrInitialDir = szCurrentDir;

	if (IDOK == Dlg.DoModal())
	{
		wofstream fout;

		fout.open(Dlg.GetPathName());

		if (fout.fail())
			return;

		wstring wstrCombined = L"";
		TCHAR szCount[MIN_STR] = L"";

		for (auto pPathInfo : m_PathInfoList)
		{
			// 정수 -> 문자
			_itow_s(pPathInfo->iImgCount, szCount, 10);

			wstrCombined = pPathInfo->wstrObjectKey + L"|" + pPathInfo->wstrStateKey + L"|"
				+ szCount + L"|" + pPathInfo->wstrRelative;

			fout << wstrCombined << endl;
		}

		fout.close();

		// 윈도우 기본 응용 프로그램 실행하는 시스템 함수.
		WinExec("notepad.exe ../Data/PathInfo.txt", SW_SHOW);
	}
	CDialog::OnOK();
}


void CPathTool::OnBnClickedLoad()
{

	CFileDialog Dlg(TRUE, L".txt", L"제목 없음.txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt|Data Files(*.dat)|*.dat||", this);

	TCHAR szCurrentDir[MAX_STR] = L"";

	::GetCurrentDirectory(MAX_STR, szCurrentDir);
	::PathRemoveFileSpec(szCurrentDir);
	::PathCombine(szCurrentDir, szCurrentDir, L"Data");

	Dlg.m_ofn.lpstrInitialDir = szCurrentDir;

	if (IDOK == Dlg.DoModal())
	{
		wifstream fin;

		fin.open(Dlg.GetPathName());

		if (fin.fail())
			return;

		if (!m_PathInfoList.empty())
		{
			for_each(m_PathInfoList.begin(), m_PathInfoList.end(), SafeDelete<PATH_INFO*>);
			m_PathInfoList.clear();

			m_ListBox.ResetContent();
		}

		TCHAR szObjectKey[MAX_STR] = L"";
		TCHAR szStateKey[MAX_STR] = L"";
		TCHAR szCount[MAX_STR] = L"";
		TCHAR szRelative[MAX_STR] = L"";

		PATH_INFO* pPathInfo = nullptr;

		while (true)
		{
			fin.getline(szObjectKey, MAX_STR, '|');
			fin.getline(szStateKey, MAX_STR, '|');
			fin.getline(szCount, MAX_STR, '|');
			fin.getline(szRelative, MAX_STR);

			if (fin.eof())
				break;

			pPathInfo = new PATH_INFO;
			pPathInfo->wstrObjectKey = szObjectKey;
			pPathInfo->wstrStateKey = szStateKey;
			pPathInfo->iImgCount = _ttoi(szCount);
			pPathInfo->wstrRelative = szRelative;

			m_PathInfoList.push_back(pPathInfo);
		}

		fin.close();

		wstring wstrCombined = L"";

		for (auto pPathInfo : m_PathInfoList)
		{
			// 정수 -> 문자
			_itow_s(pPathInfo->iImgCount, szCount, 10);

			wstrCombined = pPathInfo->wstrObjectKey + L"|" + pPathInfo->wstrStateKey + L"|"
				+ szCount + L"|" + pPathInfo->wstrRelative;

			m_ListBox.AddString(wstrCombined.c_str());
		}

		SettingHorizontalScrollSize();
	}
	CDialog::OnCancel();
}


