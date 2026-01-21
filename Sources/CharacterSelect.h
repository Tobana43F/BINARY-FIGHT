//==========================================================
// キャラクター選択画面クラス(汎用)
#ifndef _CHARACTER_SELECT_H
#define _CHARACTER_SELECT_H

#include	"Polygon.h"				// ポリゴンクラスをクラス内で使用するため
#include	"MyTexture.h"			// テクスチャを指定する定数を参照するため
#include	"BattleConfigInfo.h"	// このクラスに値を代入するため
#include	"TextureLoader.h"		// テクスチャを使用するため
#include	"MySound.h"

class CharacterSelect{
protected:
	// 画面遷移の制御用
	bool isSceneEnd;

	// このクラスでの操作を記憶するため、専用クラスへのポインタを持ち、そのクラスに情報を代入
	BattleConfigInfo *pBattleConfigInfo;

	// 選択肢関係
	const char	BOX_DIV_X;// = 2;		// 横に並べる数
	const char	BOX_DIV_Y;// = 1;		// 縦に並べる数

	const char	CHARACTER_NUM;// = 2;	// 選択可能なキャラクター数(選択肢の数)

	const float BOX_GAP_WIDTH;			// ボックス間の隙間
	const float BOX_GAP_HEIGHT;

	const float BOX_WIDTH;// = 70;		// 選択肢の大きさ
	const float BOX_HEIGHT;// = 100;
	const float BOX_BASE_X;// = 100;	// 描画する基準点X
	const float BOX_BASE_Y;// = 100;	
	
	// 背景
	Polygon2D	polyBack;

	// 選択カーソル(フレーム)関係
	// p1
	char curPlayer1Cursor;			// 選択したキャラクターとなる
	bool isPlayer1Selected;			// 選択済みか
	Polygon2D polyPlayer1Cursor;	// 選択中のフレーム表示用ポリゴン

	// p2
	char curPlayer2Cursor;
	bool isPlayer2Selected;
	Polygon2D polyPlayer2Cursor;


	//----------------------------------------------------------
	// テクスチャ関係
	enum eTexList{
		CHAR_DOTS,
		CHAR_RANDOM,
		CHAR_RESERVE_2,
		CHAR_RESERVE_3,
		CHAR_RESERVE_4,
		CHAR_RESERVE_5,
		CHAR_RESERVE_6,
		CHAR_RESERVE_7,


		PLAYER1,
		PLAYER2,
		BACK,
		MAX_TEX,
	};

	TextureLoader	*pTextureLoader;
	
	//----------------------------------------------------------
	// サウンド
	soundNS::SOUND_LABEL	labelDecideSE;
	soundNS::SOUND_LABEL	labelCancelSE;
	soundNS::SOUND_LABEL	labelSelectSE;	
	soundNS::SOUND_LABEL	labelBGM;


	//----------------------------------------------------------
	// 描画機能の分割
	void drawCharacterBox();		// キャラクターの選択肢の描画
	virtual void drawFrame();				// 選択中のフレームを表示 

	//----------------------------------------------------------
	// 更新処理の分割
	virtual void updateGameScene();				// ゲームシーンの遷移をする関数
	virtual void updateCursor();				// 選択中フレームの更新

	void updatePlayer1Cursor();
	void updatePlayer2Cursor();

public:
	CharacterSelect(BattleConfigInfo *_pBattleConfigInfo);
	virtual ~CharacterSelect();

	bool getIsSceneEnd(){ return isSceneEnd; }

	//----------------------------------------------------------
	// 描画
	virtual void draw();

	//----------------------------------------------------------
	// 更新
	virtual void update();
};

#endif