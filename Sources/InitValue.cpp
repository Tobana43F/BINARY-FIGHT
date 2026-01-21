//==========================================================
// 値の初期化をする関数

#include	"Title.h"					// タイトル確保

#include	"Common.h"					// グローバル変数を扱うため
#include	"ShareBaseFunc.h"			// initGameValue関数を使用するため
#include	"MyTexture.h"
#include	<stdlib.h>					// srand()を使用するため
#include	<time.h>					// 乱数の初期化時に使用する

//==========================================================
// プロトタイプ
void loadAllTexture();

//==========================================================
// 関数
//----------------------------------------------------------
// ゲームループ前に1度だけ行う初期化処理
void initBeforeMainLoop(){
	//----------------------------------------------------------
	// 共通の初期化処理
	srand((unsigned int)time(NULL));

	//----------------------------------------------------------
	// 使用するテクスチャのロード
	loadAllTexture();

	// 画面遷移用(仮
	gGameScene = TITLE_SCENE;
	pTitle = new Title;
}

//----------------------------------------------------------
// 使用するテクスチャを読み込む
void loadAllTexture(){
	// ビットマップフォント
	gTexFont.loadTexture(textureNS::FONT_DOT_AND_SHADOW);

	// 真っ白のテクスチャ
	gTexNothing.loadTexture(textureNS::_NO_TEXTURE);

	// 透明テクスチャ
	gTexAlpha.loadTexture(textureNS::_ALPHA_TEXTURE);
}
