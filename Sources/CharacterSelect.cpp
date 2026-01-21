#include	"CharacterSelect.h"
#include	"ShareBaseFunc.h"
#include	"Polygon.h"
#include	"MyTexture.h"
#include	"BattleConfigInfo.h"	// 引数に持つため
#include	"constCharacterList.h"
#include	"GameReady.h"
#include	"TextureLoader.h"

#include	"Common.h"	// テスト用)

//==========================================================
// CharacterSelectクラス
//----------------------------------------------------------
// コンストラクタ
CharacterSelect::CharacterSelect(BattleConfigInfo *_pBattleConfigInfo):
BOX_DIV_X(4),
BOX_DIV_Y(2),
CHARACTER_NUM(8),	// BOX_DIV_X * BOX_DIV_Y
BOX_GAP_WIDTH(5),
BOX_GAP_HEIGHT(5),
BOX_WIDTH(100),
BOX_HEIGHT(100),
BOX_BASE_X((WINDOW_WIDTH / 2.0f) - ((BOX_DIV_X * BOX_WIDTH) / 2.0f)),
BOX_BASE_Y(WINDOW_HEIGHT * 0.15f)
{
	isSceneEnd = false;

	this->pBattleConfigInfo = _pBattleConfigInfo;	// 代入するバトル情報クラスへのポインタを持つ

	// テクスチャの初期化
	pTextureLoader = new TextureLoader(MAX_TEX);
	pTextureLoader->setUseTextureLabel(CHAR_DOTS, textureNS::CHARACTER_SELECT_DOTS);
	pTextureLoader->setUseTextureLabel(CHAR_RESERVE_2, textureNS::CHARACTER_SELECT_RESERVE);
	pTextureLoader->setUseTextureLabel(CHAR_RESERVE_3, textureNS::CHARACTER_SELECT_RESERVE);
	pTextureLoader->setUseTextureLabel(CHAR_RESERVE_4, textureNS::CHARACTER_SELECT_RESERVE);
	pTextureLoader->setUseTextureLabel(CHAR_RESERVE_5, textureNS::CHARACTER_SELECT_RESERVE);
	pTextureLoader->setUseTextureLabel(CHAR_RESERVE_6, textureNS::CHARACTER_SELECT_RESERVE);
	pTextureLoader->setUseTextureLabel(CHAR_RESERVE_7, textureNS::CHARACTER_SELECT_RESERVE);
	pTextureLoader->setUseTextureLabel(CHAR_RANDOM, textureNS::CHARACTER_SELECT_RANDOM);

	pTextureLoader->setUseTextureLabel(PLAYER1, textureNS::CHARACTER_SELECT_FRAME_PLAYER1);
	pTextureLoader->setUseTextureLabel(PLAYER2, textureNS::CHARACTER_SELECT_FRAME_PLAYER2);
	pTextureLoader->setUseTextureLabel(BACK, textureNS::CHARACTER_SELECT_BACK);

	pTextureLoader->setUseAnimationLabel(CHAR_DOTS, animationNS::NO_ANM);
	pTextureLoader->setUseAnimationLabel(CHAR_RESERVE_2, animationNS::NO_ANM);
	pTextureLoader->setUseAnimationLabel(CHAR_RESERVE_3, animationNS::NO_ANM);
	pTextureLoader->setUseAnimationLabel(CHAR_RESERVE_4, animationNS::NO_ANM);
	pTextureLoader->setUseAnimationLabel(CHAR_RESERVE_5, animationNS::NO_ANM);
	pTextureLoader->setUseAnimationLabel(CHAR_RESERVE_6, animationNS::NO_ANM);
	pTextureLoader->setUseAnimationLabel(CHAR_RESERVE_7, animationNS::NO_ANM);
	pTextureLoader->setUseAnimationLabel(CHAR_RANDOM, animationNS::NO_ANM);

	pTextureLoader->setUseAnimationLabel(PLAYER1, animationNS::CHARACTER_SELECT_FRAME);
	pTextureLoader->setUseAnimationLabel(PLAYER2, animationNS::CHARACTER_SELECT_FRAME);
	pTextureLoader->setUseAnimationLabel(BACK, animationNS::NO_ANM);

	pTextureLoader->loadUseTexture();
	pTextureLoader->loadUseAnimation();

	// サウンド
	labelDecideSE = soundNS::CHARACTER_SELECT_DECIDE_SE;
	labelCancelSE = soundNS::CHARACTER_SELECT_CANCEL_SE;
	labelSelectSE = soundNS::CURSOR_SELECT_SE;
	labelBGM = soundNS::CHARACTER_SELECT_BGM;

	// 初期化
	curPlayer1Cursor = 0;
	curPlayer2Cursor = 0;

	isPlayer1Selected = false;
	isPlayer2Selected = false;

	// 背景
	polyBack.setData(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	polyBack.setTexturePointer(pTextureLoader->getTexturePointer(BACK));

	// X,Yの選択位置に変換(座標ではない)
	char tmpCursorX;
	char tmpCursorY;
	// p1側
	tmpCursorX = (curPlayer1Cursor % BOX_DIV_X);
	tmpCursorY = (curPlayer1Cursor / BOX_DIV_X);
	polyPlayer1Cursor.setData
		(BOX_BASE_X + (tmpCursorX * BOX_WIDTH), BOX_BASE_Y + (BOX_HEIGHT * tmpCursorY),
		BOX_WIDTH, BOX_HEIGHT);

	polyPlayer1Cursor.setTexturePointer(pTextureLoader->getTexturePointer(PLAYER1));

	// p2側
	tmpCursorX = (curPlayer2Cursor % BOX_DIV_X);
	tmpCursorY = (curPlayer2Cursor / BOX_DIV_X);
	polyPlayer1Cursor.setData
		(BOX_BASE_X + (tmpCursorX * BOX_WIDTH), BOX_BASE_Y + (BOX_HEIGHT * tmpCursorY),
		BOX_WIDTH, BOX_HEIGHT);

	polyPlayer2Cursor.setTexturePointer(pTextureLoader->getTexturePointer(PLAYER2));

	playSound(labelBGM);
}

//----------------------------------------------------------
// デストラクタ
CharacterSelect::~CharacterSelect(){
	delete pTextureLoader;
	pTextureLoader = NULL;

	stopSound(labelBGM);
}

//----------------------------------------------------------
// 描画
void CharacterSelect::draw(){
	polyBack.drawPolygon();
	drawCharacterBox();
	drawFrame();
}

//----------------------------------------------------------
// キャラクターの選択肢を描画
void CharacterSelect::drawCharacterBox(){
	for (int i = 0; i < BOX_DIV_Y; ++i){
		for (int j = 0; j < BOX_DIV_X; ++j){
			drawQuadPrimitive(
				BOX_BASE_X + (BOX_WIDTH * j) + (BOX_GAP_WIDTH * j), BOX_BASE_Y + (BOX_HEIGHT * i) + (BOX_GAP_HEIGHT * i),
				BOX_WIDTH, BOX_HEIGHT,
				0xff, 0xff, 0xff,
				pTextureLoader->getTexturePointer((BOX_DIV_X * i) + j), 0);
		}
	}
}

//----------------------------------------------------------
// 選択中のフレームの表示
void CharacterSelect::drawFrame(){
	polyPlayer1Cursor.drawPolygon();
	polyPlayer2Cursor.drawPolygon();
}

//----------------------------------------------------------
// 更新
void CharacterSelect::update(){
	updateGameScene();
	if (getIsSceneEnd())
		return;

	updateCursor();
}

//----------------------------------------------------------
// ゲームシーンの遷移
void CharacterSelect::updateGameScene(){
	if (isPlayer1Selected == true && isPlayer2Selected == true){// 両プレイヤー、キャラクターを選択し終わっていたら
		pBattleConfigInfo->setSelectCharacter(
			(eCharacterList)curPlayer1Cursor, (eCharacterList)curPlayer2Cursor);
		pBattleConfigInfo->setPlayerID(1, PLAYERID_PLAYER1);
		pBattleConfigInfo->setPlayerID(2, PLAYERID_PLAYER2);

		isSceneEnd = true;
	}
}

//----------------------------------------------------------
// 選択処理
void CharacterSelect::updateCursor(){
	//----------------------------------------------------------
	// カーソルの操作
	updatePlayer1Cursor();
	updatePlayer2Cursor();
}

void CharacterSelect::updatePlayer1Cursor(){
	// p1
	if (isPlayer1Selected == false){
		int prevCursor = curPlayer1Cursor;	// 前の位置
		if (getInputState(inputNS::TRIGGER, BUTTON_UP, inputNS::NO1)){
			curPlayer1Cursor = (curPlayer1Cursor + (BOX_DIV_X * (BOX_DIV_Y - 1))) % (BOX_DIV_X * BOX_DIV_Y);
		}
		else if (getInputState(inputNS::TRIGGER, BUTTON_DOWN, inputNS::NO1)){
			curPlayer1Cursor = (curPlayer1Cursor + BOX_DIV_X) % (BOX_DIV_X * BOX_DIV_Y);
		}
		if (getInputState(inputNS::TRIGGER, BUTTON_LEFT, inputNS::NO1)){
			curPlayer1Cursor = ((curPlayer1Cursor + BOX_DIV_X - 1) % BOX_DIV_X) + ((curPlayer1Cursor / BOX_DIV_X) * BOX_DIV_X);
		}
		else if (getInputState(inputNS::TRIGGER, BUTTON_RIGHT, inputNS::NO1)){
			curPlayer1Cursor = ((curPlayer1Cursor + 1) % BOX_DIV_X) + ((curPlayer1Cursor / BOX_DIV_X) * BOX_DIV_X);
		}

		// 動いた場合
		if (prevCursor != curPlayer1Cursor){
			playSound(labelSelectSE);
		}

		// 決定
		if (getInputState(inputNS::TRIGGER, BUTTON_2, inputNS::NO1)){
			// 仮)
			if (curPlayer1Cursor == CHAR_DOTS || curPlayer1Cursor == CHAR_RANDOM){
				isPlayer1Selected = true;
				polyPlayer1Cursor.anmCnt = 0;	// アニメーションをとめる
				playSound(labelDecideSE);
			}
		}
	}
	else{
		// キャンセル機能
		if (getInputState(inputNS::TRIGGER, BUTTON_1, inputNS::NO1)){
			isPlayer1Selected = false;
			playSound(labelCancelSE);
		}
	}

	if (!isPlayer1Selected)
		polyPlayer1Cursor.updatePolygon();

	char tmpCursorX;
	char tmpCursorY;
	// p1側
	tmpCursorX = (curPlayer1Cursor % BOX_DIV_X);
	tmpCursorY = (curPlayer1Cursor / BOX_DIV_X);
	polyPlayer1Cursor.setData
		(BOX_BASE_X + (tmpCursorX * BOX_WIDTH) + (BOX_GAP_WIDTH * tmpCursorX), BOX_BASE_Y + (BOX_HEIGHT * tmpCursorY) + (BOX_GAP_HEIGHT * tmpCursorY),
		BOX_WIDTH, BOX_HEIGHT);

}

void CharacterSelect::updatePlayer2Cursor(){
	// p2
	if (isPlayer2Selected == false){
		int prevCursor = curPlayer2Cursor;	// 前の位置
		if (getInputState(inputNS::TRIGGER, BUTTON_UP, inputNS::NO2)){
			curPlayer2Cursor = (curPlayer2Cursor + (BOX_DIV_X * (BOX_DIV_Y - 1))) % (BOX_DIV_X * BOX_DIV_Y);
		} else if (getInputState(inputNS::TRIGGER, BUTTON_DOWN, inputNS::NO2)){
			curPlayer2Cursor = (curPlayer2Cursor + BOX_DIV_X) % (BOX_DIV_X * BOX_DIV_Y);
		}
		if (getInputState(inputNS::TRIGGER, BUTTON_LEFT, inputNS::NO2)){
			curPlayer2Cursor = ((curPlayer2Cursor + BOX_DIV_X - 1) % BOX_DIV_X) + ((curPlayer2Cursor / BOX_DIV_X) * BOX_DIV_X);
		} else if (getInputState(inputNS::TRIGGER, BUTTON_RIGHT, inputNS::NO2)){
			curPlayer2Cursor = ((curPlayer2Cursor + 1) % BOX_DIV_X) + ((curPlayer2Cursor / BOX_DIV_X) * BOX_DIV_X);
		}

		// 動いた場合
		if (prevCursor != curPlayer2Cursor){
			playSound(labelSelectSE);
		}

		// 決定
		if (getInputState(inputNS::TRIGGER, BUTTON_2, inputNS::NO2)){
			// 仮)
			if (curPlayer2Cursor == CHAR_DOTS || curPlayer2Cursor == CHAR_RANDOM){
				isPlayer2Selected = true;
				polyPlayer2Cursor.anmCnt = 0;	// アニメーションをとめる
				playSound(labelDecideSE);
			}
		}
	}
	else{
		// キャンセル機能
		if (getInputState(inputNS::TRIGGER, BUTTON_1, inputNS::NO2)){
			isPlayer2Selected = false;
			playSound(labelCancelSE);
		}
	}

	if (!isPlayer2Selected)	
		polyPlayer2Cursor.updatePolygon();


	// 操作を座標に反映
	char tmpCursorX;
	char tmpCursorY;
	// p2側
	tmpCursorX = (curPlayer2Cursor % BOX_DIV_X);
	tmpCursorY = (curPlayer2Cursor / BOX_DIV_X);
	polyPlayer2Cursor.setData
		(BOX_BASE_X + (tmpCursorX * BOX_WIDTH) + (BOX_GAP_WIDTH * tmpCursorX), BOX_BASE_Y + (BOX_HEIGHT * tmpCursorY) + (BOX_GAP_HEIGHT * tmpCursorY),
		BOX_WIDTH, BOX_HEIGHT);
}