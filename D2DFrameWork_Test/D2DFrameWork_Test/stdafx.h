// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <functional>
#include <fstream>

#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <deque>
#include <stack>
#include <cmath>



#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")
#include <d3dx9.h>
#include <d3dx9math.h>
#pragma comment(lib, "d3dx9.lib")

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include "Define.h"
#include "Enum.h"
#include "Struct.h"
#include "Extern.h"
#include "Function.h"
#include "tinyxml2.h"
#include "XMLManager.h"

using namespace std;

//manager
#include "KeyManager.h"
#include "TextureMgr.h"
#include "DeviceMgr.h"
#include "TimeMgr.h"
#include "SceneMgr.h"
#include "FrameMgr.h"
#include "ObjectMgr.h"
#include "ColliderMgr.h"
#include "SoundMgr.h"


//#ifdef _DEBUG
//#include "vld.h"
//#endif // _DEBUG



//#include "AstarMgr.h"