#include	"CharacterDotsCPU.h"	// メソッド定義
#include	<stdlib.h>
#include	<math.h>

//==========================================================
// CharDotsCPU
//----------------------------------------------------------
// コンストラクタ
CharDotsCPU::CharDotsCPU(){
	updateInterval = 20;
	cnt = 0;
	cntGuardLength = 60;
	cntBackNoise = 100;


	cntWalkNoise = rand() % 10;
	perCrouchNear = rand() % 50;
	cntCrouchNearNoise = rand() % 30;
	perAttackFarNoise = rand() % 30;
	cntVJumpNoise = (rand() % 120);
	perGuard = 100 - (rand() % 80);
	cntGuardLength = (rand() % 50) + 30;
	cntBackNoise = rand() % 40;
	cntFJumpNoise = rand() % 60;

	isNear = false;
	isKeepBlock = true;
}

//----------------------------------------------------------
// デストラクタ
CharDotsCPU::~CharDotsCPU(){

}

//----------------------------------------------------------
// CPUの行動更新
void CharDotsCPU::updateCPU(){
	//==========================================================
	// 技用ボタンのリセット
	battleInput.setButtonData(BATTLE_BUTTON_1, false);
	battleInput.setButtonData(BATTLE_BUTTON_2, false);

	//==========================================================
	// 一定感覚の更新
	if (cnt % updateInterval == 0){
		if (rand() % 100 < 60)
			battleInput.curData.init();


		cntWalkNoise = rand() % 10;
		perCrouchNear = rand() % 50;
		cntCrouchNearNoise = rand() % 30;
		perAttackFarNoise = rand() % 30;
		cntVJumpNoise = (rand() % 120);
		perGuard = 100 - (rand() % 80);
		cntGuardLength = (rand() % 50) + 30;
		cntBackNoise = rand() % 40;
		cntFJumpNoise = rand() % 60;
	}

	
	//==========================================================
	// 間合い管理
	if (cnt % (10 + cntWalkNoise) == 0){
		// 離れているとき
		if (fabs(pOpponentActor->coord.x - coord.x) >= 230.0f){
			isNear = false;

			// 前進
			if (getCurDirState() == RIGHT)
				battleInput.setDirectionKey(BattleInputData::DIR_6);
			else
				battleInput.setDirectionKey(BattleInputData::DIR_4);
		}
		// 近いとき
		else{
			isNear = true;

			// なるべく後退しないようにする
			if (rand() % 100 < 60 + cntBackNoise){
				if (getCurDirState() == RIGHT)
					battleInput.setDirectionKey(BattleInputData::DIR_4);
				else
					battleInput.setDirectionKey(BattleInputData::DIR_6);
			}

			// かなり近いとき
			if (fabs(pOpponentActor->coord.x - coord.x) <= 10)
				battleInput.setButtonData(BATTLE_BUTTON_1, true);
			else if (fabs(pOpponentActor->coord.x - coord.x) <= 120)
				battleInput.setButtonData(BATTLE_BUTTON_2, true);
			else if (fabs(pOpponentActor->coord.x - coord.x) <= 180){
				if (getCurDirState() == RIGHT)
					battleInput.setDirectionKey(BattleInputData::DIR_6);
				else
					battleInput.setDirectionKey(BattleInputData::DIR_4);
				battleInput.setButtonData(BATTLE_BUTTON_1, true);
			}
			else {
				if (getCurDirState() == RIGHT)
					battleInput.setDirectionKey(BattleInputData::DIR_6);
				else
					battleInput.setDirectionKey(BattleInputData::DIR_4);
				battleInput.setButtonData(BATTLE_BUTTON_2, true);
			}
		}
	}
	else {
		//==========================================================
		// 適当に技を振る
		if (cnt % (20 + perAttackFarNoise) == 0){
			// 適当な間合いで
			if (fabs(pOpponentActor->coord.x - coord.x) >= 180.0f &&  fabs(pOpponentActor->coord.x - coord.x) <= 350.0f){
				if (fabs(pOpponentActor->coord.x - coord.x) >= 230.0f){
					// 攻撃
					battleInput.setButtonData(BATTLE_BUTTON_2, true);
					// 攻撃ジャンルの判断
					if (rand() % 100 < 70){
						if (getCurDirState() == RIGHT)
							battleInput.setButtonData(BATTLE_BUTTON_RIGHT, true);
						else
							battleInput.setButtonData(BATTLE_BUTTON_LEFT, true);
					}
				}
				else {
					// 攻撃
					battleInput.setButtonData(BATTLE_BUTTON_1, true);
					// 攻撃ジャンルの判断
					if (rand() % 100 < 70){
						if (getCurDirState() == RIGHT)
							battleInput.setButtonData(BATTLE_BUTTON_RIGHT, true);
						else
							battleInput.setButtonData(BATTLE_BUTTON_LEFT, true);
					}
				}
			}
		}
		//==========================================================
		// しゃがみ判定
		if (cnt % (30 + cntCrouchNearNoise)){
			if (rand() % 100 <= perCrouchNear && !isNear)
				battleInput.setButtonData(BATTLE_BUTTON_DOWN, true);
		}
	}


	

	//==========================================================
	// 適当な垂直ジャンプ
	if (cnt % (120 + cntVJumpNoise) == 0){
		battleInput.setDirectionKey(BattleInputData::DIR_8);
	}

	//==========================================================
	// 前ジャンプ
	if (cnt % (10 + cntFJumpNoise) == 0){
		// ちょうどいい間合いから
		if (fabs(pOpponentActor->coord.x - coord.x) >= 80.0f &&  fabs(pOpponentActor->coord.x - coord.x) <= 180.0f){
			if (getCurDirState() == RIGHT)
				battleInput.setDirectionKey(BattleInputData::DIR_9);
			else
				battleInput.setDirectionKey(BattleInputData::DIR_7);
		}
	}

	//==========================================================
	// 空中攻撃
	if (!isLand){
		battleInput.curData.init();
		
		if (coord.y > 30 && curSpeed.y > 0)
			battleInput.setButtonData(BATTLE_BUTTON_2, true);
	}

	//==========================================================
	// ガードしておく
	if (rand() % 100 <= perGuard){
		if (pOpponentActor->isAttackAction && fabs(pOpponentActor->coord.x - coord.x) >= 60.0f){
			battleInput.curData.init();
			if (getCurPosState() == LEFT)
				battleInput.setDirectionKey(BattleInputData::DIR_1);
			else
				battleInput.setDirectionKey(BattleInputData::DIR_3);

			if (!pOpponentActor->isLand){
				if (getCurPosState() == LEFT)
					battleInput.setDirectionKey(BattleInputData::DIR_4);
				else
					battleInput.setDirectionKey(BattleInputData::DIR_6);
			}
			isKeepBlock = true;
			cntGuard = cntGuardLength;
		}
	}

	//==========================================================
	// 相手が攻撃していたら飛んでみる
	if (rand() % 100 <= 40){
		if (isLand && pOpponentActor->isAttackAction){
			// ちょうどいい間合いから
			if (fabs(pOpponentActor->coord.x - coord.x) >= 80.0f &&  fabs(pOpponentActor->coord.x - coord.x) <= 250.0f){
				if (getCurDirState() == RIGHT)
					battleInput.setDirectionKey(BattleInputData::DIR_9);
				else
					battleInput.setDirectionKey(BattleInputData::DIR_7);
			}
		}
	}


	//==========================================================
	// ガードに成功していたら、ガードしておく
	if (isBlock){
		if (getCurPosState() == LEFT)
			battleInput.setDirectionKey(BattleInputData::DIR_1);
		else
			battleInput.setDirectionKey(BattleInputData::DIR_3);

		if (!pOpponentActor->isLand){
			if (getCurPosState() == LEFT)
				battleInput.setDirectionKey(BattleInputData::DIR_4);
			else
				battleInput.setDirectionKey(BattleInputData::DIR_6);
		}
		isKeepBlock = true;
		cntGuard = cntGuardLength;
	}

	// ガードの減算
	if (cntGuard < 0){
		isKeepBlock = false;
		cntGuard = 0;
	}
	else
	{
		cntGuard--;
	}

	//==========================================================
	// 使用しないボタン
	battleInput.setButtonData(BATTLE_BUTTON_A, false);	// 挑発防止

}


//----------------------------------------------------------
// キャラクターの操作
void CharDotsCPU::operateActor(){
	updateCPU();

	CharDots::operateActor();

	cnt++;
}