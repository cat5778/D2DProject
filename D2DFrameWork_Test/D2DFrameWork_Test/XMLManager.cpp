#include "stdafx.h"
#include "XMLManager.h"

CXMLManager* CXMLManager::m_pInstance = nullptr;
CXMLManager * CXMLManager::GetInstance()
{
	if (nullptr == m_pInstance)
		m_pInstance = new CXMLManager;

	return m_pInstance;
}
void CXMLManager::DestroyInstance()
{
	if (nullptr != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

CXMLManager::CXMLManager()
{
}


CXMLManager::~CXMLManager()
{
}

