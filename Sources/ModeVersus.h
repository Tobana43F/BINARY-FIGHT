//==========================================================
// バーサスモード(プレイヤー対プレイヤーなどが自由に選択できるモード)
#ifndef _MODE_VERSUS_H
#define _MODE_VERSUS_H

//==========================================================
// インクルード
#include	"BattleConfigInfo.h"
#include	"GameReady.h"
#include	"CharacterSelect.h"	// キャラクター選択画面を持つため
#include	"Battle.h"

//==========================================================
// クラス定義
class ModeVersus{
private:
	bool isSceneEnd;

	//----------------------------------------------------------
	// バトル情報を保持するクラス
	BattleConfigInfo	*pBattleConfigInfo;

	//----------------------------------------------------------
	// クラス内遷移
	enum eScene{
		CHARACTER_SELECT,
		STAGE_SELECT,

		GAME_READY,

		BATTLE,

		MENU_AFTER_BATTLE,	// バトルの勝敗判断画面上に、メニューを表示したい(再戦、キャラクター選択、メインメニューなど)
	}scene;

	//----------------------------------------------------------
	// バトルの制御
	eGameMode		GAME_MODE;
	eRoundTimeLimit ROUND_TIME;
	eNumOfRound		ROUND_NUM;

	//----------------------------------------------------------
	// 主な画面
	CharacterSelect	*pCharacterSelect;
	// StageSelect
	GameReady		*pGameReady;
	Battle			*pBattle;


	void interruptReturnToTitle();		// + - 同時押しで、タイトルに戻る

public:
	ModeVersus();
	virtual ~ModeVersus();

	bool getIsSceneEnd(){ return isSceneEnd; }

	void init();

	void draw();
	void update();
};


#endif