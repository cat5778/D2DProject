// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "RespawnManager.h"
#include "MyForm.h"
// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM),
	m_byTileNum(0),
	m_Texname(L"TileSet1"),
	m_byTileOption(0),
	m_iOldCount(0)
	, m_byImageIDX(0),
	m_Obj(L"",L"")
{

}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_byTileNum);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Text(pDX, IDC_EDIT3, m_byTileOption);
	DDX_Control(pDX, IDC_Picture, m_Preview);
	DDX_Control(pDX, IDC_COMBO1, m_ComboBox);
	DDX_Control(pDX, IDC_EDIT2, m_edImageIDX);
	DDX_Control(pDX, IDC_EDIT1, m_edTimeNum);
	DDX_Control(pDX, IDC_EDIT3, m_edTileOption);
	DDX_Control(pDX, IDC_SPIN1, m_spTileNum);
	DDX_Control(pDX, IDC_SPIN2, m_spTileOption);
	DDX_Text(pDX, IDC_EDIT2, m_byImageIDX);
	DDX_Control(pDX, IDC_SPIN3, m_spImageIDX);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyForm::OnBnClickedTileButton)
	ON_BN_CLICKED(IDC_BUTTON3, &CMyForm::OnBnClickedObjectTool)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMyForm::OnLbnSelchangeList)
	ON_EN_CHANGE(IDC_EDIT1, &CMyForm::OnEnChangeTileNum)
	ON_EN_CHANGE(IDC_EDIT3, &CMyForm::OnEnChangeTileOption)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CMyForm::OnCbnSelchange)
	ON_BN_CLICKED(IDC_BUTTON4, &CMyForm::OnBnClickedObject)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CMyForm::OnDeltaposTimeNum)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CMyForm::OnDeltaposTileOption)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CMyForm::OnDeltaposImageIDX)
	ON_BN_CLICKED(IDC_BUTTON6, &CMyForm::OnBnClickedObjSave)
	ON_BN_CLICKED(IDC_BUTTON7, &CMyForm::OnBnClickedObjLoad)
END_MESSAGE_MAP()


// CMyForm 진단입니다.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm 메시지 처리기입니다.


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	CString csObjectType[10] = { L"Title",L"Tile",L"Building",L"Tree",L"Grass",L"Monster",L"NPC",L"Player",L"Obstacle",L"Choose" };
	for (int i = 0; i < 10; i++)
		m_ComboBox.AddString(csObjectType[i]);
	ReadData(L"../Data/PathInfo.txt");
	//CTextureMgr::GetInstance()->
	m_TileTool.Create(IDD_TILETOOL);
	m_ObjTool.Create(IDD_OBJECTTOOL);
	m_PathTool.Create(IDD_PATHTOOL);

	m_edTimeNum.SetWindowTextW(L"0");
	m_spTileNum.SetRange(0, 100);
	m_spTileNum.SetPos(0);

	m_edTileOption.SetWindowTextW(L"0");
	m_spTileOption.SetRange(0, 100);
	m_spTileOption.SetPos(0);

	m_edImageIDX.SetWindowTextW(L"0");
	m_spImageIDX.SetRange(0, 100);
	m_spImageIDX.SetPos(0);
}



void CMyForm::OnBnClickedTileButton()
{

	m_TileTool.ShowWindow(SW_SHOW);

}



void CMyForm::OnEnChangeTileNum()
{
	UpdateData(TRUE);
	
	//cout << (int)m_byTileNum << endl;

}

CString CMyForm::ConvertionEtoC(OBJECT_TYPE eObjectType)
{
	switch (eObjectType)
	{
	case OBJECT_TITLE:
		return L"Title";
	case OBJECT_BUILDING:
		return L"Building";
	case OBJECT_MONSTER:
		return L"Monster";
	case OBJECT_NPC:
		return L"NPC";
	case OBJECT_GRASS:
		return L"Grass";
	case OBJECT_TREE:
		return L"Tree";
	case OBJECT_PLAYER:
		return L"Player";
	default:
		return L"";
	}
}

HRESULT CMyForm::ReadData(wstring wstrFilePath)
{
	wifstream fin;

	fin.open(wstrFilePath);

	if (fin.fail())
		return E_FAIL;

	TCHAR szObjectKey[MAX_STR] = L"";
	TCHAR szStateKey[MAX_STR] = L"";
	TCHAR szCount[MAX_STR] = L"";
	TCHAR szRelative[MAX_STR] = L"";

	while (true)
	{
		fin.getline(szObjectKey, MAX_STR, '|');
		fin.getline(szStateKey, MAX_STR, '|');
		fin.getline(szCount, MAX_STR, '|');
		fin.getline(szRelative, MAX_STR);

		if (fin.eof())
			break;
		ConvertionCtoE(szObjectKey);
		switch (m_eObjType)
		{
		case OBJECT_TITLE:
			m_ObjectPath.insert(make_pair(szStateKey, m_eObjType));
			break;
		case OBJECT_OBSTACLE:
			m_ObjectPath.insert(make_pair(szStateKey, m_eObjType));
			break;
		case OBJECT_GRASS:
			m_ObjectPath.insert(make_pair(szStateKey, m_eObjType));
			break;
		case OBJECT_TERRAIN:
			m_mTilePath.insert(make_pair(szStateKey,szObjectKey));
			break;
		case OBJECT_CHOOSE:
			m_ObjectPath.insert(make_pair(szStateKey, m_eObjType));
			break;
		case OBJECT_MONSTER:
			m_ObjectPath.insert(make_pair(szStateKey, m_eObjType));
			break;
		default:
			break;
		}
	}

	fin.close();
	return E_NOTIMPL;
}

void CMyForm::PreviewTile(CString csTileName)
{
	m_TilePath = make_pair(m_mTilePath.find(csTileName)->first.GetString(), m_mTilePath.find(csTileName)->second.GetString());

	const TEX_INFO* pTexInfo = CTextureMgr::GetInstance()->GetTexInfo(m_TilePath.second.GetString(), m_TilePath.first.GetString());
	NULL_CHECK(pTexInfo);
	float fCenterX = 0;
	float fCenterY = 0;

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, (float)WINCX / pTexInfo->tImgInfo.Width, (float)WINCY / pTexInfo->tImgInfo.Height, 0.f);
	D3DXMatrixTranslation(&matTrans, 0, 0, 0.f);

	matWorld = matScale * matTrans;

	CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&matWorld);
	Invalidate(FALSE);


	CDeviceMgr::GetInstance()->Render_Begin();

	CDeviceMgr::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CDeviceMgr::GetInstance()->Render_End(m_Preview.m_hWnd);
}

void CMyForm::PreiviewObject(CString csItemName)
{
	m_CurObjItr = m_pObjList.begin();
	for (; m_CurObjItr != m_pObjList.end(); m_CurObjItr++)
	{
		if ((*m_CurObjItr)->wstrObjectName.compare(csItemName.GetString()) == 0)
			break;
	}

	m_Obj = make_pair((*m_CurObjItr)->wstrObjectKey,(*m_CurObjItr)->wstrStateKey);

	const TEX_INFO* pTexInfo = CTextureMgr::GetInstance()->GetTexInfo(m_Obj.first, m_Obj.second, m_byImageIDX);
	NULL_CHECK(pTexInfo);
	float fCenterX = 0;
	float fCenterY = 0;

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, (float)WINCX / pTexInfo->tImgInfo.Width, (float)WINCY / pTexInfo->tImgInfo.Height, 0.f);
	D3DXMatrixTranslation(&matTrans, 0, 0, 0.f);

	matWorld = matScale * matTrans;

	CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&matWorld);
	Invalidate(FALSE);


	CDeviceMgr::GetInstance()->Render_Begin();

	CDeviceMgr::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	CDeviceMgr::GetInstance()->Render_End(m_Preview.m_hWnd);

}


void CMyForm::OnEnChangeTileOption()
{
	UpdateData(TRUE);

}


void CMyForm::OnLbnSelchangeList()
{

	CString csItemName;
	m_ListBox.GetText(m_ListBox.GetCurSel(), csItemName);
	switch (m_eObjType)
	{
	case OBJECT_TERRAIN:
		PreviewTile(csItemName);
		break;
	default:
		PreiviewObject(csItemName);
		break;
	}


}


void CMyForm::OnBnClickedObjectTool()
{
	m_ObjTool.ShowWindow(SW_SHOW);


}


void CMyForm::OnCbnSelchange()
{
	m_ListBox.ResetContent();
	CString cs,csTemp;
	m_ComboBox.GetLBText(m_ComboBox.GetCurSel(), cs);
	ConvertionCtoE(cs);
	switch (m_eObjType)
	{
	case OBJECT_TITLE:
		for (auto pObj : m_pObjList)
		{
			if (pObj->wstrObjectName.compare(L"") == 0)
				continue;
			if (pObj->eObjectType == OBJECT_TITLE)
			{
				csTemp = pObj->wstrObjectName.c_str();
				m_ListBox.AddString(csTemp);
			}
		}
		break;
	case OBJECT_TERRAIN:
		for (auto path : m_mTilePath)
			m_ListBox.AddString(path.first);
		break;
	case OBJECT_OBSTACLE:
		for (auto pObj : m_pObjList)
		{
			if (pObj->wstrObjectName.compare(L"") == 0)
				continue;
			if (pObj->eObjectType == OBJECT_OBSTACLE)
			{
				csTemp = pObj->wstrObjectName.c_str();
				m_ListBox.AddString(csTemp);
			}
		}
		break;
	case OBJECT_TRAP:
		break;
	case OBJECT_BUILDING:
		break;
	case OBJECT_MONSTER:
		for (auto pObj : m_pObjList)
		{
			if (pObj->wstrObjectName.compare(L"") == 0)
				continue;
			if (pObj->eObjectType == OBJECT_MONSTER)
			{
				csTemp = pObj->wstrObjectName.c_str();
				m_ListBox.AddString(csTemp);
			}
		}
		break;
	case OBJECT_NPC:
		break;
	case OBJECT_GRASS:
		for (auto pObj : m_pObjList)
		{
			if (pObj->wstrObjectName.compare(L"") == 0)
				continue;
			if (pObj->eObjectType == OBJECT_GRASS)
			{
				csTemp = pObj->wstrObjectName.c_str();
				m_ListBox.AddString(csTemp);
			}
		}
		break;
	case OBJECT_TREE:
		break;
	case OBJECT_PLAYER:
		break;
	case OBJECT_EFFECT:
		break;
	case OBJECT_UI:
		break;
	case OBJECT_CHOOSE:
		for (auto pObj : m_pObjList)
		{
			if (pObj->wstrObjectName.compare(L"") == 0)
				continue;
			if (pObj->eObjectType == OBJECT_CHOOSE)
			{
				csTemp = pObj->wstrObjectName.c_str();
				m_ListBox.AddString(csTemp);
			}
		}
		break;
	case OBJECT_END:
		break;
	default:
		break;
	}
	//if (cs.Compare(L"Tile") == 0)
	//{
	//	for (auto path : m_mTilePath)
	//		m_ListBox.AddString(path.first);

	//}
	//for (auto object : m_ObjectPath)
	//{
	//	if (cs.Compare(ConvertionEtoC(object.second)) == 0)
	//	{
	//		m_ListBox.AddString(object.first);
	//		m_iOldCount++;
	//	}
	//	
	//}
	//m_iOldCount = 0;

}

void CMyForm::ConvertionCtoE(CString csobjType)
{
	if (csobjType.Compare(L"Building") == 0)
		m_eObjType = OBJECT_BUILDING;
	else if (csobjType.Compare(L"Title") == 0)
		m_eObjType = OBJECT_TITLE;
	else if (csobjType.Compare(L"Tree") == 0)
		m_eObjType = OBJECT_TREE;
	else if (csobjType.Compare(L"Grass") == 0)
		m_eObjType = OBJECT_GRASS;
	else if (csobjType.Compare(L"NPC") == 0)
		m_eObjType = OBJECT_NPC;
	else if (csobjType.Compare(L"Monster") == 0)
		m_eObjType = OBJECT_MONSTER;
	else if (csobjType.Compare(L"Player") == 0)
		m_eObjType = OBJECT_PLAYER;
	else if (csobjType.Compare(L"Trap") == 0)
		m_eObjType = OBJECT_TRAP;
	else if (csobjType.Compare(L"Obstacle") == 0)
		m_eObjType = OBJECT_OBSTACLE;
	else if (csobjType.Compare(L"Tile") == 0)
		m_eObjType = OBJECT_TERRAIN;
	else if (csobjType.Compare(L"Choose") == 0)
		m_eObjType = OBJECT_CHOOSE;

}


void CMyForm::OnBnClickedObject()
{
	m_PathTool.ShowWindow(SW_SHOW);

}


void CMyForm::OnDeltaposTimeNum(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_byTileNum = pNMUpDown->iPos + pNMUpDown->iDelta;

	CString sVal;
	sVal.Format(_T("%d\n"), m_byTileNum);
	m_edTimeNum.SetWindowTextW(sVal);

	*pResult = 0;
}


void CMyForm::OnDeltaposTileOption(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_byTileOption = pNMUpDown->iPos + pNMUpDown->iDelta;
	CString sVal;
	sVal.Format(_T("%d\n"), m_byTileOption);
	m_edTileOption.SetWindowTextW(sVal);

	*pResult = 0;
}


void CMyForm::OnDeltaposImageIDX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_byImageIDX = pNMUpDown->iPos + pNMUpDown->iDelta;
	CString sVal;
	sVal.Format(_T("%d\n"), m_byImageIDX);
	m_edImageIDX.SetWindowTextW(sVal);
	if (m_eObjType != OBJECT_TERRAIN)
	{
		m_ListBox.GetText(m_ListBox.GetCurSel(), m_csCurObjName);
		PreiviewObject(m_csCurObjName);
	}
	*pResult = 0;
}


void CMyForm::OnBnClickedObjSave()
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
	CRespawnManager::GetInstance()->FileSave(Dlg.GetPathName());
	}
	//CRespawnManager::GetInstance()->SaveData();
}


void CMyForm::OnBnClickedObjLoad()
{
	CFileDialog Dlg(TRUE, L".dat", L"제목 없음.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat|Text Files(*.txt)|*.txt||", this);

	TCHAR szCurrentDir[MAX_STR] = L"";

	::GetCurrentDirectory(MAX_STR, szCurrentDir);
	::PathRemoveFileSpec(szCurrentDir);
	::PathCombine(szCurrentDir, szCurrentDir, L"Data");

	Dlg.m_ofn.lpstrInitialDir = szCurrentDir;

	if (IDOK == Dlg.DoModal())
		CRespawnManager::GetInstance()->FileLoad(Dlg.GetPathName());
	//CRespawnManager::GetInstance()->LoadData();

}
