//==========================================================
// 戦闘時に使用するUIのクラス
#ifndef _BATTLE_UI_H
#define _BATTLE_UI_H

#include	"Actor.h"
#include	"MyTexture.h"
#include	"Polygon.h"
#include	"Constants.h"
#include	"BattleConfigInfo.h"
#include	"TextureLoader.h"

//==========================================================
// HPバー用クラス
class HitPointBarUI{
private:
	//==========================================================
	// 定数
	const float BAR_W;// = 260 * WINDOW_WIDTH_RATIO;
	const float BAR_H;// = 40 * WINDOW_HEIGHT_RATIO;

	const float PLAYER1_BAR_X;// =	((WINDOW_WIDTH / 2.0f) - BAR_W) - (((WINDOW_WIDTH / 2.0f) - BAR_W) * 0.8f);
	const float PLAYER2_BAR_X;// =	(WINDOW_WIDTH / 2.0f) + (((WINDOW_WIDTH / 2.0f) - BAR_W) * 0.8f);
	const float BAR_Y;// =	(50) * WINDOW_HEIGHT_RATIO;


	//==========================================================
	// 変数
	//----------------------------------------------------------
	// 体力情報
	int *player1MaxHitPoint;
	int *player1CurHitPoint;
	Polygon2D polyPlayer1HpBarFrame;
	Polygon2D polyPlayer1HpBar;

	int *player2MaxHitPoint;
	int *player2CurHitPoint;
	Polygon2D polyPlayer2HpBarFrame;
	Polygon2D polyPlayer2HpBar;

	//----------------------------------------------------------
	// テクスチャ
	// 読み込むテクスチャの情報
	enum eTexList{
		HP_BAR_FRAME = 0,	// 体力バーのフレーム
		HP_BAR_NORMAL,		// 通常時の体力バー
		HP_BAR_MAX,			// 最大体力時の体力バー
		MAX_TEX,
	};
	TextureLoader	*pTextureLoader;
public:
	//==========================================================
	// 関数
	HitPointBarUI(Actor *player1, Actor *player2);
	virtual ~HitPointBarUI();

	void init();
	void load();	// テクスチャのロード

	void draw();
	void update();
};

//==========================================================
// タイマーUIクラス
class TimerUI{
private:
	//==========================================================
	// 定数
	const char LETTER_NUM;// = 2;	// 表示文字数
	const float TIME_LETTER_WIDTH;// = 40.0f;
	const float TIME_LETTER_HEIGHT;// = 80.0f;
	const float TIME_LETTER_X;// = (WINDOW_WIDTH / 2) - (TIME_LETTER_WIDTH*LETTER_NUM / 2.0f);
	const float TIME_LETTER_Y;// = (20);


	//==========================================================
	// 変数
	int maxTime;
	int curTime;

	unsigned char timeCounter;	// カウンタ(fps回カウントしたら0にリセットする)

	bool isTimeOver;

	// 時間の描画は、ポリゴンクラスを使用せず文字列表示で行う

	//==========================================================
	// テクスチャ関係
	enum eTexList{
		NUMBER_FONT = 0,
		MAX_TEX,
	};

	TextureLoader	*pTextureLoader;

public:
	TimerUI(BattleConfigInfo *_pBattleConfigInfo);
	virtual ~TimerUI();

	void init();
	void load();

	void draw();
	void update();

	int getCurTime(){ return curTime; }	// 現在時間を取得
	int getMaxTime(){ return maxTime; }	// 最大時間を取得
	bool getIsTimeOver(){ return isTimeOver; }	// 時間切れフラグを取得

};

//==========================================================
// ラウンド勝利アイコン
class RoundIconUI{
private:
	//==========================================================
	// テクスチャ関係
	enum eTexList{
		ROUND_ICON = 0,
		MAX_TEX,
	};
	TextureLoader *pTextureLoader;

public:
	// 座標用、定数
	const float ICON_WIDTH;// = 20.0f;
	const float ICON_HEIGHT;// = 20.0f;
	const float ICON_GAP_WIDTH;// = 5.0f;
	const float PLAYER1_ICON_BASE_X;// = (WINDOW_WIDTH / 2) - (50.0f) - ICON_WIDTH;	// 表示を開始する基準点
	const float PLAYER2_ICON_BASE_X;// = (WINDOW_WIDTH / 2) + (50.0f);
	const float ICON_Y;// = (30.0f);

	int	maxRound;	// 設定した最大ラウンド数を格納しておく
	int curRound;	// 現在のラウンド数(１～maxRound)

	// アイコン数はラウンド数によって変動する(1:1 3:2 5:3 7:4)
	char iconNum;

	Polygon2D *polyPlayer1RoundIcon;	// アイコン数だけ、配列を確保
	Polygon2D *polyPlayer2RoundIcon;

	enum eTexRoundIconInfo{	// RoundIconないのテクスチャの情報(anmNoで指定すれば使用できる)
		NOTHING = 0,	
		VICTORY,
		TIME_UP,
		DRAW,
		PERFECT,
	};


	RoundIconUI(BattleConfigInfo *_pBattleConfigInfo);
	virtual ~RoundIconUI();

	//==========================================================
	// セッター
	void setPlayer1Icon(eTexRoundIconInfo _iconType);
	void setPlayer2Icon(eTexRoundIconInfo _iconType);

	void setNextRound();	// 次のラウンドへ進める。

	//==========================================================
	// ゲッター
	// 次に使用するICONの位置を探す
	char searchPlayer1NextIconSpace();
	char searchPlayer2NextIconSpace();

	bool getBattleEndState();	// 全ラウンドが終了したかどうかを取得(true:終了 false:継続)

	void init();
	void load();

	void draw();
	void update();
};

//==========================================================
// コンボ
class ComboUI{
private:
	// 定数
	const float FONT_WIDTH;
	const float FONT_HEIGHT;

	const float DRAW_POS_Y;
	const float PLAYER1_DRAW_POS_X;
	const float PLAYER2_DRAW_POS_X;	


	// テクスチャ関係
	enum eTexList{
		NUMBER_FONT,
		MAX_TEX,
	};
	TextureLoader *pTextureLoader;

	// 参照するプレイヤー
	Actor *player1;
	Actor *player2;

public:
	ComboUI(Actor *_player1, Actor *_player2);
	virtual ~ComboUI();
	
	void drawCombo(Actor *_player);	

	void load();

	void draw();
	void update();
};

#endif