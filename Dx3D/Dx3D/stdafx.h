﻿// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")

#import "C:\\Program Files\\Common Files\\System\\ado\\msado15.dll" no_namespace rename("EOF", "EndOfFile")

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <queue>
#include <vector>
#include <map>
#include <time.h>
#include <assert.h>
#include <set>
#include <list>

#include "json.hpp"
using json = nlohmann::json;

#define FPS_LIMIT   1000.0f
#define W_WIDTH     1600
#define W_HEIGHT    900

using namespace std;

#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "DefineMacro.h"
#include "DataStructure.h"
#include "InlineMacro.h"

extern HWND     g_hWnd;
extern POINT    g_ptMouse;
extern POINT    g_ptDeltaMouse;
extern POINT    g_ptLClick;
extern POINT    g_ptRClick;
extern POINT    g_ptLDoubleClick;
extern POINT    g_ptRDoubleClick;
extern INT      g_nWheelMouse;
extern Vector3  g_vCameraPos;

#include "cRay.h"

// 매니져 클래스 추가
#include "cLogManager.h"
#include "cDbManager.h"
#include "cSceneManager.h"
#include "cKeyManager.h"
#include "cSoundManager.h"
#include "cDeviceManager.h"
#include "cTextureManager.h"
#include "cFontManager.h"
#include "cTimerManager.h"
#include "cObjectManager.h"
#include "cAutoReleasePool.h"
#include "cMeshManager.h"
#include "cShaderManager.h"
#include "cCameraManager.h"
#include "cLightManager.h"
#include "cMaterialManager.h"
#include "cMapManager.h"
#include "cCharacterManager.h"
#include "cGameManager.h"