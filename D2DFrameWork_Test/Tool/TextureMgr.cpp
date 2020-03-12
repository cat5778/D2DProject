#include "stdafx.h"
#include "TextureMgr.h"
#include "SingleTexture.h"
#include "MultiTexture.h"

IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr()
{
}


CTextureMgr::~CTextureMgr()
{
	Release();
}

const TEX_INFO* CTextureMgr::GetTexInfo(
	const wstring& wstrObjectKey, 
	const wstring& wstrStateKey, 
	const WORD& wIndex) const
{
	auto iter_find = m_mapTexture.find(wstrObjectKey);

	if(m_mapTexture.end() == iter_find)
		return nullptr;

	return iter_find->second->GetTexInfo(wstrStateKey, wIndex);
}

HRESULT CTextureMgr::LoadTextureFrom
(LPDIRECT3DDEVICE9 pGrahicDev, const wstring & wstrFilePath)
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

		HRESULT hr = LoadTexture(pGrahicDev, TEXTURE_MULTI,
			szRelative, szObjectKey, szStateKey, _ttoi(szCount));

		if (FAILED(hr))
		{
			fin.close();
			return E_FAIL;
		}
	}

	fin.close();

	return S_OK;

}

HRESULT CTextureMgr::LoadTexture(
	LPDIRECT3DDEVICE9 pGrahicDev,
	TEXTURE_TYPE eTextureType, 
	const wstring& wstrFilePath, 
	const wstring& wstrObjectKey, 
	const wstring& wstrStateKey, 
	const WORD& wCnt)
{
	CTexture* pTexture = nullptr;

	auto iter_find = m_mapTexture.find(wstrObjectKey);	

	if (m_mapTexture.end() == iter_find)
	{
		switch (eTextureType)
		{
		case TEXTURE_SINGLE:
			pTexture = CSingleTexture::Create(pGrahicDev, wstrFilePath);
			break;
		case TEXTURE_MULTI:
			pTexture = CMultiTexture::Create(pGrahicDev, wstrFilePath, wstrStateKey, wCnt);
			break;
		}

		NULL_CHECK_RETURN(pTexture, E_FAIL);
		m_mapTexture.insert(make_pair(wstrObjectKey, pTexture));
	}
	else
	{
		HRESULT hr = 0;

		switch (eTextureType)
		{
		case TEXTURE_SINGLE:
			return E_FAIL;
		case TEXTURE_MULTI:
			hr = m_mapTexture[wstrObjectKey]->LoadTexture(pGrahicDev, wstrFilePath, wstrStateKey, wCnt);
			FAILED_CHECK_RETURN(hr, E_FAIL);
			break;
		}
	}	

	return S_OK;
}

void CTextureMgr::Release()
{
	for (auto& MyPair : m_mapTexture)
		SafeDelete(MyPair.second);

	m_mapTexture.clear();
}
