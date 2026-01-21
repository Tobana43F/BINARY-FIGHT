#include	"GameReady.h"
#include	"AllCharacterHeader.h"	// キャラクター専用のクラスを確保するため
#include	"AllStageHeader.h"		// ステージ専用クラスの確保
#include	"BattleConfigInfo.h"
#include	"constCharacterList.h"
#include	"Battle.h"				// クラスの確保

#include	"ShareBaseFunc.h"		// 入力・文字列
#include	"Common.h"				// グローバル変数
#include	"Constants.h"			// 画面サイズ

#include	<stdlib.h>

//==========================================================
// GameReadyクラス
//----------------------------------------------------------
// コンストラクタ
GameReady::GameReady(BattleConfigInfo *_pBattleConfigInfo):
VS_POLY_WIDTH(300),
VS_POLY_HEIGHT(300),
VS_POLY_PLAYER_INIT_Y(WINDOW_HEIGHT * 0.4f - VS_POLY_HEIGHT / 2.0f),
VS_POLY_PLAYER1_INIT_X(-VS_POLY_WIDTH),	// ちょうど画面外
VS_POLY_PLAYER2_INIT_X(WINDOW_WIDTH),	// ちょうど画面外
VS_POLY_PLAYER1_TARGET_X(WINDOW_WIDTH * 0.45f - VS_POLY_WIDTH),
VS_POLY_PLAYER2_TARGET_X(WINDOW_WIDTH * 0.55f),
NOW_LOADING_WIDTH(300),
NOW_LOADING_HEIGHT(80),
NOW_LOADING_X(WINDOW_WIDTH * 0.95f - NOW_LOADING_WIDTH),
NOW_LOADING_Y(WINDOW_HEIGHT * 0.95f - NOW_LOADING_HEIGHT),
FONT_V_WIDTH(200),
FONT_V_HEIGHT(200),
FONT_INIT_V_X(WINDOW_WIDTH * 0.555f - FONT_V_WIDTH),
FONT_INIT_V_Y(WINDOW_HEIGHT * 0.43f - FONT_V_HEIGHT / 2.0f),
FONT_S_WIDTH(200),
FONT_S_HEIGHT(200),
FONT_INIT_S_X(WINDOW_WIDTH * 0.445f),
FONT_INIT_S_Y(WINDOW_HEIGHT * 0.53f - FONT_S_HEIGHT / 2.0f)
{
	isSceneEnd = false;

	pBattleConfigInfo = _pBattleConfigInfo;		// 自クラスないで使えるように、値をもらっておく

	//==========================================================
	// メモリの確保など
	// キャラクター
	allocCharacter(&pPlayer1, 1);
	allocCharacter(&pPlayer2, 2);

	// 相手の情報への参照を設定する
	pPlayer1->setOpponentActorPointer(pPlayer2);
	pPlayer2->setOpponentActorPointer(pPlayer1);

	// ステージ
	allocStage();	// 上位クラスのポインタに、それぞれのクラスの実態を確保する

	pCharacterShadow = new CharacterShadow(pPlayer1, pPlayer2, &pStage->groundPosY);	// キャラの影
	pTimerUI = new TimerUI(pBattleConfigInfo);				// タイマー
	pHitPointBarUI = new HitPointBarUI(pPlayer1, pPlayer2);	// HPバー
	pRoundIconUI = new RoundIconUI(pBattleConfigInfo);		// ラウンドアイコン
	pComboUI = new ComboUI(pPlayer1, pPlayer2);				// コンボ
	pBattleEffect = new BattleEffect;						// エフェクト
	pRoundStartEffect = new RoundStartEffect(&pRoundIconUI->curRound, &pRoundIconUI->maxRound);// ラウンドコール
	pRoundKOEffect = new RoundKOEffect;						// KO時のエフェクト
	pRoundEndEffect = new RoundEndEffect(pTimerUI, pRoundIconUI, pPlayer1, pPlayer2);	// ラウンド終了時
	pBattleEndEffect = new BattleEndEffect(pRoundIconUI);

	//==========================================================
	// このクラスの初期化
	// クラス内の遷移を初期化
	gameReadyScene = VERSUS_DRAW;
	
	// テクスチャ
	pTextureLoader = new TextureLoader(MAX_TEX);
	pTextureLoader->setUseTextureLabel(NOW_LOADING_STRING, textureNS::GENERAL_NOW_LOADING_STRING);
	pTextureLoader->setUseTextureLabel(FONT_V, textureNS::GAME_READY_FONT_V);
	pTextureLoader->setUseTextureLabel(FONT_S, textureNS::GAME_READY_FONT_S);
	pTextureLoader->setUseTextureLabel(BACK, textureNS::GAME_READY_BACK);
	pTextureLoader->loadUseTexture();	// ここで読み込んでしまう。

	// サウンド
	labelStart = soundNS::GAMEREADY_START_SE;
	labelEnd = soundNS::GAMEREADY_END_SE;

	// p1
	polyVSPlayer1.setData(
		VS_POLY_PLAYER1_INIT_X, VS_POLY_PLAYER_INIT_Y, 
		VS_POLY_WIDTH, VS_POLY_HEIGHT);
	polyVSPlayer1.setTexturePointer(pPlayer1->pTextureLoaderForGameReady->getTexturePointer(Actor::VS_TEX));

	// p2
	polyVSPlayer2.setData(
		VS_POLY_PLAYER2_INIT_X, VS_POLY_PLAYER_INIT_Y,
		VS_POLY_WIDTH, VS_POLY_HEIGHT);
	polyVSPlayer2.setTexturePointer(pPlayer2->pTextureLoaderForGameReady->getTexturePointer(Actor::VS_TEX));
	polyVSPlayer2.setIsReverseX(true);

	// nowLoading用
	polyNowLoading.setData(NOW_LOADING_X, NOW_LOADING_Y, NOW_LOADING_WIDTH, NOW_LOADING_HEIGHT);
	polyNowLoading.setTexturePointer(pTextureLoader->getTexturePointer(NOW_LOADING_STRING));

	// VS
	// V
	polyFontV.setData(FONT_INIT_V_X, FONT_INIT_V_Y, FONT_V_WIDTH, FONT_V_HEIGHT);
	polyFontV.setTexturePointer(pTextureLoader->getTexturePointer(FONT_V));
	// S
	polyFontS.setData(FONT_INIT_S_X, FONT_INIT_S_Y, FONT_S_WIDTH, FONT_S_HEIGHT);
	polyFontS.setTexturePointer(pTextureLoader->getTexturePointer(FONT_S));

	isVSInDrawEnd = false;
	isVSOutDrawEnd = false;
}

//----------------------------------------------------------
// デストラクタ
GameReady::~GameReady(){
	// このクラスで確保したものは解放する
	delete pTextureLoader;
	pTextureLoader = NULL;

	// このクラスでは確保したメモリを使わないため、渡した後は、NULLとする。
	pBattleConfigInfo = NULL;
	pPlayer1 = NULL;
	pPlayer2 = NULL; 
	pCharacterShadow = NULL;
	pStage = NULL;
	pTimerUI = NULL;
	pRoundIconUI = NULL;
	pHitPointBarUI = NULL;
	pComboUI = NULL;
	pBattleEffect = NULL;
	pRoundStartEffect = NULL;
	pRoundKOEffect = NULL;
	pRoundEndEffect = NULL;
	pBattleEndEffect = NULL;
}

//----------------------------------------------------------
// キャラクター専用のクラスを確保
void GameReady::allocCharacter(Actor **_pPlayer, char _playerNo){
	eCharacterList tmpCharList;
	ePlayerID tmpPlayerID;

	// プレイヤー１、２の判別
	if (_playerNo == 1){
		tmpCharList = pBattleConfigInfo->getPlayer1SelectedCharacter();
		tmpPlayerID = pBattleConfigInfo->getPlayer1ID();
	}
	else if (_playerNo == 2){
		tmpCharList = pBattleConfigInfo->getPlayer2SelectedCharacter();
		tmpPlayerID = pBattleConfigInfo->getPlayer2ID();
	}

	// ランダムだった場合
	if (tmpCharList == CHARACTER_LIST_RANDOM){
		// ランダムで確保
		tmpCharList = (eCharacterList)(rand() % (CHARACTER_LIST_RANDOM));
		pBattleConfigInfo->setSelectCharacter(_playerNo, tmpCharList);	// ランダム以外のキャラクターを指定する
	}

	// クラスの確保
	switch (tmpCharList){
		case CHARACTER_LIST_DOTS:
			if (tmpPlayerID != PLAYERID_CPU){ *_pPlayer = new CharDots; }
			else { *_pPlayer = new CharDotsCPU; }
			break;

		case CHARACTER_LIST_DOTS_FOR_TUTORIAL:
			*_pPlayer = new CharDotsForTutorial(pModeTutorial);	// 仮)チュートリアルの情報を渡したかった(無理やりになってしまった。)
			break;
	}
}

void GameReady::setPlayerID(Actor *_pPlayer, ePlayerID _playerID){
	switch (_playerID){
		case PLAYERID_PLAYER1:
			_pPlayer->battleInput.setUseControllerNo(inputNS::NO1);
			break;
		case PLAYERID_PLAYER2:
			_pPlayer->battleInput.setUseControllerNo(inputNS::NO2);
			break;
		case PLAYERID_CPU:
			_pPlayer->battleInput.setUseControllerNo(inputNS::PROGRAM);
			break;
	}
}

//----------------------------------------------------------
// 選択されたステージクラスの確保(プレイヤー確保後に)
void GameReady::allocStage(void){
	eStageList tmpStageList = pBattleConfigInfo->getSelectedStage();

	// ランダムだった場合
	if (tmpStageList == STAGE_LIST_RANDOM){
		// ランダムで確保
		tmpStageList = (eStageList)(rand() % (STAGE_LIST_RANDOM));
		pBattleConfigInfo->setSelectStage(tmpStageList);
	}

	// クラスの確保
	switch (pBattleConfigInfo->getSelectedStage()){
		case STAGE_LIST_TRAINING:
			pStage = new StageTraining;	// 仮)
			break;

		case STAGE_LIST_SKY_PLACE:
			pStage = new StageSkyPlace;
			break;
	}

	// 参照プレイヤーの設定
	pStage->initPlayerPointer(pPlayer1, pPlayer2);
}

//----------------------------------------------------------
// ゲーム前のすべての初期化
void GameReady::initBeforeGame(){
	// プレイヤー初期化
	setPlayerID(pPlayer1, pBattleConfigInfo->getPlayer1ID());
	setPlayerID(pPlayer2, pBattleConfigInfo->getPlayer2ID());

	pPlayer1->init();
	pPlayer1->initCharacter();
	pPlayer2->init();
	pPlayer2->initCharacter();

	// ステージの初期化
	pStage->init();
	pStage->initStage();

	// 影の初期化
	pCharacterShadow->init();

	// UI初期化
	pTimerUI->init();
	pHitPointBarUI->init();
	pRoundIconUI->init();
	// Comboは初期化は特にない
	pBattleEffect->init();
	pRoundStartEffect->init();
	pRoundKOEffect->init();
	pRoundEndEffect->init();
	pBattleEndEffect->init();
}

//----------------------------------------------------------
// テクスチャのロード
void GameReady::load(){
	// プレイヤーのテクスチャロード
	pPlayer1->load();
	pPlayer2->load();

	pStage->load();

	pCharacterShadow->load();

	pHitPointBarUI->load();
	pTimerUI->load();
	pRoundIconUI->load();
	pComboUI->load();
	pBattleEffect->load();
	pRoundStartEffect->load();
	pRoundKOEffect->load();
	pRoundEndEffect->load();
	pBattleEndEffect->load();
}

//----------------------------------------------------------
// VS画面の入り
// 描画
void GameReady::drawVSIn(){
	polyVSPlayer1.drawPolygon();
	polyVSPlayer2.drawPolygon();
}
// 更新
void GameReady::updateVSIn(){
	// 仮) この辺全て
	bool isPlayer1ReachTarget = false;
	bool isPlayer2ReachTarget = false;

	polyVSPlayer1.coord.x += 5.0f;
	polyVSPlayer2.coord.x -= 5.0f;

	if (polyVSPlayer1.coord.x >= VS_POLY_PLAYER1_TARGET_X){
		polyVSPlayer1.coord.x = VS_POLY_PLAYER1_TARGET_X;
		isPlayer1ReachTarget = true;
	}
	if (polyVSPlayer2.coord.x <= VS_POLY_PLAYER2_TARGET_X){
		polyVSPlayer2.coord.x = VS_POLY_PLAYER2_TARGET_X;
		isPlayer2ReachTarget = true;
	}

	// 画面遷移
	if (isPlayer1ReachTarget && isPlayer2ReachTarget){
		gameReadyScene = LOADING;
		playSound(labelStart);
	}
}

//----------------------------------------------------------
// VS画面の終わり(ロード後の演出)
// 描画
void GameReady::drawVSOut(){
	polyVSPlayer1.drawPolygon();
	polyVSPlayer2.drawPolygon();
}
// 更新
void GameReady::updateVSOut(){
	bool isPlayer1ReachTarget = false;
	bool isPlayer2ReachTarget = false;


	polyVSPlayer1.coord.x -= 50.0f;
	polyVSPlayer2.coord.x += 50.0f;

	if (polyVSPlayer1.coord.x <= VS_POLY_PLAYER1_INIT_X){
		polyVSPlayer1.coord.x = VS_POLY_PLAYER1_INIT_X;
		isPlayer1ReachTarget = true;
	}
	if (polyVSPlayer2.coord.x >= VS_POLY_PLAYER2_INIT_X){
		polyVSPlayer2.coord.x = VS_POLY_PLAYER2_INIT_X;
		isPlayer2ReachTarget = true;
	}

	// 画面遷移
	if (isPlayer1ReachTarget && isPlayer2ReachTarget){
		gameReadyScene = GAME_READY_END;
	}
}



//----------------------------------------------------------
// 描画
void GameReady::draw(){
	drawQuadPrimitive(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0xFF, 0xFF, 0xFF, pTextureLoader->getTexturePointer(BACK), 0);	// 背景表示

	switch (gameReadyScene){
	case VERSUS_DRAW:
		drawVSIn();

		polyFontV.drawPolygon();
		polyFontS.drawPolygon();

		break;

	case LOADING:
		drawVSIn(); // 描画内容は変わらない
		polyNowLoading.drawPolygon();

		polyFontV.drawPolygon();
		polyFontS.drawPolygon();

		break;

	case LOAD_COMPLETE:
		drawVSOut();

		polyFontV.drawPolygon();
		polyFontS.drawPolygon();

		break;

	case GAME_READY_END:
		break;

	}
}

//----------------------------------------------------------
// 更新
void GameReady::update(){
	switch (gameReadyScene){
	case VERSUS_DRAW:
		updateVSIn();

		break;

	case LOADING:
		load();		// 一度だけロードする
		gameReadyScene = LOAD_COMPLETE;
		playSound(labelEnd);
		break;

	case LOAD_COMPLETE:
		updateVSOut();
		break;

	case GAME_READY_END:
		updateGameScene();
		if (isSceneEnd)
			return;
		break;
	}
}

//----------------------------------------------------------
// ゲームシーンの更新
void GameReady::updateGameScene(){
	initBeforeGame();	// ゲーム開始前に初期化しておく
	isSceneEnd = true;
}