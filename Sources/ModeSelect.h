//==========================================================
// モードセレクト(トップメニュー)画面クラス
#ifndef _MODE_SELECT_H
#define _MODE_SELECT_H

//==========================================================
// インクルード
#include	"BattleConfigInfo.h"	// モードの定数を参照するため,クラスへの値渡し

#include	"Polygon.h"				// ポリゴンの使用
#include	"TextureLoader.h"		// テクスチャを使用するため
#include	"MyTexture.h"			// 使用するテクスチャの参照
#include	"MySound.h"

//==========================================================
// クラス定義
class ModeSelect{
private:
	bool isSceneEnd;
	//----------------------------------------------------------
	// クラス内遷移
	enum {
		MENU_START,
		MENU_SELECTING,
		MENU_SELECTED,
		MENU_END,
	}scene;
	//----------------------------------------------------------
	// 項目の定数
	static const int STRING_NUM = 30;	// 文字配列の数
	const float FONT_WIDTH;
	const float FONT_HEIGHT;
	const float FONT_BASE_X;		// 左上のｘ
	const float FONT_BASE_Y;		// 左上のｙ
	const float MENU_GAP_HEIGHT;	// メニュー間の隙間の大きさ

	// メニューの文字列
	const int MENU_NUM;	// 選択項目の数
	const char **allMenuStringPointer;			// 全てのメニューの文字列へのポインタをまとめる役目(コンストラクタでMENU_NUM個確保する)
	const char *STRING_ARCADE_MODE;
	const char *STRING_VERSUS_MODE;
	const char *STRING_TUTORIAL_MODE;
	const char *STRING_MANUAL_MODE;

	// 制御用
	const float CURSOR_FLASH_SECOND;				// 点滅させる時間(秒)
	const int	CURSOR_FLASH_INTERVAL_SELECTED;	// 選択時の点滅速度

	float		pullX;	// 選択後、選択されてないものを動かす

	int cntStart;	// 制御するときの開始時のフレームを保持
	int cnt;		// クラス内カウンタ

	//----------------------------------------------------------
	// テクスチャ関係
	enum eTexList{
		FONT_TEX,
		CURSOR_TEX,			// カーソル用テクスチャ
		BACK,
		STRING_ARCADE,
		STRING_VERSUS,
		STRING_TUTORIAL,
		STRING_MANUAL,
		MAX_TEX
	};
	TextureLoader	*pTextureLoader;

	//----------------------------------------------------------
	// サウンド
	soundNS::SOUND_LABEL	labelCursorSE;
	soundNS::SOUND_LABEL	labelDecideSE;
	soundNS::SOUND_LABEL	labelBGM;

	//----------------------------------------------------------
	// クラス内で使用する変数
	Polygon2D	polyBack;
	Polygon2D	polyDescription;

	Polygon2D	polyCursor;
	int curCursor;		// 現在のカーソル位置
	bool isSelected;	// 決定フラグ

	//----------------------------------------------------------
	// 描画
	void drawMenuSelecting();
	void drawMenuSelected();

	//----------------------------------------------------------
	// 更新
	void updateCursor();
	void updateMenuSelected();
	void updateGameScene();

	void interruptReturnToTitle();		// + - 同時押しで、タイトルに戻る

public:
	ModeSelect();
	virtual ~ModeSelect();

	bool getIsSceneEnd() { return isSceneEnd; }

	void init();
	void load();
	void draw();
	void update();
};

#endif