#pragma once


const DWORD KEY_W = 0x00000001;	// 0000 0000 0000 0000 0000 0000 0000 0001
const DWORD KEY_S = 0x00000002;	// 0000 0000 0000 0000 0000 0000 0000 0010
const DWORD KEY_A = 0x00000004;	// 0000 0000 0000 0000 0000 0000 0000 0100
const DWORD KEY_D = 0x00000008;	// 0000 0000 0000 0000 0000 0000 0000 1000
const DWORD KEY_SPACE = 0x00000010;	// 0000 0000 0000 0000 0000 0000 0001 0000
const DWORD KEY_SHIFT = 0x00000020;	// 0000 0000 0000 0000 0000 0000 0010 0000
const DWORD KEY_LBUTTON = 0x00000040;	// 0000 0000 0000 0000 0000 0000 0100 0000
const DWORD KEY_RBUTTON = 0x00000080;	// 0000 0000 0000 0000 0000 0000 1000 0000
const DWORD KEY_R = 0x00000100;	// 0000 0000 0000 0000 0000 0001 0000 0000
const DWORD KEY_F = 0x00000200;	// 0000 0000 0000 0000 0000 0010 0000 0000
const DWORD KEY_RETURN = 0x00000400;	// 0000 0000 0000 0000 0000 0010 0000 0000
const DWORD KEY_O = 0x00000800;	// 0000 0000 0000 0000 0000 0010 0000 0000
const DWORD KEY_1 = 0x00001000;
const DWORD KEY_2 = 0x00004000;
const DWORD KEY_3 = 0x00008000;
const DWORD KEY_4 = 0x00010000;
const DWORD KEY_5 = 0x00020000;
const DWORD KEY_6 = 0x00040000;
const DWORD KEY_T = 0x00080000;
const DWORD KEY_ESC = 0x00100000;
const DWORD KEY_I =   0x00200000;
class CKeyManager
{
public:
	static CKeyManager* GetInstance();
	void DestroyInstance();
private:
	static CKeyManager* m_pInstance;
public:
	CKeyManager();
	~CKeyManager();

public:
	void Update();

public:
	bool KeyDown(DWORD dwKey);
	bool KeyUp(DWORD dwKey);
	bool KeyPressing(DWORD dwKey);

private:
	DWORD m_dwKeyDown;
	DWORD m_dwKeyUp;
	DWORD m_dwCurKey;
	float m_fScale;

};

