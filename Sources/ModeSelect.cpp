//==========================================================
// インクルード
#include	"ModeSelect.h"	// メソッド定義

#include	"Common.h"			// グローバル変数
#include	"Constants.h"		// ウィンドサイズの定数を参照するため
#include	"ShareBaseFunc.h"	// 文字列描画関数を使用するため,入力関数

//==========================================================
// ModeSelectクラス
//----------------------------------------------------------
// コンストラクタ
ModeSelect::ModeSelect() :
MENU_NUM(4),
STRING_ARCADE_MODE("ARCADE MODE"),
STRING_VERSUS_MODE("VERSUS MODE"),
STRING_TUTORIAL_MODE("TUTORIAL MODE"),
STRING_MANUAL_MODE("MANUAL"),
FONT_WIDTH(24),
FONT_HEIGHT(30),
FONT_BASE_X(WINDOW_WIDTH * 0.5f - WINDOW_WIDTH * 0.45f),
FONT_BASE_Y(WINDOW_HEIGHT * 0.05f),
MENU_GAP_HEIGHT(10.0f),
CURSOR_FLASH_SECOND(0.8f),
CURSOR_FLASH_INTERVAL_SELECTED(6)
{
	isSceneEnd = false;

	// テクスチャ関係
	pTextureLoader = new TextureLoader(MAX_TEX);
	pTextureLoader->setUseTextureLabel(FONT_TEX, textureNS::FONT_DOT_AND_SHADOW);
	pTextureLoader->setUseTextureLabel(CURSOR_TEX, textureNS::MODE_SELECT_CURSOR);
	pTextureLoader->setUseTextureLabel(BACK, textureNS::MODE_SELECT_BACK);
	pTextureLoader->setUseTextureLabel(STRING_ARCADE, textureNS::MODE_SELECT_DESCRIPTION_ARCADE);
	pTextureLoader->setUseTextureLabel(STRING_VERSUS, textureNS::MODE_SELECT_DESCRIPTION_VERSUS);
	pTextureLoader->setUseTextureLabel(STRING_TUTORIAL, textureNS::MODE_SELECT_DESCRIPTION_TUTORIAL);
	pTextureLoader->setUseTextureLabel(STRING_MANUAL, textureNS::MODE_SELECT_DESCRIPTION_MANUAL);

	// サウンド
	labelCursorSE = soundNS::CURSOR_SELECT_SE;
	labelDecideSE = soundNS::DECIDE_GENERAL_SE;
	labelBGM = soundNS::MODE_SELECT_BGM;

	// 文字列
	// メニュー項目のまとめ作成
	allMenuStringPointer = new const char*[MENU_NUM];
	allMenuStringPointer[ARCADE] = STRING_ARCADE_MODE;
	allMenuStringPointer[VERSUS] = STRING_VERSUS_MODE;
	allMenuStringPointer[TUTORIAL] = STRING_TUTORIAL_MODE;
	allMenuStringPointer[MANUAL] = STRING_MANUAL_MODE;

	playSound(labelBGM);

	// 仮)
	load();
	init();
}

//----------------------------------------------------------
// デストラクタ
ModeSelect::~ModeSelect(){
	delete[] allMenuStringPointer;
	allMenuStringPointer = NULL;
	delete pTextureLoader;
	pTextureLoader = NULL;

	stopSound(labelBGM);	// 念のため

}

//----------------------------------------------------------
// 初期化
void ModeSelect::init(){
	scene = MENU_START;
	isSelected = false;
	cnt = 0;
	cntStart = cnt;

	pullX = 1;

	curCursor = 0;
	polyCursor.setTexturePointer(pTextureLoader->getTexturePointer(CURSOR_TEX));

	polyBack.setData(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	polyBack.setTexturePointer(pTextureLoader->getTexturePointer(BACK));
	polyDescription.setData(0, WINDOW_HEIGHT * 0.83f, WINDOW_WIDTH, 64);
	polyDescription.setTexturePointer(pTextureLoader->getTexturePointer(STRING_ARCADE + curCursor));
}

//----------------------------------------------------------
// 読み込み
void ModeSelect::load(){
	pTextureLoader->loadUseTexture();
}
//----------------------------------------------------------
// 描画
void ModeSelect::draw(){
	switch (scene){
		case MENU_START:
			// メニューをどう現させるか
			break;

		case MENU_SELECTING:
			drawMenuSelecting();
			break;

		case MENU_SELECTED:
			// 選択項目をちかちかさせたりする
			drawMenuSelected();
			break;

		case MENU_END:
			break;
	}

}

//----------------------------------------------------------
// メニューの描画
void ModeSelect::drawMenuSelecting(){
	polyBack.drawPolygon();
	polyDescription.drawPolygon();

	// カーソルの表示
	polyCursor.drawPolygon();

	// 仮)選択したモードを前に出す
	float tmpPushX;	// X軸を増やす

	// メニュー項目の表示
	for (int i = 0; i < MENU_NUM; ++i){
		if (i == curCursor)
			tmpPushX = FONT_WIDTH;
		else
			tmpPushX = 0;

		// 説明文の変更
		polyDescription.setTexturePointer(pTextureLoader->getTexturePointer(STRING_ARCADE + curCursor));

		// 色は仮)
		printfText(
			FONT_BASE_X + tmpPushX, FONT_BASE_Y + (FONT_HEIGHT * i) + (MENU_GAP_HEIGHT * i),
			FONT_WIDTH, FONT_HEIGHT,
			0x00, 0xFF, 0xFF,
			pTextureLoader->getTexturePointer(FONT_TEX),
			allMenuStringPointer[i]
			);
	}
}

//----------------------------------------------------------
// 選択後の描画
void ModeSelect::drawMenuSelected(){
	polyBack.drawPolygon();
	polyDescription.drawPolygon();

	// カーソルの表示
	if (cnt % CURSOR_FLASH_INTERVAL_SELECTED == 0)	// 点滅させる
		polyCursor.drawPolygon();

	// 仮)選択したモードを前に出す
	float tmpPushX;	// X軸を増やす
	float tmpX;

	// メニュー項目の表示
	for (int i = 0; i < MENU_NUM; ++i){
		if (i == curCursor){
			tmpPushX = FONT_WIDTH * 1.2f;
			tmpX = FONT_BASE_X + tmpPushX;
		} else{
			tmpPushX = 0;
			pullX += pullX * 2;
			tmpX = FONT_BASE_X - pullX;
		}

		// 色は仮)
		printfText(
			tmpX, FONT_BASE_Y + (FONT_HEIGHT * i) + (MENU_GAP_HEIGHT * i),
			FONT_WIDTH, FONT_HEIGHT,
			0x00, 0xFF, 0xFF,
			pTextureLoader->getTexturePointer(FONT_TEX),
			allMenuStringPointer[i]
			);
	}
}


//----------------------------------------------------------
// 更新
void ModeSelect::update(){
	switch (scene){
		case MENU_START:
			scene = MENU_SELECTING;
			break;

		case MENU_SELECTING:
			updateCursor();
			break;

		case MENU_SELECTED:
			updateMenuSelected();
			break;

		case MENU_END:
			updateGameScene();
			break;
	}
	cnt++;

	interruptReturnToTitle();
}

//----------------------------------------------------------
// カーソル位置の更新
void ModeSelect::updateCursor(){
	if (getInputState(inputNS::TRIGGER, BUTTON_UP, inputNS::NO1)){
		curCursor--;
		if (curCursor < 0)
			curCursor = MENU_NUM - 1;
		playSound(labelCursorSE);
	}
	if (getInputState(inputNS::TRIGGER, BUTTON_DOWN, inputNS::NO1)){
		curCursor++;
		if (curCursor >= MENU_NUM)
			curCursor = 0;
		playSound(labelCursorSE);
	}

	isSelected = false;
	if (getInputState(inputNS::TRIGGER, BUTTON_2, inputNS::NO1)){
		isSelected = true;
	}

	// メニューのサイズにカーソルを合わせる
	polyCursor.setData(
		0, FONT_BASE_Y + (FONT_HEIGHT * curCursor) + (MENU_GAP_HEIGHT * curCursor),
		FONT_WIDTH * (stringLen(allMenuStringPointer[curCursor]) + 3), FONT_HEIGHT	// 幅を3文字分大きくする
		);

	if (isSelected && curCursor == ARCADE || 
		isSelected && curCursor == VERSUS ||
		isSelected && curCursor == TUTORIAL ||
		isSelected && curCursor == MANUAL){	// 仮)選択肢が完成するまでの間の処理
		playSound(labelDecideSE);
		scene = MENU_SELECTED;
		stopSound(labelBGM);
		cntStart = cnt;
	}
}

//----------------------------------------------------------
// 選択後の更新
void ModeSelect::updateMenuSelected(){
	if (cnt - cntStart >= (int)(DEF_FRAME_RATE * CURSOR_FLASH_SECOND)){
		scene = MENU_END;
		cntStart = cnt;
	}
}

//----------------------------------------------------------
// 画面遷移
void ModeSelect::updateGameScene(){
	// 仮)選択項目により、遷移させる先を変えるようにする

	switch (curCursor){
		case ARCADE:
			gGameScene = GS_ARCADE_MODE;
			pModeArcade = new ModeArcade;
			break;
		case VERSUS:
			gGameScene = GS_VERSUS_MODE;
			pModeVersus = new ModeVersus;
			break;

		case TUTORIAL:
			gGameScene = GS_TUTORIAL_MODE;
			pModeTutorial = new ModeTutorial;
			break;

		case MANUAL:
			gGameScene = GS_MANUAL_MODE;
			pModeManual = new ModeManual;
			break;

	}
	isSceneEnd = true;
}

//----------------------------------------------------------
// 割り込み的処理
void ModeSelect::interruptReturnToTitle(){
	// タイトルに戻る
	if (getInputState(inputNS::HOLD, BUTTON_PLUS, inputNS::NO1) &&
		getInputState(inputNS::HOLD, BUTTON_MINUS, inputNS::NO1) ||
		getInputState(inputNS::HOLD, BUTTON_PLUS, inputNS::NO2) &&
		getInputState(inputNS::HOLD, BUTTON_MINUS, inputNS::NO2)){
		// 開放処理は、遷移後に行う
		gGameScene = RETURN_TO_TITLE;
	}
}