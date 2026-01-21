#include	"ModeManual.h"
#include	"MyTexture.h"
#include	"Common.h"			// gGameScene
#include	"ShareBaseFunc.h"
#include	"Constants.h"

//==========================================================
// ModeManual
//----------------------------------------------------------
// コンストラクタ
ModeManual::ModeManual():
PAGE_NUM(4)
{
	isSceneEnd = false;

	pTextureLoader = new TextureLoader(MAX_TEX);
	pTextureLoader->setUseTextureLabel(PAGE_1, textureNS::MANUAL_PAGE1);
	pTextureLoader->setUseTextureLabel(PAGE_2, textureNS::MANUAL_PAGE2);
	pTextureLoader->setUseTextureLabel(PAGE_3, textureNS::MANUAL_PAGE3);
	pTextureLoader->setUseTextureLabel(PAGE_4, textureNS::MANUAL_PAGE4);

	labelPageFlip = soundNS::MANUAL_SELECT_SE;

	load();
	init();
}

//----------------------------------------------------------
// デストラクタ
ModeManual::~ModeManual(){
	delete pTextureLoader;
	pTextureLoader = NULL;
}

//----------------------------------------------------------
// 初期化
void ModeManual::init(){
	scene = MANUAL_COMPLETE;
	curPage = 0;
}

//----------------------------------------------------------
// ロード
void ModeManual::load(){
	pTextureLoader->loadUseTexture();
}

//----------------------------------------------------------
// 描画
void ModeManual::draw(){
	switch (scene){
		case MANUAL_START:
			drawPage();
			break;

		case MANUAL_COMPLETE:
			drawPage();
			break;

		case MANUAL_END:
			drawPage();
			break;

		case END:
			drawPage();
			break;
	}
}

// ページの描画
void ModeManual::drawPage(){
	drawQuadPrimitive(
		0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
		0xFF, 0xFF, 0xFF,
		pTextureLoader->getTexturePointer(curPage), 0);
}

//----------------------------------------------------------
// 更新
void ModeManual::update(){
	switch (scene){
		case MANUAL_START:
			scene = MANUAL_COMPLETE;
			break;

		case MANUAL_COMPLETE:
			updateCursor();
			break;

		case MANUAL_END:
			scene = END;
			break;
			
		case END:
			isSceneEnd = true;
			break;
	}

	interruptReturnToTitle();
}

// ページめくりなどの処理
void ModeManual::updateCursor(){
	// ページめくり
	if (getInputState(inputNS::TRIGGER, BUTTON_RIGHT, inputNS::NO1)){
		curPage++;
		if (curPage >= PAGE_NUM)
			curPage = PAGE_NUM - 1;
		else
			playSound(labelPageFlip);

	}
	else if (getInputState(inputNS::TRIGGER, BUTTON_LEFT, inputNS::NO1)){
		curPage--;
		if (curPage < 0)
			curPage = 0;
		else
			playSound(labelPageFlip);
	}

	// １ボタンで画面を戻れるようにしておく
	if (getInputState(inputNS::HOLD, BUTTON_1, inputNS::NO1)){
		scene = MANUAL_END;
	}

}

//----------------------------------------------------------
// 割り込み的処理
void ModeManual::interruptReturnToTitle(){
	// タイトルに戻る
	if (getInputState(inputNS::HOLD, BUTTON_PLUS, inputNS::NO1) &&
		getInputState(inputNS::HOLD, BUTTON_MINUS, inputNS::NO1) ||
		getInputState(inputNS::HOLD, BUTTON_PLUS, inputNS::NO2) &&
		getInputState(inputNS::HOLD, BUTTON_MINUS, inputNS::NO2)){
		// 開放処理は、遷移後に行う
		gGameScene = RETURN_TO_TITLE;
	}
}