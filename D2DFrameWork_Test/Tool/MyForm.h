#pragma once

#include "TileTool.h"
#include "ObjectTool.h"
#include "PathTool.h"
#include "afxwin.h"
#include "afxcmn.h"
// CMyForm 폼 뷰입니다.
class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMyForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual void OnInitialUpdate();
	//afx_msg
	afx_msg void OnBnClickedTileButton();
	afx_msg void OnBnClickedObjectTool();
	afx_msg void OnLbnSelchangeList();
	afx_msg void OnEnChangeTileOption();
	afx_msg void OnEnChangeTileNum();
	afx_msg void OnCbnSelchange();
	afx_msg void OnBnClickedObject();
	afx_msg void OnDeltaposTimeNum(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposTileOption(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposImageIDX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedObjSave();
	afx_msg void OnBnClickedObjLoad();
private:
	void ConvertionCtoE(CString csobjType);
	CString ConvertionEtoC(OBJECT_TYPE eObjectType);
	HRESULT ReadData(wstring wstrFilePath);
	void PreviewTile(CString csTileName);
	void PreiviewObject(CString csItemName);

public: // Tool Class
	TileTool		m_TileTool;
	ObjectTool		m_ObjTool;
	CPathTool		m_PathTool;

public:
	CDeviceMgr*		m_pDeviceMgr;
	CTextureMgr*	m_pTextureMgr;
public:
	CSpinButtonCtrl m_spTileNum;
	CSpinButtonCtrl m_spTileOption;
	CSpinButtonCtrl m_spImageIDX;
	CEdit			m_edTimeNum;
	CEdit			m_edTileOption;
	CEdit			m_edImageIDX;
	CListBox		m_ListBox;
	CStatic			m_Preview;
	CComboBox		m_ComboBox;
	int				m_iOldCount;

public:
	map<CString, OBJECT_TYPE>	m_ObjectPath;
	map<CString, CString>		m_mTilePath;
	pair<CString, CString>		m_TilePath;
	wstring						m_Texname;
	BYTE						m_byTileNum;
	BYTE						m_byTileOption;
	OBJECT_TYPE					m_eObjType;
	BYTE						m_byImageIDX;

	list<OBJ_INFO*>				m_pObjList;
	list<OBJ_INFO*>::iterator	m_CurObjItr;
	pair<wstring, wstring>		m_Obj;
	CString						m_csCurObjName;
	
};


