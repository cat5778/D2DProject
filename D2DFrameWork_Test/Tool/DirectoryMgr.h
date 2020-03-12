#pragma once
class CDirectoryMgr
{
public:
	CDirectoryMgr();
	~CDirectoryMgr();


public:
	static CString ConvertRelativePath(const TCHAR* pFullPath);
	static void ExtractPathInfo(const TCHAR* pFullPath, list<PATH_INFO*>& rPathInfoList);
	static int GetFileCount(const TCHAR* pFullPath);



};

