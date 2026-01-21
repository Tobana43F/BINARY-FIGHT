//==========================================================
// インクルード
#include	"ModeVersus.h"		// メソッド定義
#include	"Common.h"			// gGameScene

//==========================================================
// ModeVersusクラス
//----------------------------------------------------------
// コンストラクタ
ModeVersus::ModeVersus() :
GAME_MODE(VERSUS),
ROUND_TIME(LIMIT_60SEC),
ROUND_NUM(BEST_OF_THREE)
{
	isSceneEnd = false;

	pCharacterSelect = NULL;
	pGameReady = NULL;
	pBattle = NULL;

	// バトル情報の作成
	pBattleConfigInfo = new BattleConfigInfo;
	pBattleConfigInfo->setModeData(GAME_MODE);		// アーケードモード
	pBattleConfigInfo->setNumOfRound(ROUND_NUM);
	pBattleConfigInfo->setRoundTimeLimit(ROUND_TIME);

	// 最初の遷移
	pCharacterSelect = new CharacterSelect(pBattleConfigInfo);

	init();
}

//----------------------------------------------------------
// デストラクタ
ModeVersus::~ModeVersus(){
	delete pBattleConfigInfo;
	pBattleConfigInfo = NULL;

	if (pBattle != NULL){ delete pBattle; }
	if (pGameReady != NULL){ delete pGameReady; }
	if (pCharacterSelect != NULL){ delete pCharacterSelect; }
}

//----------------------------------------------------------
// 初期化
void ModeVersus::init(){
	scene = CHARACTER_SELECT;
}

//----------------------------------------------------------
// 描画
void ModeVersus::draw(){
	switch (scene){
		case CHARACTER_SELECT:
			pCharacterSelect->draw();
			break;

		case STAGE_SELECT:
			break;

		case GAME_READY:
			pGameReady->draw();
			break;

		case BATTLE:
			pBattle->draw();
			break;

		case MENU_AFTER_BATTLE:
			break;
	}
}

//----------------------------------------------------------
// 更新
void ModeVersus::update(){
	switch (scene){
		case CHARACTER_SELECT:
			pCharacterSelect->update();
			if (pCharacterSelect->getIsSceneEnd()){
				delete pCharacterSelect;
				pCharacterSelect = NULL;
				scene = STAGE_SELECT;
			}
			break;

		case STAGE_SELECT:
			pBattleConfigInfo->setSelectStage(STAGE_LIST_RANDOM);
			pGameReady = new GameReady(pBattleConfigInfo);
			scene = GAME_READY;
			break;

		case GAME_READY:
			pGameReady->update();
			if (pGameReady->getIsSceneEnd()){
				pBattle = new Battle(pGameReady);
				delete pGameReady;
				pGameReady = NULL;
				scene = BATTLE;
			}
			break;

		case BATTLE:
			pBattle->update();
			if (pBattle->getIsSceneEnd())
				scene = MENU_AFTER_BATTLE;
			break;

		case MENU_AFTER_BATTLE:
			isSceneEnd = true;
			break;
	}

	interruptReturnToTitle();
}

//----------------------------------------------------------
// 割り込み的処理
void ModeVersus::interruptReturnToTitle(){
	// 中断すると困る場所は例外を定めておく
	if (scene == GAME_READY)
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