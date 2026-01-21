#include	"BattleConfigInfo.h"

//==========================================================
// BattleInfo
//----------------------------------------------------------
// コンストラクタ
BattleConfigInfo::BattleConfigInfo(){
	// 適当な初期化をする
	setModeData(VERSUS);

	setNumOfRound(BEST_OF_FIVE);
	setRoundTimeLimit(LIMIT_30SEC);

	setSelectStage(STAGE_LIST_SKY_PLACE);

	setSelectCharacter(CHARACTER_LIST_RANDOM, CHARACTER_LIST_RANDOM);	
	setPlayerID(1, PLAYERID_PLAYER1);
	setPlayerID(2, PLAYERID_PLAYER2);
}


//----------------------------------------------------------
// デストラクタ
BattleConfigInfo::~BattleConfigInfo(){

}

//----------------------------------------------------------
// データのセット
void BattleConfigInfo::setModeData(eGameMode _gameMode){
	selectedGameMode = _gameMode;
}

//----------------------------------------------------------
// 定数を渡し、それに対応した整数(ラウンド数を)代入する
void BattleConfigInfo::setNumOfRound(eNumOfRound _val){
	switch (_val){
		case BEST_OF_ONE:
			maxRound = 1;
			break;
		case BEST_OF_THREE:
			maxRound = 3;
			break;
		case BEST_OF_FIVE:
			maxRound = 5;
			break;
		case BEST_OF_SEVEN:
			maxRound = 7;
			break;
	}
}

//----------------------------------------------------------
// 定数で判断し、整数の時間を代入する
void BattleConfigInfo::setRoundTimeLimit(eRoundTimeLimit _val){
	switch (_val){
		case LIMIT_30SEC:
			roundTime = 30;
			break;
		case LIMIT_60SEC:
			roundTime = 60;
			break;
		case LIMIT_99SEC:
			roundTime = 99;
			break;
		case NO_LIMIT:
			roundTime = ROUND_TIME_NO_LIMIT;
			break;
	}
}

//----------------------------------------------------------
// 選択されたキャラクターの値を代入
void BattleConfigInfo::setSelectCharacter(eCharacterList _player1, eCharacterList _player2){
	player1SelectedCharacter = _player1;
	player2SelectedCharacter = _player2;
}

//----------------------------------------------------------
// プレイヤー番号を指定して、キャラクターを代入
void BattleConfigInfo::setSelectCharacter(int _playerNo, eCharacterList _val){
	if (_playerNo == 1)
		player1SelectedCharacter = _val;
	else if (_playerNo == 2)
		player2SelectedCharacter = _val;
}

//----------------------------------------------------------
// プレイヤーとCPUの識別子をセット
void BattleConfigInfo::setPlayerID(int _playerNo, ePlayerID _val){
	if (_playerNo == 1)
		player1ID = _val;
	else if (_playerNo == 2)
		player2ID = _val;
}



//----------------------------------------------------------
// 選択されたステージの値を代入
void BattleConfigInfo::setSelectStage(eStageList _val){
	selectedStage = _val;
}