#include	"CharDotsForTutorial.h"
#include	<stdlib.h>


//==========================================================
// CharDotsForTutorialクラス
//----------------------------------------------------------
// コンストラクタ
CharDotsForTutorial::CharDotsForTutorial(ModeTutorial *_pModeTutorial):
INPUT_RESET_CNT(360)
{
	pModeTutorial = _pModeTutorial;

	cnt = 0;
	cntStart = 0;
}

//----------------------------------------------------------
// デストラクタ
CharDotsForTutorial::~CharDotsForTutorial(){
	pModeTutorial = NULL;	// 参照を解除
}

//----------------------------------------------------------
// 行動の決定
void CharDotsForTutorial::updateCPU(){
	int curStep = pModeTutorial->getCurStep();

	if (curStep == ModeTutorial::STEP_MOVE || 
		curStep == ModeTutorial::STEP_JUMP ||
		curStep == ModeTutorial::STEP_ATTACK )
	{
		// 何もしない
	}
	else if (curStep == ModeTutorial::STEP_GUARD_HIGH_POS){
		updateGuardHighPosStep();
	}
	else if (curStep == ModeTutorial::STEP_GUARD_LOW_POS){
		updateGuardLowPosStep();
	}
	else if (curStep == ModeTutorial::STEP_GUARD_MID_POS){
		updateGuardMidPosStep();
	}
	else if (curStep == ModeTutorial::STEP_BATTLE){
		updateBattle();
	}


}

//----------------------------------------------------------
// キャラクター操作
void CharDotsForTutorial::operateActor(){
	updateCPU();

	CharDots::operateActor();

	cnt++;
}

//----------------------------------------------------------
// 上段ガードステップ時の操作
void CharDotsForTutorial::updateGuardHighPosStep(){
	int walkFrame = 70;

	// バグ防止
	if (cnt % INPUT_RESET_CNT == 0){
		battleInput.setDirectionKey(BattleInputData::DIR_5);
		battleInput.setButtonData(BATTLE_BUTTON_1, false);
		battleInput.setButtonData(BATTLE_BUTTON_2, false);
	}



	if (getCurDirState() == RIGHT){
		if ((cnt - cntStart) % walkFrame == 0){
			battleInput.setButtonData(BATTLE_BUTTON_RIGHT, false);
			battleInput.setButtonData(BATTLE_BUTTON_2, true);
			cntStart = cnt;
		} else{
			battleInput.setButtonData(BATTLE_BUTTON_RIGHT, true);
			battleInput.setButtonData(BATTLE_BUTTON_2, false);
		}
	} else {
		if ((cnt - cntStart) % walkFrame == 0){
			battleInput.setButtonData(BATTLE_BUTTON_LEFT, false);
			battleInput.setButtonData(BATTLE_BUTTON_2, true);
			cntStart = cnt;
		} else{
			battleInput.setButtonData(BATTLE_BUTTON_LEFT, true);
			battleInput.setButtonData(BATTLE_BUTTON_2, false);
		}
	}
}

//----------------------------------------------------------
// ガードステップ時の操作
void CharDotsForTutorial::updateGuardLowPosStep(){
	int walkFrame = 70;

	// バグ防止
	if (cnt % INPUT_RESET_CNT == 0){
		battleInput.setDirectionKey(BattleInputData::DIR_5);
		battleInput.setButtonData(BATTLE_BUTTON_1, false);
		battleInput.setButtonData(BATTLE_BUTTON_2, false);
	}

	if (getCurDirState() == RIGHT){
		if ((cnt - cntStart) % walkFrame == 0){
			battleInput.setButtonData(BATTLE_BUTTON_RIGHT, false);
			battleInput.setButtonData(BATTLE_BUTTON_DOWN, true);
			battleInput.setButtonData(BATTLE_BUTTON_1, true);
			cntStart = cnt;
		}
		else if ((cnt - cntStart) % walkFrame >= walkFrame - 10){
			battleInput.setButtonData(BATTLE_BUTTON_DOWN, true);

		} else{
			battleInput.setButtonData(BATTLE_BUTTON_RIGHT, true);
			battleInput.setButtonData(BATTLE_BUTTON_DOWN, false);
			battleInput.setButtonData(BATTLE_BUTTON_1, false);
		}
	} 
	else {
		if ((cnt - cntStart) % walkFrame == 0){
			battleInput.setButtonData(BATTLE_BUTTON_LEFT, false);
			battleInput.setButtonData(BATTLE_BUTTON_DOWN, true);
			battleInput.setButtonData(BATTLE_BUTTON_1, true);
			cntStart = cnt;
		} else if ((cnt - cntStart) % walkFrame >= walkFrame - 10){
			battleInput.setButtonData(BATTLE_BUTTON_DOWN, true);

		} else{
			battleInput.setButtonData(BATTLE_BUTTON_LEFT, true);
			battleInput.setButtonData(BATTLE_BUTTON_DOWN, false);
			battleInput.setButtonData(BATTLE_BUTTON_1, false);
		}
	}

}

//----------------------------------------------------------
// ガードステップ時の操作
void CharDotsForTutorial::updateGuardMidPosStep(){
	int walkFrame = 90;

	// バグ防止
	if (cnt % INPUT_RESET_CNT == 0){
		battleInput.setDirectionKey(BattleInputData::DIR_5);
		battleInput.setButtonData(BATTLE_BUTTON_1, false);
		battleInput.setButtonData(BATTLE_BUTTON_2, false);
	}


	if (getCurDirState() == RIGHT){
		if ((cnt - cntStart) % walkFrame == 0){
			battleInput.setButtonData(BATTLE_BUTTON_RIGHT, false);
			battleInput.setButtonData(BATTLE_BUTTON_UP, true);
			battleInput.setButtonData(BATTLE_BUTTON_2, true);
			cntStart = cnt;
		} else if ((cnt - cntStart) % walkFrame >= walkFrame - 30){
			battleInput.setButtonData(BATTLE_BUTTON_UP, true);

		} else{
			battleInput.setButtonData(BATTLE_BUTTON_RIGHT, true);
			battleInput.setButtonData(BATTLE_BUTTON_UP, false);
			battleInput.setButtonData(BATTLE_BUTTON_2, false);
		}
	} else {
		if ((cnt - cntStart) % walkFrame == 0){
			battleInput.setButtonData(BATTLE_BUTTON_LEFT, false);
			battleInput.setButtonData(BATTLE_BUTTON_UP, true);
			battleInput.setButtonData(BATTLE_BUTTON_2, true);
			cntStart = cnt;
		} else if ((cnt - cntStart) % walkFrame >= walkFrame - 30){
			battleInput.setButtonData(BATTLE_BUTTON_UP, true);

		} else{
			battleInput.setButtonData(BATTLE_BUTTON_LEFT, true);
			battleInput.setButtonData(BATTLE_BUTTON_UP, false);
			battleInput.setButtonData(BATTLE_BUTTON_2, false);
		}
	}

}

//----------------------------------------------------------
// バトル時の更新
void CharDotsForTutorial::updateBattle(){
	int updateInterval = 10;

	if (cnt % updateInterval == 0){
		if ((rand() % 100) < 60)
			battleInput.curData.init();

		if ((rand() % 100 )< 60){
			if (getCurDirState() == RIGHT)
				battleInput.setDirectionKey(BattleInputData::DIR_1);
			else
				battleInput.setDirectionKey(BattleInputData::DIR_3);
		} else{
			if (getCurDirState() == RIGHT)
				battleInput.setDirectionKey(BattleInputData::DIR_6);
			else
				battleInput.setDirectionKey(BattleInputData::DIR_4);
		}


		for (int i = 0; i < MAX_BATTLE_BUTTON; ++i){
			battleInput.setButtonData(rand() % MAX_BATTLE_BUTTON, (rand() % 2) != 0);
		}


	}
	battleInput.setButtonData(BATTLE_BUTTON_A, false);	// 挑発防止

}