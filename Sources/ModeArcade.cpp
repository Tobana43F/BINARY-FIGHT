//==========================================================
// インクルード
#include	"ModeArcade.h"
#include	"BattleConfigInfo.h"

#include	"Common.h"	// gGameScene

#include	<stdlib.h>	// rand();

//==========================================================
// ModeArcade
//----------------------------------------------------------
// コンストラクタ
ModeArcade::ModeArcade():
GAME_MODE(ARCADE),
ROUND_TIME(LIMIT_60SEC),
ROUND_NUM(BEST_OF_THREE),
MAX_BATTLE_NUM(1)
{
	isSceneEnd = false;

	pBattle = NULL;
	pGameReady = NULL;
	pCharacterSelect = NULL;

	pBattleConfig = new BattleConfigInfo;
	pBattleConfig->setModeData(GAME_MODE);		// アーケードモード
	pBattleConfig->setNumOfRound(ROUND_NUM);
	pBattleConfig->setRoundTimeLimit(ROUND_TIME);

	// 最初に遷移する画面
	pCharacterSelect = new CharacterSelectForArcadeMode(pBattleConfig);

	init();
}

//----------------------------------------------------------
// デストラクタ
ModeArcade::~ModeArcade(){
	delete pBattleConfig;
	pBattleConfig = NULL;

	if (pBattle != NULL){ delete pBattle; pBattle = NULL; }
	if (pGameReady != NULL){ delete pGameReady; pGameReady = NULL; }
	if (pCharacterSelect != NULL){ delete pCharacterSelect; pCharacterSelect = NULL; }

}

//----------------------------------------------------------
// 初期化
void ModeArcade::init(){
	curBattleNum = 0;
	scene = CHARACTER_SELECT;

}

//----------------------------------------------------------
// 描画
void ModeArcade::draw(){
	switch (scene){
		case CHARACTER_SELECT:
			pCharacterSelect->draw();
			break;

		case AUTO_SETTING:
			break;

		case GAME_READY:
			pGameReady->draw();
			break;

		case BATTLE:
			pBattle->draw();
			break;

		case CONTINUE:
			break;

		case RESULT:
			break;
	}
}

//----------------------------------------------------------
// 更新
void ModeArcade::update(){
	switch (scene){
		case CHARACTER_SELECT:{
			pCharacterSelect->update();
			if (pCharacterSelect->getIsSceneEnd()){
				delete pCharacterSelect;
				pCharacterSelect = NULL;
				scene = AUTO_SETTING;
			}
			break;
		}
		case AUTO_SETTING:{
			// 仮 )関数か
			pBattleConfig->setSelectStage(STAGE_LIST_RANDOM);
			pBattleConfig->setSelectCharacter(
				pBattleConfig->getPlayer1SelectedCharacter(),	// プレイヤー１がプレイヤー
				CHARACTER_LIST_RANDOM
				);

			pGameReady = new GameReady(pBattleConfig);
			scene = GAME_READY;
			break;
		}
		case GAME_READY:{
			pGameReady->update();
			if (pGameReady->getIsSceneEnd()){
				pBattle = new Battle(pGameReady);	// GameReadyを先に渡す
				delete pGameReady;
				pGameReady = NULL;
				scene = BATTLE;
			}
			break;
		}
		case BATTLE:
			pBattle->update();
			//----------------------------------------------------------
			// 戦闘終了時の処理
			if (pBattle->getIsSceneEnd()){
				//----------------------------------------------------------
				// 負けたときの処理
				if (pBattle->pRoundIconUI->searchPlayer1NextIconSpace() != pBattle->pRoundIconUI->iconNum)
					scene = RESULT;	// とりあえず
				else
				{
					// 勝利
					//----------------------------------------------------------
					// 次の戦闘への遷移
					curBattleNum++;	// 次の戦闘へ
					if (curBattleNum < MAX_BATTLE_NUM)	// 連戦中
						scene = AUTO_SETTING;
					else
						scene = RESULT;
				}
				delete pBattle;
				pBattle = NULL;
			}
			break;

		case CONTINUE:
			pGameReady = new GameReady(pBattleConfig);	// 前回と同じ状態で戦闘を開始する
			scene = GAME_READY;
			break;
			
		case RESULT:
			scene = END;
			break;

		case END:
			isSceneEnd = true;
			break;
	}

	// 共通の処理
	interruptReturnToTitle();
}

//----------------------------------------------------------
// 割り込み的処理
void ModeArcade::interruptReturnToTitle(){
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