#ifndef _TITLE_H
#define _TITLE_H

#include	"TextureLoader.h"
#include	"Polygon.h"

#include	"Constants.h"	// DEF_FRAME_RATEを使うため
#include	"MySound.h"

//==========================================================
// クラス
class Title{
private:
	bool isSceneEnd;

	// クラス内遷移
	enum eScene{
		TITLE_START,		// タイトルの入り	
		TITLE_COMPLETE,		// タイトルの入りが終了
		GAME_START,			// タイトルでスタートボタンが押された
		TITLE_END,			// 画面遷移
	}scene;

	// テクスチャ
	enum eTexList{
		BACK,
		STRING_PUSH,
		MAX_TEX,
	};
	TextureLoader *pTextureLoader;

	// サウンド
	soundNS::SOUND_LABEL	labelDecideSE;
	soundNS::SOUND_LABEL	labelBGM;

	// 背景
	Polygon2D	polyBack;
	// 文字
	const float STRING_WIDTH;
	const float STRING_HEIGHT;
	const float STRING_X;
	const float STRING_Y;
	Polygon2D	polyString;

	// 制御用
	// 定数
	const int STRING_FLASH_INTERVAL_NORMAL;	// 文字の点滅間隔(フレーム数) 
	const int STRING_FLASH_INTERVAL_SELECTED;

	int		cntStringDraw;	// 文字を描画したフレーム数
	bool	isStringDraw;	// 文字を描画するフラグ(true:描画　false:描画しない)

	int		cnt;		// 総フレーム
	int		cntStart;	// 測定開始時の総フレームを取得

	//----------------------------------------------------------
	// 内部メソッド
	// 画面遷移
	void updateGameScene();	// ゲームシーンの更新の身行う(true:遷移　false:何もない)

	// クラス内遷移ごとの描画
	void drawTitleStart();
	void drawTitleComplete();
	void drawGameStart();
	void drawTitleEnd();

	void updateTitleStart();
	void updateTitleComplete();
	void updateGameStart();
	void updateTitleEnd();


public:
	Title();
	virtual ~Title();

	bool getIsSceneEnd(){ return isSceneEnd; }

	void init();

	void draw();
	void update();

};

#endif