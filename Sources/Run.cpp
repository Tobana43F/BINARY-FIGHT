//==========================================================
// メイン処理の実行

//==========================================================
// インクルード
#include	"MyDefinitions.h"			// DirectX,Wiiの切り替え
#include	"Common.h"					// グローバル変数を利用するため
#include	"ShareBaseFunc.h"			// runのプロトタイプ宣言

#include	<stdio.h>					// sprintf , vsprintfなどを使うため	
#include	<stdarg.h>					// 可変引数を利用した関数を作るため

//==========================================================
// メイン処理の実行
void run(){
#ifdef DIRECTX
	if (!timer->frameControl())	// FPS制御
		return;

	// FPSをタイトルバーに出力
	// スコープ管理
	{
		char buf[100];
		sprintf(buf, "%s [%.1f/%.1f]", TITLE, timer->getFPS(), timerNS::FRAME_RATE);
		SetWindowText(hWnd, buf);
	}

	// 入力状態を更新
	UpdateInput();
#endif
#ifdef WII

#endif

	// 画面遷移はこの二つの関数内、双方で行う
	update();
	draw();

	// グローバルカウンタ
	gCounter++;
}