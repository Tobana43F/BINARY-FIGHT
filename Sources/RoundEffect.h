//==========================================================
// ラウンド開始時(ラウンド○、ファイト！)、
// 終了時(KO！！、TimeUp)などの演出を担うクラス
#ifndef _ROUND_EFFECT_H
#define _ROUND_EFFECT_H

#include	"TextureLoader.h"	// テクスチャを読み込むため
#include	"Polygon.h"			// ポリゴンを扱うため
#include	"Constants.h"		// 画面サイズ,FPS

#include	"BattleUI.h"		// 情報を参照するため

//==========================================================
// ゲーム開始クラス(キャラクターの登場演出)など


//==========================================================
// ゲーム終了(プレイヤー○ WIN)など
class BattleEndEffect{
private:
	bool isEffectEnd;

	// 文字サイズなどの定数
	const float PLAYER_WIN_WIDTH;
	const float PLAYER_WIN_HEIGHT;
	const float PLAYER_WIN_X;
	const float PLAYER_WIN_Y;

	const float DRAW_GAME_WIDTH;
	const float DRAW_GAME_HEIGHT;
	const float DRAW_GAME_X;
	const float DRAW_GAME_Y;

	// 点滅処理の定数
	const int	FLASH_NUM;			// 点滅回数
	const int	FLASH_INTERVAL;		// 点滅感覚(フレーム数)
	bool		isDrawFlash;		// 描画フラグ(true:描画　false:点滅の消えているとき)
	int			curFlashCnt;		// 現在の点滅回数

	// クラス内遷移
	enum eScene{
		CHECK_END_STATE,		// どちらが勝利したのかなどを判断する
		SET_DATA,
		DRAW_EFFECT_START,		// 点滅させる
		DRAW_EFFECT,
		END,
	}scene;

	// テクスチャ
	enum eTexList{
		PLAYER1_WIN_TEX,
		PLAYER2_WIN_TEX,
		DRAW_GAME_TEX,
		MAX_TEX,
	};
	TextureLoader	*pTextureLoader;

	enum eEffectType{
		INIT,			// 初期化用
		PLAYER1_WIN,
		PLAYER2_WIN,
		DRAW_GAME,
	}effectType;

	// 参照用
	RoundIconUI		*pRoundIconUI;

	// クラス内メソッド
	void checkEndState();	// 勝敗判断
	void setData();			// 必要なデータの設定

	// 制御用
	int cnt;	// クラス内カウンタ
	int cntStart;

public:
	Polygon2D		polyEffect;

	//----------------------------------------------------------
	// 関数
	BattleEndEffect(RoundIconUI *_pRoundIconUI);
	virtual ~BattleEndEffect();

	//----------------------------------------------------------
	// ゲッター
	bool getIsEffectEnd(){ return isEffectEnd; }

	//----------------------------------------------------------
	// 基本的な関数
	void init();
	void load();

	void draw();
	void update();
};

//==========================================================
// ラウンド開始時用クラス
class RoundStartEffect{
private:
	const float INIT_ROUND_WIDTH;
	const float INIT_ROUND_HEIGHT;
	const float INIT_ROUND_X;
	const float INIT_ROUND_Y;

	const float	INIT_NUMBER_WIDTH;
	const float INIT_NUMBER_HEIGHT;
	const float INIT_NUMBER_X;
	const float INIT_NUMBER_Y;

	const float INIT_FIGHT_WIDTH;
	const float INIT_FIGHT_HEIGHT;
	const float INIT_FIGHT_X;
	const float INIT_FIGHT_Y;

	// クラス内遷移
	enum eScene{
		WAIT,
		ROUND_CALL,
		FIGHT_CALL,
		END,
	}scene;

	// 参照するデータ
	int *curRound;	// 参照用
	int *maxRound;	// 参照用(FinalRoundを検地するため)

	// テクスチャ
	enum eTexList{
		FONT_ROUND,
		FONT_NUMBER,	

		FONT_FIGHT,
		
		MAX_TEX,
	};
	TextureLoader *pTextureLoader;

	// 制御用
	int cnt;	// 制御用カウンタ

	bool isRoundStarted;	// ラウンドが開始した。(コールが終わった)

public:
	Polygon2D	polyFontRound;
	Polygon2D	polyFontNumber;
	Polygon2D	polyFontFight;
	

	//==========================================================
	// 関数
	RoundStartEffect(int *_curRound, int *_maxRound);
	virtual ~RoundStartEffect();

	//----------------------------------------------------------
	// ゲッター
	bool getIsRoundStarted(){ return isRoundStarted; }

	void init();
	void load();

	void draw();
	void update();
};

//==========================================================
// KOしたときの演出用クラス(KOの表示と、スローモーション)
// KO判断は行わない(バトル内で行う)
class RoundKOEffect{
private:
	const float INIT_KO_WIDTH;
	const float INIT_KO_HEIGHT;
	const float INIT_KO_X;
	const float INIT_KO_Y;

	const int	SLOW_UPDATE_INTERVAL;	// スローモーション時の更新フレーム間隔

	// 遷移
	enum eScene{
		KO_STOP_AND_DRAW,	// KO時の停止とKOの文字の描画
		KO_SLOW,			// KO文字が消えたあとの少しの間のスローモーション
		END,				// KO描画、制御処理の終了
	}scene;

	// テクスチャ
	enum eTexList{
		FONT_KO,
		MAX_TEX,
	};
	TextureLoader *pTextureLoader;

	// 制御用
	int cnt;

	bool isNeedUpdate;	// 更新が必要(これの状態がtrueのときだけ、バトルの更新処理を行う)
	
	bool isKOEffectEnd;

public:
	
	Polygon2D	polyFontKO;

	//==========================================================
	// 関数
	RoundKOEffect();
	virtual ~RoundKOEffect();

	//----------------------------------------------------------
	// ゲッター
	bool getIsNeedUpdate(){ return isNeedUpdate; }
	bool getIsKOEffectEnd(){ return isKOEffectEnd; }

	//----------------------------------------------------------
	// 基本的な関数
	void init();
	void load();

	void draw();
	void update();
};


//==========================================================
// ラウンド終了時用クラス
class RoundEndEffect{
private:
	// 定数	
	const float DRAW_WIDTH;
	const float DRAW_HEIGHT;

	const float TIME_OVER_WIDTH;
	const float TIME_OVER_HEIGHT;

	const float PERFECT_WIDTH;
	const float PERFECT_HEIGHT;
	

	// クラス内遷移
	enum eScene{
		CHECK_END_STATE,
		SET_EFFECT,			// 値の設定など
		WAIT_TIME,			// 待ち時間（演出のため）
		DRAW_EFFECT,		// TimeUP,Draw,などの、ラウンド終了時のエフェクトを描画
		END,
	} scene;

	// 大まかなラウンド終了状態の判断
	enum eEffectType{
		PLAYER_KO,		// どちらかのプレイヤーがKO(勝敗がついた状態)
		PLAYER_PERFECT,	// パーフェクト
		TIME_OVER,		// 時間切れによる終了(判定勝ち,負け) 
		DRAW,			// 引き分け
	} effectType;

	// テクスチャ
	enum eTexList{
		FONT_DRAW_TEX,
		FONT_TIME_OVER_TEX,
		FONT_PERFECT_TEX,
		MAX_TEX,
	};
	TextureLoader	*pTextureLoader;

	// 参照用
	TimerUI	*pTimerUI;
	
	// 操作する
	RoundIconUI *pRoundIconUI;
	Actor		*pPlayer1;
	Actor		*pPlayer2;

	// 制御用
	int cnt;
	bool isRoundEndEffectEnd;

	// 関数
	void checkEndState();
	void setEffect();

	void setRoundIcon();

	void drawEffect();
	void updateEffect();

	void updateWaitTime();	// 待ち時間の制御

public:

	Polygon2D	polyEffect;	// 全エフェクトに共通させる。

	//==========================================================
	// 関数
	RoundEndEffect(TimerUI *_pTimerUI, RoundIconUI *_pRoundIconUI, Actor *_pPlayer1, Actor *_pPlayer2);
	virtual ~RoundEndEffect();

	//----------------------------------------------------------
	// ゲッター
	bool getIsRoundEndEffectEnd(){ return isRoundEndEffectEnd; }

	//----------------------------------------------------------
	// 基本的な関数
	void init();
	void load();

	void draw();
	void update();
};


#endif