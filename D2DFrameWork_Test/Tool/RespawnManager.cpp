#include "stdafx.h"
#include "RespawnManager.h"
#include "ToolView.h"

IMPLEMENT_SINGLETON(CRespawnManager)
CRespawnManager::CRespawnManager()
{
	//
}


CRespawnManager::~CRespawnManager()
{
	auto itr = m_mObjects.begin();
	for (; itr != m_mObjects.end();)
	{
		if ((*itr).first != nullptr)
			delete (*itr).first;
		//itr->first = nullptr;
		itr = m_mObjects.erase(itr);
	}
}

bool CRespawnManager::CheckObject(const OBJ_INFO& objInfo ,D3DXVECTOR3 &vPos)
{
	const TEX_INFO* pTexInfo;
	bool bIsReady=true;
	if (objInfo.IsAni)
		pTexInfo = CTextureMgr::GetInstance()->GetTexInfo(objInfo.wstrObjectKey, objInfo.wstrStateKey);
	else
		pTexInfo = CTextureMgr::GetInstance()->GetTexInfo(objInfo.wstrObjectKey, objInfo.wstrStateKey, objInfo.ImageIDX);
	ConvertPos(objInfo, *pTexInfo, vPos);


	for_each(m_mObjects.begin(), m_mObjects.end(), [&bIsReady,vPos](pair<OBJ_INFO*, D3DXVECTOR3> fobj)
	{
		if (fobj.second == vPos)
		{
			bIsReady = false;
			return ;
		}
	});
	return bIsReady;
}

void CRespawnManager::Render()
{
	D3DXMATRIX matScale, matTrans;

	const TEX_INFO* pTexInfo = nullptr;
	float fCenterX = 0.f, fCenterY = 0.f;

	TCHAR szIndexNum[MIN_STR] = L"";
	for (auto Obj: m_mObjects)
	{

		if(Obj.first->IsAni)
			pTexInfo = CTextureMgr::GetInstance()->GetTexInfo(Obj.first->wstrObjectKey, Obj.first->wstrStateKey);
		else
			pTexInfo = CTextureMgr::GetInstance()->GetTexInfo(Obj.first->wstrObjectKey, Obj.first->wstrStateKey, Obj.first->ImageIDX);
		NULL_CHECK_CONTINUE(pTexInfo);
		ConvertPos(*Obj.first, *pTexInfo, Obj.second);
	
		D3DXMatrixScaling(&matScale,
			1, 1, 1);
		D3DXMatrixTranslation(&matTrans,
			Obj.second.x - m_pView->GetScrollPos(0),
			Obj.second.y - m_pView->GetScrollPos(1),
			ZOrder(Obj.second.y));
		ConvertCenter(*Obj.first, *pTexInfo, fCenterX, fCenterY);
		//fCenterX = pTexInfo->tImgInfo.Width *0.5f;
		//fCenterY = pTexInfo->tImgInfo.Height;
		CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&(matScale * matTrans));
		CDeviceMgr::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	}
}

float  CRespawnManager::ZOrder(float _y)
{
	return 1-(_y *0.0001f);
}

void CRespawnManager::ConvertPos(const OBJ_INFO& objInfo,const TEX_INFO& pTexInfo,D3DXVECTOR3 &vPos)
{
	D3DXVECTOR3 Temp;
	int iW=0,iH=0;
	if (objInfo.wstrObjectName.find(L"TP") != wstring::npos)
	{
		iW = pTexInfo.tImgInfo.Width*0.6f;
		iH = pTexInfo.tImgInfo.Height*0.25f;
		vPos.x = (((int)vPos.x / iW) *iW);
		vPos.y = (((int)vPos.y / iH) *iH);
	}
	else if (objInfo.wstrObjectName.find(L"Horizontal_Wall") != wstring::npos)
	{
		iW = pTexInfo.tImgInfo.Width*0.9f;
		iH = pTexInfo.tImgInfo.Height;
		vPos.x = (((int)vPos.x / iW) *iW);
		vPos.y = (((int)vPos.y / iH) *iH);
	}
	else if (objInfo.wstrObjectName.find(L"Vertical_Wall") != wstring::npos)
	{
	iW = pTexInfo.tImgInfo.Width*0.75;
	iH = pTexInfo.tImgInfo.Height*0.8;
	vPos.x = (((int)vPos.x / iW) *iW);
	vPos.y = (((int)vPos.y / iH) *iH);
	}
	else if (objInfo.wstrObjectName.find(L"Wooden_Wall") != wstring::npos)
	{
		iW = pTexInfo.tImgInfo.Width;
		iH = pTexInfo.tImgInfo.Height*0.5;
		vPos.x = (((int)vPos.x / iW) *iW);
		vPos.y = (((int)vPos.y / iH) *iH);
	}
	else if (objInfo.wstrObjectName.find(L"Tree") != wstring::npos)
	{
		//iW = pTexInfo.tImgInfo.Width*0.5;
		//iH = pTexInfo.tImgInfo.Height;
		//vPos.x -=iW;
		//vPos.y -=iH;
	}
	/*else
	{
		iW = pTexInfo.tImgInfo.Width;
		iH = pTexInfo.tImgInfo.Height;
		vPos.x = (((int)vPos.x / iW) *iW);
		vPos.y = (((int)vPos.y / iH) *iH);
	}*/

}

void CRespawnManager::RemoveObj(D3DXVECTOR3 vPos)
{
	bool IsObj=false;
	auto itr = m_mObjects.begin();
	for_each(m_mObjects.begin(), m_mObjects.end(), [&](pair<OBJ_INFO*, D3DXVECTOR3> temp)
	{
		//cout << temp.second.y << endl;
		if (vPos.x - 30 <= temp.second.x &&vPos.x + 30 >= temp.second.x&&
				vPos.y - 30 <= temp.second.y &&vPos.y + 30 >= temp.second.y)
		{
			IsObj = true;
			
		}
		else
		{
			if(!IsObj)
				itr++;
		}
	});
	if (IsObj)
		m_mObjects.erase(itr);
}

void CRespawnManager::SaveData()
{

	//ofstream fout;
	//fout.open("../Data/RespawnData.txt", ios::trunc);
	//if (fout.fail())
	//	return;

	//for (auto pObj : m_mObjects)
	//{
	//	if (pObj.first->wstrObjectName.compare(L"") == 0)
	//		continue;
	//	fout << CW2A(pObj.first->wstrObjectName.c_str()) << endl;
	//	fout << CW2A(pObj.first->wstrObjectKey.c_str()) << endl;
	//	fout << CW2A(pObj.first->wstrStateKey.c_str()) << endl;
	//	fout << pObj.first->IsAni << endl;
	//	fout << pObj.first->ImageIDX << endl;
	//	fout << pObj.first->eObjectType << endl;
	//	fout << pObj.second.x << endl;
	//	fout << pObj.second.y << endl;
	//	fout << pObj.second.z << endl;
	//}
	//fout.close();
}

void CRespawnManager::FileSave(const TCHAR * pFilePath)
{
	HANDLE hFile = ::CreateFile(pFilePath, GENERIC_WRITE, 0, nullptr,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return;
	size_t iLen = 0;
	DWORD dwBytes = 0;
	for (auto pObj : m_mObjects)
	{
		iLen = pObj.first->wstrObjectName.length() + 1;
		WriteFile(hFile, &iLen, sizeof(size_t), &dwBytes, nullptr);
		WriteFile(hFile, pObj.first->wstrObjectName.c_str(), sizeof(wchar_t) * iLen, &dwBytes, nullptr);
		
		iLen = pObj.first->wstrObjectKey.length() + 1;
		WriteFile(hFile, &iLen, sizeof(size_t), &dwBytes, nullptr);
		WriteFile(hFile, pObj.first->wstrObjectKey.c_str(), sizeof(wchar_t) * iLen, &dwBytes, nullptr);
		
		iLen = pObj.first->wstrStateKey.length() + 1;
		WriteFile(hFile, &iLen, sizeof(size_t), &dwBytes, nullptr);
		WriteFile(hFile, pObj.first->wstrStateKey.c_str(), sizeof(wchar_t) * iLen, &dwBytes, nullptr);

		WriteFile(hFile, &pObj.first->IsAni, sizeof(bool), &dwBytes, nullptr);
		WriteFile(hFile, &pObj.first->ImageIDX, sizeof(WORD), &dwBytes, nullptr);
		WriteFile(hFile, &pObj.first->eObjectType, sizeof(OBJECT_TYPE), &dwBytes, nullptr);
		WriteFile(hFile, &pObj.second.x, sizeof(float), &dwBytes, nullptr);
		WriteFile(hFile, &pObj.second.y, sizeof(float), &dwBytes, nullptr);
		WriteFile(hFile, &pObj.second.z, sizeof(float), &dwBytes, nullptr);

		//WriteFile(hFile, m_vecTile[i], sizeof(TILE_INFO), &dwBytes, nullptr); //원본
		//WriteFile(hFile, &m_vecTile[i]->vPos, sizeof(D3DXVECTOR3), &dwBytes, nullptr);
		//WriteFile(hFile, &m_vecTile[i]->vSize, sizeof(D3DXVECTOR3), &dwBytes, nullptr);
		//WriteFile(hFile, &m_vecTile[i]->byDrawID, sizeof(BYTE), &dwBytes, nullptr);
		//WriteFile(hFile, &m_vecTile[i]->byOption, sizeof(BYTE), &dwBytes, nullptr);
		//wstring ws = m_vecTile[i]->wsTexKey+"/0"
	}
	CloseHandle(hFile);
}

void CRespawnManager::FileLoad(const TCHAR * pFilePath)
{
	HANDLE hFile = ::CreateFile(pFilePath, GENERIC_READ, 0, nullptr,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return;
/*
	if (!m_mObjects.empty())
	{
		for_each(m_mObjects.begin(), m_mObjects.end(), SafeDelete<TILE_INFO*>);
		m_mObjects.clear();
	}*/

	DWORD dwBytes = 0;
	OBJ_INFO* objInfo;
	D3DXVECTOR3 vPos;
	size_t iLen = 0;
	wchar_t* pString = nullptr;

	while (true)
	{
		objInfo = new OBJ_INFO;

		//ReadFile(hFile, &tTile, sizeof(TILE_INFO), &dwBytes, nullptr);
		ReadFile(hFile, &iLen, sizeof(size_t), &dwBytes, nullptr);
		pString = new wchar_t[iLen];
		ReadFile(hFile, pString, sizeof(wchar_t) * iLen, &dwBytes, nullptr);
		objInfo->wstrObjectName = pString;

		ReadFile(hFile, &iLen, sizeof(size_t), &dwBytes, nullptr);
		pString = new wchar_t[iLen];
		ReadFile(hFile, pString, sizeof(wchar_t) * iLen, &dwBytes, nullptr);
		objInfo->wstrObjectKey = pString;

		ReadFile(hFile, &iLen, sizeof(size_t), &dwBytes, nullptr);
		pString = new wchar_t[iLen];
		ReadFile(hFile, pString, sizeof(wchar_t) * iLen, &dwBytes, nullptr);
		objInfo->wstrStateKey = pString;

		ReadFile(hFile, &objInfo->IsAni, sizeof(bool), &dwBytes, nullptr);
		ReadFile(hFile, &objInfo->ImageIDX, sizeof(WORD), &dwBytes, nullptr);
		ReadFile(hFile, &objInfo->eObjectType, sizeof(OBJECT_TYPE), &dwBytes, nullptr);
		ReadFile(hFile, &vPos.x, sizeof(float), &dwBytes, nullptr);
		ReadFile(hFile, &vPos.y, sizeof(float), &dwBytes, nullptr);
		ReadFile(hFile, &vPos.z, sizeof(float), &dwBytes, nullptr);


		if (0 == dwBytes)
			break;

		m_mObjects.insert(make_pair(objInfo,vPos));
	}

	CloseHandle(hFile);
}

void CRespawnManager::LoadData()
{
	ifstream fin;

	fin.open("../Data/RespawnData.txt");

	if (fin.fail())
		return;

	CString csTemp;
	char cTemp[32];
	while (!fin.eof())
	{
		OBJ_INFO *temp = new OBJ_INFO;
		D3DXVECTOR3 vPos;
		fin.getline(cTemp, 32); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		csTemp = cTemp;
		temp->wstrObjectName = csTemp.GetString();
		fin.getline(cTemp, 32); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		csTemp = cTemp;
		temp->wstrObjectKey = csTemp.GetString();
		fin.getline(cTemp, 32); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		csTemp = cTemp;
		temp->wstrStateKey = csTemp.GetString();
		fin.getline(cTemp, 32); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		temp->IsAni = (bool)atoi(cTemp);
		fin.getline(cTemp, 32); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		temp->ImageIDX = (DWORD)atoi(cTemp);
		fin.getline(cTemp, 32); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		temp->eObjectType = (OBJECT_TYPE)atoi(cTemp);
		fin.getline(cTemp, 32);
		vPos.x = atof(cTemp);
		fin.getline(cTemp, 32);
		vPos.y = atof(cTemp);
		fin.getline(cTemp, 32);
		vPos.z = atof(cTemp);
		m_mObjects.insert(make_pair(temp, vPos));
	}
	fin.close();
}

void CRespawnManager::ConvertCenter(const OBJ_INFO& objInfo, const TEX_INFO& pTexInfo, float &cx, float &cy)
{
	//if (objInfo.wstrObjectName.find(L"TP") != wstring::npos)
	//{
	//	cx = pTexInfo.tImgInfo.Width*0.f;
	//	cy = pTexInfo.tImgInfo.Height*0.25f;
	//}
	//else if (objInfo.wstrObjectName.find(L"Horizontal_Wall") != wstring::npos)
	//{
	//	cx = pTexInfo.tImgInfo.Width*0.9f;
	//	cy = pTexInfo.tImgInfo.Height;
	//}
	//else if (objInfo.wstrObjectName.find(L"Vertical_Wall") != wstring::npos)
	//{
	//	cx = pTexInfo.tImgInfo.Width*0.75f;
	//	cy = pTexInfo.tImgInfo.Height*0.8f;
	//}
	//else if (objInfo.wstrObjectName.find(L"Wooden_Wall") != wstring::npos)
	//{
	//	cx = pTexInfo.tImgInfo.Width;
	//	cy = pTexInfo.tImgInfo.Height*0.5f;
	//}
	/*if (objInfo.wstrObjectName.find(L"Tree") != wstring::npos)
	{
		cx = pTexInfo.tImgInfo.Width*0.5f;
		cy = pTexInfo.tImgInfo.Height;
	}
	else
	{
	}*/
	if (objInfo.wstrObjectName.find(L"Tree") != wstring::npos)
	{
		cx = pTexInfo.tImgInfo.Width*0.5f;
		cy = pTexInfo.tImgInfo.Height;
	}
	else if (objInfo.wstrObjectName.find(L"Lamp") != wstring::npos)
	{
		cx = pTexInfo.tImgInfo.Width*0.5f;
		cy = pTexInfo.tImgInfo.Height;
	}
	else
	{
		cx = pTexInfo.tImgInfo.Width*0.5f;
		cy = pTexInfo.tImgInfo.Height*0.5f;
	}
	
}