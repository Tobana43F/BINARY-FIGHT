//==========================================================
// 共通項目を定義
#ifndef _COMMON
#define	_COMMON

//==========================================================
// マクロ定義

//==========================================================
// インクルード

#include	"MyDefinitions.h"			// DirectX・Wiiの切り替えも行うので最初にインクルード

#ifdef DIRECTX

#pragma warning(disable : 4996)			// セキュリティ関係の警告を無視するため
#include	<Windows.h>					// WinApi関係のプロトタイプの宣言
#include	<d3dx9.h>					// D3DデバイスなどのDirectX関係のため
#pragma comment(lib, "d3d9.lib")		// 〃
#pragma comment(lib, "d3dx9.lib")		// 〃

#include	"Timer.h"					// Timer型のグローバル変数を宣言するため

//#if _DEBUG
//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>
//#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#endif


#endif
#ifdef WII
#endif

// 共通
#include	"Title.h"				// グローバル変数定義
#include	"GameReady.h"			// 〃
#include	"Battle.h"				// 〃
#include	"CharacterSelect.h"		// 〃
#include	"ModeSelect.h"			// 〃
#include	"ModeArcade.h"			// 
#include	"ModeVersus.h"
#include	"ModeManual.h"		
#include	"ModeTutorial.h"

//==========================================================
// 定数定義

//==========================================================
// 構造体・クラス

//==========================================================
// プロトタイプ宣言
#ifdef DIRECTX
int		WINAPI	WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);	// DirectXプロジェクト用エントリポイント(念のため)
LRESULT	CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);												// 
bool	createWindow(HWND &_hWnd, HINSTANCE _hInstance, int _nCmdShow);									// 
HRESULT	InitD3D(HWND);
void	DrawD3D(void);
void	DestroyD3D(void);
#endif 
#ifdef WII
#endif


//==========================================================
// グローバル変数宣言
#ifdef MAIN_MODULE													// externの指定
#define	EXTERN
#define	INIT(value)	=value											// 初期値を入れるマクロ
#else
#define	EXTERN extern 
#define	INIT(value)
#endif

#ifdef DIRECTX
EXTERN HWND					hWnd;
EXTERN LPDIRECT3D9			g_pD3D;
EXTERN LPDIRECT3DDEVICE9	g_pd3dDevice;

EXTERN Timer				*timer;
#endif
#ifdef WII
#endif

//----------------------------------------------------------
// 共通

// 使用する全テクスチャ
EXTERN TextureUV gTexNothing;		// テスト時に使用
EXTERN TextureUV gTexAlpha;			// 透明色テクスチャ(色は頂点カラーで変更)
EXTERN TextureUV gTexFont;			// フォント

// タイトル
EXTERN Title	  *pTitle;

EXTERN ModeSelect	*pModeSelect;
EXTERN ModeArcade	*pModeArcade;
EXTERN ModeVersus	*pModeVersus;
EXTERN ModeTutorial	*pModeTutorial;
EXTERN ModeManual	*pModeManual;

EXTERN eGameScene gGameScene;				// ゲームシーン(画面遷移用)
EXTERN int		  gCounter;					// グローバルカウンタ

#endif	// !_COMMON