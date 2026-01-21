//==========================================================
// 戦闘時のステージのクラス
#ifndef _ABSTRACT_STAGE_H
#define _ABSTRACT_STAGE_H

#include	"Polygon.h"			// ポリゴンを使用するため。FCoord2D構造体を使用するためなど
#include	"TextureLoader.h"	// テクスチャを使用するため
#include	"Actor.h"			// このクラスのポインタを持つため

#include	"MySound.h"			// 使用するサウンドラベルを持つため

#include	"Constants.h"		// 画面の幅を知るため

//==========================================================
// ステージの抽象クラス(このオブジェクトを生成しないこと)
class AbstractStage{
protected:
	//----------------------------------------------------------
	// カメラ処理に必要な変数
	Actor		*player1;
	Actor		*player2;
	bool		isPlayerInited;	// プレイヤーが参照されているかどうか

	//----------------------------------------------------------
	// テクスチャ関数
	TextureLoader	*pTextureLoader;

	// 使用するサウンドラベル
	soundNS::SOUND_LABEL	soundLabelBGM;

public:
	//----------------------------------------------------------
	// 定数
	const float INIT_GROUND_POS_Y;				// 地上のY座標
	const float INIT_PLAYER_GAP_WIDTH;			// キャラクターの初期位置のキャラ間の幅

	const float STAGE_HEIGHT;					// ステージの高さ
	const float STAGE_WIDTH;					// ステージの幅

	const float INIT_STAGE_POS_X;				// ステージの初期座標
	const float INIT_STAGE_POS_Y;

	const float WIDTH_RATIO;										// ステージ幅に対しての画面の幅の割合
	const float	HEIGHT_RATIO;										// ステージの高さに対しての画面の高さの割合

	// スクロール関係
	// 下限・上限
	const float SCROLL_LIMIT_POS_LEFT;// = (0.0f);
	const float SCROLL_LIMIT_POS_RIGHT;// = (WINDOW_WIDTH - STAGE_WIDTH);
	const float SCROLL_LIMIT_POS_TOP;// = (0.0f);
	const float SCROLL_LIMIT_POS_BOTTOM;// = (WINDOW_HEIGHT - STAGE_HEIGHT);

	// スクロール開始座標
	const float SCROLL_POS_LEFT;// = (WINDOW_WIDTH / 2.0f) - (WINDOW_WIDTH / 100.0f);
	const float SCROLL_POS_RIGHT;// = (WINDOW_WIDTH / 2.0f) + (WINDOW_WIDTH / 100.0f);
	const float SCROLL_POS_TOP;// = (WINDOW_HEIGHT * 0.4f);	// Y軸は同じ値にしたほうが、不具合がない。(とおもう)
	const float SCROLL_POS_BOTTOM;//= (WINDOW_HEIGHT * 0.4f);

	//----------------------------------------------------------
	// ステージのポリゴン関係
	Polygon2D	polyBackGround;	// 背景用ポリゴン(描画用ポリゴン)
	float		texU;				// 左上のテクスチャのU座標
	float		texV;				// 左上のテクスチャのV座標

	//----------------------------------------------------------
	// 基準点
	FCoord2D	stageBaseCoord;		// 背景の位置
	FCoord2D	charBaseCoord;		// 基準点となる座標(P1とP2の真ん中の座標となる)
	float		groundPosY;			// 地上のY座標

	// 判定用
	bool isStageLimitX;				// 画面端かどうか
	bool isStageLimitY;	

	//----------------------------------------------------------
	// 関数
	AbstractStage();
	virtual ~AbstractStage();

	void initPlayerPointer(Actor *_player1, Actor *_player2);	// 参照するプレイヤーの設定

	void initStageScroll();		// 画面のスクロールをリセットする。

	void updateBaseCoord();		// 基準座標を更新する
	void updateScroll();		// 画面スクロールをする
	void updateScrollLimit();	// 上限(下限)を超えたときの処理

	void updateStagePolygonUV();	// 背景のUV値を変えて、絵のスクロールを行う。

	//----------------------------------------------------------
	// ゲッター
	soundNS::SOUND_LABEL getUseBGMLabel(){ return soundLabelBGM; }

	//==========================================================
	// 基本的な関数(一応仮想関数にしておく)
	virtual void init();
	virtual void initStage() PURE_FUNC;	// ステージ個別の初期化

	virtual void load();

	virtual void draw();
	virtual void update();
};


#endif