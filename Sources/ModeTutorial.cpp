#include	"ModeTutorial.h"	// メソッド定義
#include	"ShareBaseFunc.h"
#include	"Common.h"
#include	<math.h>	// abs()

//==========================================================
// StringClearクラス
//----------------------------------------------------------
// コンストラクタ
StringClear::StringClear() : 
WIDTH(WINDOW_WIDTH * 0.5f),
HEIGHT(200),
INIT_X(WINDOW_WIDTH + WIDTH),
INIT_Y(WINDOW_HEIGHT / 2.0f - HEIGHT / 2.0f),
STOP_POINT_X(WINDOW_WIDTH / 2.0f - WIDTH / 2.0f),
STOP_POINT_STOP_CNT(30),
END_POINT_X(0 - WIDTH),
MAX_SPEED_X(-30)	// 左へ動かす
{
	isSceneEnd = false;
	pTextureLoader = new TextureLoader(MAX_TEX);
	pTextureLoader->setUseTextureLabel(STRING_CLEAR, textureNS::TUTORIAL_CLEAR);
}

//----------------------------------------------------------
// デストラクタ
StringClear::~StringClear(){

}

//----------------------------------------------------------
// 初期化
void StringClear::init(){
	isSceneEnd = false;
	scene = INIT;

	cnt = 0;
	cntStart = cnt;

	poly.setData(INIT_X, INIT_Y, WIDTH, HEIGHT);
	poly.setTexturePointer(pTextureLoader->getTexturePointer(STRING_CLEAR));
	curSpeedX = MAX_SPEED_X;
}

//----------------------------------------------------------
// ロード
void StringClear::load(){
	pTextureLoader->loadUseTexture();
}

//----------------------------------------------------------
// 描画
void StringClear::draw(){
	switch (scene){
		case INIT:
			break;

		case START_POINT:
			poly.drawPolygon();
			break;

		case STOP_POINT:
			poly.drawPolygon();
			break;

		case END_POINT:
			poly.drawPolygon();
			break;

		case END:
			break;
	}
}

//----------------------------------------------------------
// 更新
void StringClear::update(){
	switch (scene){
		case INIT:
			init();
			scene = START_POINT;
			break;

		case START_POINT:
			updateStartPoint();
			break;

		case STOP_POINT:
			updateStopPoint();
			break;

		case END_POINT:
			updateEndPoint();
			break;

		case END:
			break;
	}
	cnt++;
}

//----------------------------------------------------------
// 遷移時のカウンタ更新
void StringClear::updateCntStart(){
	cntStart = cnt;
}

//----------------------------------------------------------
// 各シーンごとの更新
void StringClear::updateStartPoint(){
	curSpeedX--;
	if (curSpeedX > 0 ){
		curSpeedX = -1;	// ゆっくりにする
	}
	poly.coord.x += curSpeedX;

	if (poly.coord.x <= STOP_POINT_X){
		poly.coord.x = STOP_POINT_X; 
		updateCntStart();
		scene = STOP_POINT;
	}
}

void StringClear::updateStopPoint(){
	if (cnt - cntStart > STOP_POINT_STOP_CNT){
		updateCntStart();
		scene = END_POINT;
		curSpeedX = 10;	// 初期化する
	}
}

void StringClear::updateEndPoint(){
	curSpeedX--;
	poly.coord.x += curSpeedX;

	if (poly.coord.x <= END_POINT_X){
		scene = END;
		isSceneEnd = true;
	}
}

//==========================================================
// ModeTutorial
//----------------------------------------------------------
// コンストラクタ
ModeTutorial::ModeTutorial() :
GAME_MODE(TUTORIAL),
ROUND_TIME(NO_LIMIT),
ROUND_NUM(BEST_OF_ONE),
PLAYER_1_CHARACTER(CHARACTER_LIST_DOTS),
PLAYER_2_CHARACTER(CHARACTER_LIST_DOTS_FOR_TUTORIAL),
PLAYER_1_ID(PLAYERID_PLAYER1),
PLAYER_2_ID(PLAYERID_CPU),
STAGE(STAGE_LIST_TRAINING),
TITLE_BAR_WIDTH(WINDOW_WIDTH *0.8f),
TITLE_BAR_HEIGHT(100),
TITLE_BAR_X(WINDOW_WIDTH / 2.0f - TITLE_BAR_WIDTH / 2.0f),
TITLE_BAR_Y(0),
TITLE_BAR_CAP_WIDTH(WINDOW_WIDTH * 0.1f),
TITLE_BAR_CAP_HEIGHT(TITLE_BAR_HEIGHT),
TITLE_BAR_CAP_Y(TITLE_BAR_Y),
TITLE_BAR_CAP_LEFT_X(TITLE_BAR_X - TITLE_BAR_CAP_WIDTH),
TITLE_BAR_CAP_RIGHT_X(TITLE_BAR_X + TITLE_BAR_WIDTH - 1),
DESCRIPTION_BAR_WIDTH(WINDOW_WIDTH * 0.8f),
DESCRIPTION_BAR_HEIGHT(64),
DESCRIPTION_BAR_X(WINDOW_WIDTH / 2.0f - DESCRIPTION_BAR_WIDTH / 2.0f),
DESCRIPTION_BAR_Y(WINDOW_HEIGHT - DESCRIPTION_BAR_HEIGHT),
DESCRIPTION_BAR_CAP_WIDTH(WINDOW_WIDTH * 0.1f),
DESCRIPTION_BAR_CAP_HEIGHT(DESCRIPTION_BAR_HEIGHT),
DESCRIPTION_BAR_CAP_Y(DESCRIPTION_BAR_Y),
DESCRIPTION_BAR_CAP_LEFT_X(DESCRIPTION_BAR_X - DESCRIPTION_BAR_CAP_WIDTH),
DESCRIPTION_BAR_CAP_RIGHT_X(DESCRIPTION_BAR_X + DESCRIPTION_BAR_WIDTH - 1)
{
	isSceneEnd = false;
	scene = AUTO_SETTING;

	//----------------------------------------------------------
	// テクスチャ
	pTextureLoader = new TextureLoader(MAX_TEX);
	pTextureLoader->setUseTextureLabel(BAR, textureNS::TUTORIAL_BAR);
	pTextureLoader->setUseTextureLabel(BAR_CAP, textureNS::TUTORIAL_BAR_CAP);

	//----------------------------------------------------------
	// バトル情報の設定(全て固定にしてある)
	pBattleConfigInfo = new BattleConfigInfo;
	pBattleConfigInfo->setModeData(GAME_MODE);
	pBattleConfigInfo->setRoundTimeLimit(ROUND_TIME);
	pBattleConfigInfo->setNumOfRound(ROUND_NUM);
	pBattleConfigInfo->setSelectCharacter(PLAYER_1_CHARACTER, PLAYER_2_CHARACTER);
	pBattleConfigInfo->setPlayerID(1, PLAYER_1_ID);
	pBattleConfigInfo->setPlayerID(2, PLAYER_2_ID);
	pBattleConfigInfo->setSelectStage(STAGE);

	//----------------------------------------------------------
	// チュートリアルクラスを確保
	pTutorialStepMove = new TutorialStepMove;
	pTutorialStepJump = new TutorialStepJump;
	pTutorialStepAttack = new TutorialStepAttack;
	pTutorialStepGuardHighPos = new TutorialStepGuardHighPos;
	pTutorialStepGuardLowPos = new TutorialStepGuardLowPos;
	pTutorialStepGuardMidPos = new TutorialStepGuardMidPos;
	pTutorialStepBattle = new TutorialStepBattle;


	pTutorialStep = new AbstractTutorialStep*[MAX_STEP];	// まとめ
	pTutorialStep[STEP_MOVE] = pTutorialStepMove;
	pTutorialStep[STEP_JUMP] = pTutorialStepJump;
	pTutorialStep[STEP_ATTACK] = pTutorialStepAttack;
	pTutorialStep[STEP_GUARD_HIGH_POS] = pTutorialStepGuardHighPos;
	pTutorialStep[STEP_GUARD_LOW_POS] = pTutorialStepGuardLowPos;
	pTutorialStep[STEP_GUARD_MID_POS] = pTutorialStepGuardMidPos;
	pTutorialStep[STEP_BATTLE] = pTutorialStepBattle;


}

//----------------------------------------------------------
// デストラクタ
ModeTutorial::~ModeTutorial(){
	safeDelete(pTextureLoader);

	safeDelete(pTutorialStepMove);
	safeDelete(pTutorialStepJump);
	safeDelete(pTutorialStepAttack);
	safeDelete(pTutorialStepGuardHighPos);
	safeDelete(pTutorialStepGuardLowPos);
	safeDelete(pTutorialStepGuardMidPos);
	safeDelete(pTutorialStepBattle);

	if (pTutorialStep != NULL){
		delete[] pTutorialStep;
		pTutorialStep = NULL;
	}

	safeDelete(pBattleConfigInfo);
	safeDelete(pGameReady);
	safeDelete(pBattle);
}

//----------------------------------------------------------
// 初期化
void ModeTutorial::init(){
	curStep = STEP_MOVE;
	cnt = 0;
	cntStart = cnt;


	// ポリゴンの設定
	polyTitleBar.setData(TITLE_BAR_X, TITLE_BAR_Y, TITLE_BAR_WIDTH, TITLE_BAR_HEIGHT);
	polyTitleBar.setTexturePointer(pTextureLoader->getTexturePointer(BAR));
	polyTitleBarCapLeft.setData(TITLE_BAR_CAP_LEFT_X, TITLE_BAR_CAP_Y, TITLE_BAR_CAP_WIDTH, TITLE_BAR_CAP_HEIGHT);
	polyTitleBarCapLeft.setTexturePointer(pTextureLoader->getTexturePointer(BAR_CAP));
	polyTitleBarCapRight.setData(TITLE_BAR_CAP_RIGHT_X, TITLE_BAR_CAP_Y, TITLE_BAR_CAP_WIDTH, TITLE_BAR_CAP_HEIGHT);
	polyTitleBarCapRight.setTexturePointer(pTextureLoader->getTexturePointer(BAR_CAP));
	polyTitleBarCapRight.setIsReverseX(true);

	polyDescriptionBar.setData(DESCRIPTION_BAR_X, DESCRIPTION_BAR_Y, DESCRIPTION_BAR_WIDTH, DESCRIPTION_BAR_HEIGHT);
	polyDescriptionBar.setTexturePointer(pTextureLoader->getTexturePointer(BAR));
	polyDescriptionBarCapLeft.setData(DESCRIPTION_BAR_CAP_LEFT_X, DESCRIPTION_BAR_CAP_Y, DESCRIPTION_BAR_CAP_WIDTH, DESCRIPTION_BAR_CAP_HEIGHT);
	polyDescriptionBarCapLeft.setTexturePointer(pTextureLoader->getTexturePointer(BAR_CAP));
	polyDescriptionBarCapRight.setData(DESCRIPTION_BAR_CAP_RIGHT_X, DESCRIPTION_BAR_CAP_Y, DESCRIPTION_BAR_CAP_WIDTH, DESCRIPTION_BAR_CAP_HEIGHT);
	polyDescriptionBarCapRight.setTexturePointer(pTextureLoader->getTexturePointer(BAR_CAP));
	polyDescriptionBarCapRight.setIsReverseX(true);

	stringClear.init();
}

//----------------------------------------------------------
// 初期化
void ModeTutorial::load(){
	pTextureLoader->loadUseTexture();
}

//----------------------------------------------------------
// バトルの例外処理
void ModeTutorial::initBattleHitPoint(){
	if (pBattle == NULL)
		return;

	pBattle->player[0]->curHitPoint = pBattle->player[0]->maxHitPoint;

	if (curStep != STEP_BATTLE){	// 相手をKOできる状態にする
		pBattle->player[1]->curHitPoint = pBattle->player[1]->maxHitPoint;
	}
}

//----------------------------------------------------------
// 描画
void ModeTutorial::draw(){
	switch (scene){
		case AUTO_SETTING:
			break;

		case GAME_READY:
			pGameReady->draw();
			break;

		case STEP:
			pBattle->draw();
			drawBar();
			
			break;

		case CLEAR_STEP:
			pBattle->draw();
			drawBar();
			stringClear.draw();
			break;

		case NEXT_STEP:
			pBattle->draw();
			drawBar();
			break;

		case ALL_CLEAR:
			pBattle->draw();
			break;

		case END:
			break;
	}
}

//----------------------------------------------------------
// タイトルなどのバーの表示
void ModeTutorial::drawBar(){
	if (curStep == STEP_BATTLE &&	// バトル中は邪魔になるので、数秒後に消す
		cnt - cntStart >= 300)
		return;	
	
	polyTitleBar.drawPolygon();
	polyTitleBarCapLeft.drawPolygon();
	polyTitleBarCapRight.drawPolygon();

	polyDescriptionBar.drawPolygon();
	polyDescriptionBarCapLeft.drawPolygon();
	polyDescriptionBarCapRight.drawPolygon();

	pTutorialStep[curStep]->draw();
}

//----------------------------------------------------------
// 更新
void ModeTutorial::update(){
	switch (scene){
		case AUTO_SETTING:
			pGameReady = new GameReady(pBattleConfigInfo);
			scene = GAME_READY;
			updateCounterStart();
			break;

		case GAME_READY:
			pGameReady->update();
			if (pGameReady->getIsSceneEnd()){
				pBattle = new Battle(pGameReady);
				safeDelete(pGameReady);

				load();	// タイトルバーなどのロード
				init();
				// ステップにバトル情報を渡す
				for (int i = 0; i < MAX_STEP; ++i){
					pTutorialStep[i]->load();
					pTutorialStep[i]->init();
					pTutorialStep[i]->setBattlePointer(pBattle);
				}

				stringClear.load();	// ロード
				stringClear.init();

				scene = STEP;
				updateCounterStart();
			}
			break;

		case STEP:
			pBattle->update();
			initBattleHitPoint();

			pTutorialStep[curStep]->update();
			if (pTutorialStep[curStep]->getIsStepCleard()){
				scene = CLEAR_STEP;
				stringClear.init();
				updateCounterStart();
			}
			break;

		case CLEAR_STEP:	
			pBattle->update();
			initBattleHitPoint();
			pTutorialStep[curStep]->update();
			// クリアの表示
			stringClear.update();
			// クリアの表示が終わったら
			if (stringClear.getIsSceneEnd()){
				scene = NEXT_STEP;
				updateCounterStart();
			}
			break;

		case NEXT_STEP:
			pBattle->update();
			initBattleHitPoint();
			// 次のステップへ
			curStep++;	// 次へ
			// 終了判断
			if (curStep < MAX_STEP){
				scene = STEP;
				updateCounterStart();
			}
			else{
				scene = ALL_CLEAR;
				updateCounterStart();
			}

			break;

		case ALL_CLEAR:
			// チュートリアルクリア！
			pBattle->update();
			// 描画終了で
			if (pBattle->getIsSceneEnd()){
				scene = END;
				updateCounterStart();
			}

			break;

		case END:
			isSceneEnd = true;
			break;
	}
	cnt++;

	// 共通の処理
	interruptReturnToTitle();
}

//----------------------------------------------------------
// 割り込み的処理
void ModeTutorial::interruptReturnToTitle(){
	// 中断すると困る場所は例外を定めておく
	if (scene == GAME_READY ||
		scene == AUTO_SETTING)
		return;

	// タイトルに戻る
	if (getInputState(inputNS::HOLD, BUTTON_PLUS, inputNS::NO1) &&
		getInputState(inputNS::HOLD, BUTTON_MINUS, inputNS::NO1) ||
		getInputState(inputNS::HOLD, BUTTON_PLUS, inputNS::NO2) &&
		getInputState(inputNS::HOLD, BUTTON_MINUS, inputNS::NO2)){
		// 開放処理は、遷移後に行う
		gGameScene = RETURN_TO_TITLE;
	}
}