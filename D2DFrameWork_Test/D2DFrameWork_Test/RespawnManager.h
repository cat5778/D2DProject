#pragma once

class CRespawnManager
{
	DECLARE_SINGLETON(CRespawnManager)
public:
	CRespawnManager();
	~CRespawnManager();
	
public:
	float ZOrder(float _y);
	bool CheckObject(const OBJ_INFO& objInfo , D3DXVECTOR3 &vPos);
	void Render();
	void ConvertPos(const OBJ_INFO& objInfo, const TEX_INFO& pTexInfo, D3DXVECTOR3 &vPos);
	void RemoveObj(D3DXVECTOR3 vPos);
	//void SaveData();
	void LoadData(wstring wsDataPath);
	void FileLoad(const TCHAR * pFilePath);
	void ConvertCenter(const OBJ_INFO& objInfo, const TEX_INFO& pTexInfo, float &cx, float &cy);
	
private:
	void Release();
public:
	multimap<OBJ_INFO*, D3DXVECTOR3>	m_mObjects;

};

