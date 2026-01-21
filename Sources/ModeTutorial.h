//==========================================================
// チュートリアルモード
#ifndef _MODE_TUTORIAL_H
#define _MODE_TUTORIAL_H

#include	"Battle.h"
#include	"GameReady.h"	
#include	"BattleConfigInfo.h"
#include	"TutorialStep.h"		// 達成項目を作成
#include	"TextureLoader.h"
#include	"Polygon.h"

//==========================================================
// 実験)CLEAR 文字を制御、描画するクラス
class StringClear{
private:
	bool isSceneEnd;

	// クラス内遷移
	enum eScene{
		INIT,
		START_POINT,
		STOP_POINT,
		END_POINT,
		END,
	}scene;

	enum eTexList{
		STRING_CLEAR,
		MAX_TEX,
	};
	TextureLoader *pTextureLoader;

	const float WIDTH;
	const float HEIGHT;

	const float INIT_X;
	const float INIT_Y;

	const float STOP_POINT_X;
	const float STOP_POINT_STOP_CNT;	// とまっているフレーレム数

	const float END_POINT_X;


	// 制御用
	int cnt;
	int cntStart;

	// 描画用ポリゴン
	Polygon2D poly;
	float curSpeedX;
	const float MAX_SPEED_X;

	//----------------------------------------------------------
	// クラス内メソッド
	void updateCntStart();
	
	void updateStartPoint();
	void updateStopPoint();
	void updateEndPoint();

public:
	//==========================================================
	// 関数
	StringClear();
	virtual ~StringClear();

	bool getIsSceneEnd(){ return isSceneEnd; }

	void init();
	void load();
	void draw();
	void update();
};

//==========================================================
// チュートリアルクラス
class ModeTutorial{
protected:
	bool isSceneEnd;

	// クラス内遷移
	enum eScene{
		AUTO_SETTING,	// 固定の設定をする
		GAME_READY,
		STEP,			// (チュートリアル)
		CLEAR_STEP,		// ステップクリア
		NEXT_STEP,		// 
		ALL_CLEAR,
		END,
	}scene;

	// テクスチャ
	enum eTexList{
		BAR,
		BAR_CAP,
		MAX_TEX,
	};
	TextureLoader	*pTextureLoader;

	// ポリゴンの定数
	const float TITLE_BAR_WIDTH;
	const float TITLE_BAR_HEIGHT;
	const float TITLE_BAR_X;
	const float TITLE_BAR_Y;

	const float TITLE_BAR_CAP_WIDTH;
	const float TITLE_BAR_CAP_HEIGHT;
	const float TITLE_BAR_CAP_Y;
	const float TITLE_BAR_CAP_LEFT_X;		// 左側
	const float TITLE_BAR_CAP_RIGHT_X;		// 右側

	const float DESCRIPTION_BAR_WIDTH;
	const float DESCRIPTION_BAR_HEIGHT;
	const float DESCRIPTION_BAR_X;
	const float DESCRIPTION_BAR_Y;

	const float DESCRIPTION_BAR_CAP_WIDTH;
	const float DESCRIPTION_BAR_CAP_HEIGHT;
	const float DESCRIPTION_BAR_CAP_Y;
	const float DESCRIPTION_BAR_CAP_LEFT_X;
	const float DESCRIPTION_BAR_CAP_RIGHT_X;

	// 描画用ポリゴン
	Polygon2D	polyTitleBar;
	Polygon2D	polyTitleBarCapLeft;
	Polygon2D	polyTitleBarCapRight;

	Polygon2D	polyDescriptionBar;
	Polygon2D	polyDescriptionBarCapLeft;
	Polygon2D	polyDescriptionBarCapRight;

	StringClear	stringClear;	// クリア文字の描画と制御

	// 制御用
	int curStep;		// 現在のステップ
	
	int cnt;
	int cntStart;

	// 試合情報
	eGameMode			GAME_MODE;
	eRoundTimeLimit		ROUND_TIME;
	eNumOfRound			ROUND_NUM;
	eCharacterList		PLAYER_1_CHARACTER;	// キャラクターを固定にする
	eCharacterList		PLAYER_2_CHARACTER;
	ePlayerID			PLAYER_1_ID;
	ePlayerID			PLAYER_2_ID;
	eStageList			STAGE;

	BattleConfigInfo	*pBattleConfigInfo;
	
	// 主な画面
	GameReady			*pGameReady;
	Battle				*pBattle;

	AbstractTutorialStep	**pTutorialStep;	// ポインタをまとめる配列
	AbstractTutorialStep	*pTutorialStepMove;
	AbstractTutorialStep	*pTutorialStepJump;
	AbstractTutorialStep	*pTutorialStepAttack;
	AbstractTutorialStep	*pTutorialStepGuardHighPos;
	AbstractTutorialStep	*pTutorialStepGuardLowPos;
	AbstractTutorialStep	*pTutorialStepGuardMidPos;
	AbstractTutorialStep	*pTutorialStepBattle;

	// クラス内メソッド
	void drawBar();

	void updateCounterStart(){ cntStart = cnt; };	// カウンタの更新

	void initBattleHitPoint();	// 仮)プレイヤーを死なせない処理

	void interruptReturnToTitle();		// + - 同時押しで、タイトルに戻る

public:
	// チュートリアル項目
	enum eTutorialSteps{
		STEP_MOVE,
		STEP_JUMP,
		STEP_ATTACK,
		STEP_GUARD_HIGH_POS,
		STEP_GUARD_LOW_POS,
		STEP_GUARD_MID_POS,
		STEP_BATTLE,
		MAX_STEP,
	};

	ModeTutorial();
	virtual ~ModeTutorial();

	bool getIsSceneEnd(){ return isSceneEnd; }
	int getCurStep(){ return curStep; }

	void init();
	void load();
	void draw();
	void update();
};


#endif