//==========================================================
// メニューから、バトルまでの全ての情報を記憶するクラス。(記憶するだけ)
// ・メニューで選択した情報などが、全て記憶される。
// ・このクラスをいろいろなクラスに渡し、データをそのクラスから代入し、一連の流れが終了するまで保持する。
#ifndef _BATTLE_CONFIG_INFO_H
#define _BATTLE_CONFIG_INFO_H

//==========================================================
// インクルード
#include	"constCharacterList.h"		// 選択したキャラクターを判別するための、定数を参照するため。
#include	"constStageList.h"			// 選択したステージを判別するための、定数を参照するため。

//==========================================================
// 定数
// 仮)ゲームモード
enum eGameMode{
	ARCADE = 0,		//	(PvsC)を何連戦か行う
	VERSUS,			//	(PvsP)
	TUTORIAL,		//	特殊(チュートリアル)
	MANUAL,		//	マニュアル(ゲームモード？)
};

// 仮)ラウンド数
enum eNumOfRound{
	BEST_OF_ONE,	// 1
	BEST_OF_THREE,	// 3
	BEST_OF_FIVE,	// 5
	BEST_OF_SEVEN,	// 7
};

// タイム
enum eRoundTimeLimit{
	LIMIT_30SEC,	// 30秒
	LIMIT_60SEC,	// 60秒
	LIMIT_99SEC,	// 99秒
	NO_LIMIT,		// ∞
};
#define ROUND_TIME_NO_LIMIT (0xFF)	// この値を外部(TimerUI)で参照するため

// プレイヤーとCPUの識別子
enum ePlayerID{
	PLAYERID_PLAYER1,
	PLAYERID_PLAYER2,
	PLAYERID_CPU
};

//==========================================================
// クラス定義
class BattleConfigInfo{
private:
	// 遊ぶモード関係
	eGameMode		selectedGameMode;	// 仮)

	// 選択キャラクター関係
	eCharacterList	player1SelectedCharacter;
	eCharacterList	player2SelectedCharacter;

	// プレイヤー情報(CPUとの識別用)
	ePlayerID		player1ID;
	ePlayerID		player2ID;

	// 選択ステージ
	eStageList		selectedStage;

	// バトル関係
	int maxRound;		// ラウンド数(1,3,5,7)
	int roundTime;		// 1ラウンドの時間(30,60,99, ∞)

public:
	BattleConfigInfo();
	virtual ~BattleConfigInfo();

	// セッター
	void setModeData(eGameMode _gameMode);
	void setNumOfRound(eNumOfRound _val);
	void setRoundTimeLimit(eRoundTimeLimit val);

	void setSelectCharacter(eCharacterList _player1, eCharacterList _player2);
	void setSelectCharacter(int _playerNo, eCharacterList _val);

	void setPlayerID(int _playerNO, ePlayerID _val);

	void setSelectStage(eStageList _val);

	// ゲッター
	eGameMode getModeData(){ return selectedGameMode; }
	eCharacterList	getPlayer1SelectedCharacter(){ return player1SelectedCharacter; }
	eCharacterList	getPlayer2SelectedCharacter(){ return player2SelectedCharacter; }
	ePlayerID		getPlayer1ID(){ return player1ID; }
	ePlayerID		getPlayer2ID(){ return player2ID; }

	eStageList getSelectedStage(){ return selectedStage; }

	int getMaxRound(){ return maxRound; }
	int getRoundTime(){ return roundTime; }

};


#endif