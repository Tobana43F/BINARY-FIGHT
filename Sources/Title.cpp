#include	"Title.h"				// Titleクラスのメソッドを定義するため

#include	"MyTexture.h"

// 仮)
#include	"ShareBaseFunc.h"		// 文字列を表示するため	
#include	"Common.h"				// グローバル変数を使用するため


//==========================================================
// Titleクラス
//----------------------------------------------------------
// コンストラクタ
Title::Title() :
STRING_FLASH_INTERVAL_NORMAL(30),
STRING_FLASH_INTERVAL_SELECTED(3),
STRING_WIDTH(512),
STRING_HEIGHT(64),
STRING_X(WINDOW_WIDTH / 2.0f - STRING_WIDTH / 2.0f),
STRING_Y(WINDOW_HEIGHT * 0.7)
{
	// テクスチャ
	pTextureLoader = new TextureLoader(MAX_TEX);
	pTextureLoader->setUseTextureLabel(BACK, textureNS::TITLE_BACK);
	pTextureLoader->setUseTextureLabel(STRING_PUSH, textureNS::TITLE_STRING);
	pTextureLoader->loadUseTexture();

	polyBack.setData(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	polyBack.setTexturePointer(pTextureLoader->getTexturePointer(BACK));
	polyString.setData(STRING_X, STRING_Y, STRING_WIDTH, STRING_HEIGHT);
	polyString.setTexturePointer(pTextureLoader->getTexturePointer(STRING_PUSH));

	// サウンド
	labelDecideSE = soundNS::TITLE_ENTER_SE;
	labelBGM = soundNS::TITLE_BGM;

	playSound(labelBGM);

	init();
}

//----------------------------------------------------------
// デストラクタ
Title::~Title(){
	delete pTextureLoader;
	pTextureLoader = NULL;
}

//----------------------------------------------------------
// デストラクタ
void Title::init(){
	isSceneEnd = false;
	// カウンタ初期化
	cntStringDraw = 0;
	isStringDraw = true;
	cnt = 0;
	cntStart = cnt;

	scene = TITLE_COMPLETE;
}

//----------------------------------------------------------
// 描画
void Title::draw(){
	switch (scene){
		case TITLE_START:
			drawTitleStart();
			break;

		case TITLE_COMPLETE:
			drawTitleComplete();
			break;

		case GAME_START:
			drawGameStart();
			break;

		case TITLE_END:
			drawTitleEnd();
			break;
	}
}

//----------------------------------------------------------
// 更新
void Title::update(){
	switch (scene){
		case TITLE_START:
			updateTitleStart();
			break;

		case TITLE_COMPLETE:
			updateTitleComplete();
			break;

		case GAME_START:
			updateGameStart();
			break;

		case TITLE_END:
			updateTitleEnd();
			return;
			break;
	}

	// カウンタ更新
	cnt++;
}

//----------------------------------------------------------
// ゲームシーンの更新
void Title::updateGameScene(){
	isSceneEnd = true;
	gGameScene = RETURN_TO_MODE_SELECT;
}

//----------------------------------------------------------
// クラス内遷移ごとの描画
void Title::drawTitleStart(){
	polyBack.drawPolygon();
}

void Title::drawTitleComplete(){
	polyBack.drawPolygon();
	if (isStringDraw)
		polyString.drawPolygon();
}

void Title::drawGameStart(){
	polyBack.drawPolygon();
	// 文字を点滅させる
	if (isStringDraw)
		polyString.drawPolygon();
}

void Title::drawTitleEnd(){
	polyBack.drawPolygon();
}

//----------------------------------------------------------
// クラス内遷移ごとの更新
void Title::updateTitleStart(){
	if (cnt - cntStart >= (int)(DEF_FRAME_RATE * 2)){
		cntStart = cnt;			// 遷移後の開始フレームとして取得
		scene = TITLE_COMPLETE;
	}
}

void Title::updateTitleComplete(){
	// 文字の点滅描画更新
	cntStringDraw++;
	if (cntStringDraw > STRING_FLASH_INTERVAL_NORMAL){
		cntStringDraw = 0;	// カウンタ初期化
		isStringDraw = !isStringDraw;	// フラグ反転
	}

	// 放置状態を取得(10秒放置されたら)
	if (cnt - cntStart > (int)(DEF_FRAME_RATE * 10)){
		cntStart = cnt;
		scene = TITLE_COMPLETE;	// 仮)デモ画面が作れれば良いかも
	}

	// 入力でスタート
	if (getInputState(inputNS::TRIGGER, BUTTON_PLUS, inputNS::NO1) ||
		getInputState(inputNS::TRIGGER, BUTTON_PLUS, inputNS::NO2)){
		cntStart = cnt;
		scene = GAME_START;
		playSound(labelDecideSE);
		stopSound(labelBGM);
	}
}

void Title::updateGameStart(){
	// 文字の点滅描画更新
	cntStringDraw++;
	if (cntStringDraw > STRING_FLASH_INTERVAL_SELECTED){
		cntStringDraw = 0;	// カウンタ初期化
		isStringDraw = !isStringDraw;	// フラグ反転
	}

	if (cnt - cntStart >= (int)(DEF_FRAME_RATE * 1)){
		cntStart = cnt;
		scene = TITLE_END;
	}
}

void Title::updateTitleEnd(){
	updateGameScene();
}
