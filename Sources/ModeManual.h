//==========================================================
// マニュアル画面
#ifndef _MODE_MANUAL_H
#define _MODE_MANUAL_H

#include	"TextureLoader.h"
#include	"Polygon.h"
#include	"MySound.h"

class ModeManual{
private:
	bool	isSceneEnd;

	// クラス内遷移
	enum eScene{
		MANUAL_START,
		MANUAL_COMPLETE,
		MANUAL_END,
		END,
	}scene;

	// テクスチャ
	enum eTexList{
		PAGE_1,
		PAGE_2,
		PAGE_3,
		PAGE_4,
		MAX_TEX,
	};
	TextureLoader	*pTextureLoader;

	// ラベル
	soundNS::SOUND_LABEL	labelPageFlip;

	// 制御用
	const int PAGE_NUM;
	int curPage;

	// クラスないメソッド
	void drawPage();

	void updateCursor();

	void interruptReturnToTitle();
public:
	ModeManual();
	virtual ~ModeManual();

	//----------------------------------------------------------
	// ゲッター
	bool getIsSceneEnd(){ return isSceneEnd; }

	//----------------------------------------------------------
	// 基本的な関数
	void init();
	void load();
	void draw();
	void update();
};


#endif