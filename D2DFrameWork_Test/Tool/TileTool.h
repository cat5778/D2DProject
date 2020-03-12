#pragma once
#include "afxwin.h"


// TileTool 대화 상자입니다.

class TileTool : public CDialog
{
	DECLARE_DYNAMIC(TileTool)

public:
	TileTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~TileTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TILETOOL };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();

public:
	CListBox m_ListBox;
	map<CString, CString> m_mTilePath;
	LPDIRECT3DDEVICE9 m_pGraphicDev;
};
