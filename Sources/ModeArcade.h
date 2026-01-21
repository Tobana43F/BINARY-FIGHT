//==========================================================
// アーケードモードクラス
#ifndef _MODE_ARCADE_H
#define _MODE_ARCADE_H

//==========================================================
// インクルード
#include	"BattleConfigInfo.h"
#include	"CharacterSelectForArcadeMode.h"
#include	"GameReady.h"
#include	"Battle.h"

//==========================================================
// クラス定義
class ModeArcade{
private:
	bool isSceneEnd;

	//----------------------------------------------------------
	// バトル情報を保持するくらす
	BattleConfigInfo	*pBattleConfig;

	//----------------------------------------------------------
	// クラス内遷移
	enum eScene{
		CHARACTER_SELECT,	// 最初の一回

		AUTO_SETTING,		// 自動設定項目の設定をする(敵、ステージ)

		GAME_READY,
		BATTLE,
		
		CONTINUE,			// 敗戦後の画面
		RESULT,				// すべての敵を倒したあと(game over)
		END,				// 終了時
	}scene;

	//----------------------------------------------------------
	// バトルの制御
	eGameMode		GAME_MODE;
	eRoundTimeLimit ROUND_TIME;
	eNumOfRound		ROUND_NUM;
	int MAX_BATTLE_NUM;	// 対戦相手の数(連戦数)
	int curBattleNum;

	//----------------------------------------------------------
	// 主な画面
	CharacterSelect *pCharacterSelect;	// アーケード用のキャラ選画面を用意
	GameReady		*pGameReady;		// ゲーム準備画面
	Battle			*pBattle;			// バトル

	//----------------------------------------------------------
	// 割り込み的な処理
	void interruptReturnToTitle();		// + - 同時押しで、タイトルに戻る

public:
	//==========================================================
	// 関数
	ModeArcade();
	virtual ~ModeArcade();

	bool getIsSceneEnd(){ return isSceneEnd; }

	void init();

	void update();
	void draw();
};


#endif