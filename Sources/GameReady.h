//==========================================================
// ゲーム前の準備をするクラス(テクスチャのロードなど)
// キャラクター、ステージを選択したあとの ○VS□ のような画面
// ここでロードするのが一番自然だと思うから。

#ifndef _GAME_READY_H
#define _GAME_READY_H


#include	"Polygon.h"
#include	"BattleConfigInfo.h"
#include	"Actor.h"
#include	"AbstractStage.h"
#include	"BattleUI.h"
#include	"BattleEffect.h"	
#include	"CharacterShadow.h"
#include	"RoundEffect.h"	


#include	"TextureLoader.h"
#include	"MySound.h"

class GameReady{
private:
	//==========================================================
	// 変数
	// クラスの遷移の制御
	bool isSceneEnd;

	// クラス内の遷移
	enum eGameReadyScene{
		VERSUS_DRAW,	// VS画面
		LOADING,		// その後のロード画面
		LOAD_COMPLETE,	// ロード完了
		GAME_READY_END,	// 画面遷移
	}gameReadyScene;

	//----------------------------------------------------------
	// ○VS□ 描画用
	// 大きさ
	const float VS_POLY_WIDTH;
	const float VS_POLY_HEIGHT;
	// 初期位置
	const float VS_POLY_PLAYER_INIT_Y;		// 1,2共通
	const float VS_POLY_PLAYER1_INIT_X;
	const float VS_POLY_PLAYER2_INIT_X;
	// 目標位置
	const float VS_POLY_PLAYER1_TARGET_X;
	const float VS_POLY_PLAYER2_TARGET_X;

	// NowLoading
	const float NOW_LOADING_WIDTH;
	const float NOW_LOADING_HEIGHT;
	const float NOW_LOADING_X;
	const float NOW_LOADING_Y;

	// フォントVS
	// V
	const float FONT_V_WIDTH;
	const float FONT_V_HEIGHT;
	const float FONT_INIT_V_X;
	const float FONT_INIT_V_Y;
	// V
	const float FONT_S_WIDTH;
	const float FONT_S_HEIGHT;
	const float FONT_INIT_S_X;
	const float FONT_INIT_S_Y;

	// テクスチャ
	enum eTexList{
		NOW_LOADING_STRING,
		FONT_V,	// VSの表示用
		FONT_S,
		BACK,
		MAX_TEX,
	};
	TextureLoader	*pTextureLoader;
	
	// サウンド
	soundNS::SOUND_LABEL	labelStart;
	soundNS::SOUND_LABEL	labelEnd;

	Polygon2D	polyNowLoading;

	Polygon2D	polyFontV;
	Polygon2D	polyFontS;

	// 仮)クラス化したほうがいいかも
	Polygon2D	polyVSPlayer1;	// Actorクラスで個々で使用するテクスチャを読み込んでいる。
	F2Value		curSpeed1;		// プレイヤー１用のスピード

	Polygon2D	polyVSPlayer2;
	F2Value		curSpeed2;
	
	bool isVSInDrawEnd;		// ロード前のVS表示の描画終了フラグ
	bool isVSOutDrawEnd;	// ロード後のVS表示の描画終了フラグ

	//==========================================================
	// 関数
	void drawVSIn();
	void updateVSIn();

	void drawVSOut();
	void updateVSOut();
	
	void updateGameScene();

public:
	//==========================================================
	// 実態を確保するポインタ
	// バトルクラスへ渡す
	BattleConfigInfo	*pBattleConfigInfo;		// バトル情報

	Actor				*pPlayer1;	// プレイヤー
	Actor				*pPlayer2;

	AbstractStage		*pStage;	// ステージ

	CharacterShadow		*pCharacterShadow;
	
	TimerUI				*pTimerUI;			// UI
	RoundIconUI			*pRoundIconUI;
	HitPointBarUI		*pHitPointBarUI;	
	ComboUI				*pComboUI;

	BattleEffect		*pBattleEffect;	// エフェクト

	RoundStartEffect	*pRoundStartEffect;

	RoundKOEffect		*pRoundKOEffect;

	RoundEndEffect		*pRoundEndEffect;

	BattleEndEffect		*pBattleEndEffect;

	//==========================================================
	// 関数
	GameReady(BattleConfigInfo *_pBattleConfigInfo);
	virtual ~GameReady();
	
	bool getIsSceneEnd(){ return isSceneEnd; }


	void allocCharacter(Actor **_pPlayer, char _playerNo);
	void setPlayerID(Actor *_pPlayer, ePlayerID _playerID);
	void allocStage(void);
	
	void initBeforeGame();
	void load();	// テクスチャのロードなど

	void draw();
	void update();
};

#endif