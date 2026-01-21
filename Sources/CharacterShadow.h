#ifndef _CHARACTER_SHADOW_H
#define _CHARACTER_SHADOW_H

#include	"Actor.h"	// 参照するため
#include	"Polygon.h"	// ポリゴン
#include	"TextureLoader.h"	// テクスチャを使用する

//==========================================================
// 影クラス
class CharacterShadow{
private:
	const float SMALL_RATIO;	// 縮小率

	float		*pGroundPosY;	// 参照する地面座標

	Actor		*pPlayer1;		// 参照するプレイヤー
	Actor		*pPlayer2;

	// テクスチャ
	enum eTexList{
		SHADOW,
		MAX_TEX,
	};
	TextureLoader	*pTextureLoader;

	void updateShadowPos();	// 座標の更新

public:
	// 影描画用ポリゴン
	Polygon2D	polyPlayer1;
	F2Value		initPlayer1ShadowSize;	// キャラクターごとに、初期サイズを受け取る

	Polygon2D	polyPlayer2;
	F2Value		initPlayer2ShadowSize;
	
	//==========================================================
	// 関数
	CharacterShadow(Actor *_pPlayer1, Actor *_pPlayer2, float *_pGroundPosY);
	virtual ~CharacterShadow();


	void init();
	void load();

	void draw();
	void update();

};

#endif