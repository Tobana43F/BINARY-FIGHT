//==========================================================
// 内容
//----------------------------------------------------------
// @DirectXプロジェクトで使用する基本的な関数群

#include	"MyDefinitions.h"	// DirectX,Wiiを切り替えるためにしようする

#define		MAIN_MODULE
#include	"Constants.h"		// MAIN_MODULEの定義により、グローバル変数を定義させるため。
#include	"Common.h"			// グローバル変数を扱うため

#ifdef DIRECTX
#include	"input.h"			// 入力の初期化に使用する
#include	"ShareBaseFunc.h"	// runなどの共通関数を使用するため
#include	"XAudio2.h"			// サウンドの初期化を行うため
#endif

#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif
//==========================================================
// Win32プロジェクト関係
//==========================================================
// エントリポイント
#ifdef DIRECTX
int WINAPI	WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	// メモリリーク検出用
#endif

	MSG			msg;

	// ウィンドウの作成
	if (!createWindow(hWnd, hInstance, nCmdShow))
		return 1;

	// DirectInput初期化
	InitInput(hInstance, hWnd);

	// XAudio2を初期化
	InitSound();

	// タイマークラスのオブジェクト生成
	timer = new Timer;

	// 値の初期化
	initBeforeMainLoop();

	// メインループ
	while (true){
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)){
			if (GetMessage(&msg, NULL, 0, 0) == 0)			// "WM_QUIT"メッセージを取得した場合"0"を返す
				break;

			TranslateMessage(&msg);							// 仮想キーメッセージを文字メッセージへ変換
			DispatchMessage(&msg);							// ウィンドウプロシージャへメッセージを送出
		} else
			run();
	}

	// メモリの開放など
	destroy();

	//safeDelete(timer);	// delete出来ない(エラーになる？　なぜ？？)

	// DirectInput解放
	UninitInput();
	
	// XAudio2の開放
	UninitSound();

	// DirectX終了処理関数
	DestroyD3D();
	return (int)msg.wParam;
}
#endif

//==========================================================
// ウィンドウプロシージャ
#ifdef DIRECTX
LRESULT CALLBACK	WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
		//----------------------------------------------------------
		// 必須メッセージ
		case WM_CREATE:
			break;
		case WM_CLOSE:
			DefWindowProc(hWnd, message, wParam, lParam);	//デフォルト処理を実行する。
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

			//----------------------------------------------------------
			// デフォルト処理
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
#endif

//==========================================================
// ウィンドウを作成する関数
#ifdef DIRECTX
bool createWindow(HWND &_hWnd, HINSTANCE _hInstance, int _nCmdShow)
{
	WNDCLASSEX	wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = _hInstance;
	wc.lpszClassName = NAME;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;

	if (!RegisterClassEx(&wc)){
		MessageBox(NULL, "ウィンドウクラス構造体の初期化エラー", "", MB_OK);
		return false;
	}

	_hWnd = CreateWindow(
		wc.lpszClassName,
		TITLE,
		WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		NULL,
		NULL,
		_hInstance,
		NULL);

	// ウィンドウサイズを再計算、移動
	RECT	rWindow, rClient;
	GetWindowRect(_hWnd, &rWindow);
	GetClientRect(_hWnd, &rClient);
	int width = (rWindow.right - rWindow.left) - (rClient.right - rClient.left) + WINDOW_WIDTH;
	int height = (rWindow.bottom - rWindow.top) - (rClient.bottom - rClient.top) + WINDOW_HEIGHT;
	// 設定を適応
	SetWindowPos(
		_hWnd,
		NULL,
		GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2,
		GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2,
		width - 1,
		height - 1,
		SWP_NOZORDER);

	// エラーが起こった場合
	if (!_hWnd)
		return false;

	// DirectX初期化関数
	if (FAILED(InitD3D(_hWnd)))
		return false;

	ShowWindow(_hWnd, _nCmdShow);
	UpdateWindow(_hWnd);

	return true;
}
#endif

//==========================================================
// DirectX関係
//==========================================================
// DirectX初期化関数
#ifdef DIRECTX
HRESULT InitD3D(HWND hWnd)
{
	D3DPRESENT_PARAMETERS	d3dpp;

	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pd3dDevice)))
	{
		return E_FAIL;
	}

	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	g_pd3dDevice->LightEnable(0, FALSE);

	return S_OK;
}
#endif

//==========================================================
// DirectX描画関数
#ifdef DIRECTX
void DrawD3D(void)
{
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(DEF_BACK_COLOR_R, DEF_BACK_COLOR_G, DEF_BACK_COLOR_B), 1.0f, 0);

	if (SUCCEEDED(g_pd3dDevice->BeginScene()))				// 描画開始
	{
		render();		// 共通の描画関数

		g_pd3dDevice->EndScene();							// 描画終了
	}
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}
#endif

//==========================================================
// DirectX終了処理関数
#ifdef DIRECTX
void DestroyD3D(void){
	if (g_pd3dDevice != NULL){
		g_pd3dDevice->Release();
	}
	if (g_pD3D != NULL){
		g_pD3D->Release();
	}
}
#endif