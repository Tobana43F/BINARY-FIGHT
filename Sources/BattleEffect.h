//==========================================================
// 戦闘中のエフェクト関係
#ifndef _BATTLE_EFFECT_H
#define _BATTLE_EFFECT_H

#include	"Polygon.h"			// ポリゴンを扱うため
#include	"TextureLoader.h"	// テクスチャを使用するため
#include	"MyTexture.h"		// 〃
#include	"Actor.h"

#include	"AttackInfo.h"		// 攻撃情報を参照して、サイズなどを決めるため

//==========================================================
// 定数

// ポリゴンのサイズ(仮… 攻撃の強さ、属性によって固定的に指定する)
#define EFFECT_WIDTH	(50)
#define EFFECT_HEIGHT	(50)

//==========================================================
// エフェクトクラス
// 表示するポリゴンなどをリスト構造として持つ
class EffectList{
private:

public:
	Polygon2D	poly;
	EffectList	*next;

	EffectList();
	virtual ~EffectList();

	// セッター
	void setData(FCoord2D *_pSrc, TextureUV *_src);	// 座標の設定
	void changeAnmNo(int _anmNo);

	void draw();
	void update();
};


//==========================================================
// エフェクトクラスを管理するクラス
//  テクスチャを共有するため、
//  エフェクトリストのポリゴンのテクスチャはこのクラス内のテクスチャを参照する。
class BattleEffect{
private:
	const float LOW_DAMAGE_SIZE;
	const float MID_DAMAGE_SIZE;
	const float HIGH_DAMAGE_SIZE;

	EffectList	*pEffectListTop;	// リストの戦闘を保持

	// テクスチャ
	enum eAnmNoList{	// アニメーションの指定に使う
		NORMAL,
		MAX_ANM_NO_LIST,
	};

	enum eTexList{
		HIGH_ATTACK,	// 強攻撃用テクスチャ
		HIGH_GUARD,		// ガード
		MAX_TEX,
	};
	TextureLoader	*pTextureLoader;

	//----------------------------------------------------------
	// ゲッター
	EffectList* getEffectListDataTop();		// ダミーデータの次からが実際のデータ
	EffectList* getEffectListTop();			// 先頭取得

	void getDataFromAttackInfo(FCoord2D *_pCoordDest, eTexList *_pTexListDest, AttackInfo *_pAttackInfo);

	//----------------------------------------------------------
	// リスト操作
	void freeAllEffectList();	// 初期化(全リスト開放)

public:
	BattleEffect();
	virtual ~BattleEffect();



	//----------------------------------------------------------
	// リスト操作
	void addEffectList(float _centerX, float _centerY, AttackInfo *_attackInfo);// リストの追加(最後尾へ)
	void addEffectList(Actor *_pActor, AttackInfo *_attackInfo);
	void freeEffectList(EffectList *_target);									// リストを開放(指定位置)

	void init();

	void load();

	void draw();
	void update();
};

#endif