//==========================================================
// 終了処理
#include	"Common.h"
#include	"ShareBaseFunc.h"

//==========================================================
// 終了処理関数
void destroy(){
	//----------------------------------------------------------
	// メモリの解放など

	// グローバル変数の安全な開放
	safeDelete(pTitle);
	safeDelete(pModeSelect);
	safeDelete(pModeArcade);
	safeDelete(pModeVersus);
	safeDelete(pModeManual);
}
