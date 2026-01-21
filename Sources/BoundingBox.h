//==========================================================
// 当たり判定用クラス
#ifndef _BOUNDINGBOX_H
#define _BOUNDINGBOX_H

#include	"Polygon.h"	// TextrueUVクラス、 FCoord2D構造体をメンバに持つため
#include	"AttackInfo.h"

//==========================================================
// クラス定義
// 基底クラス : ボックス一つあたりの、当たり判定座標と、判定状態。
class BoundingBox{
public:
	// 判定座標のデータ
	FCoord2D	coord;

	// 判定状態を格納する変数
	bool			isActive;			// true:判定する　false: 判定しない(使わない予定)
	bool			isHit;				// true:衝突　false:衝突していない

	// デバッグ時に必要
	GXColor			col;				// 判定時に色を変えるときなど
	TextureUV		*pTex;				// デバッグ時に見えるように

	// 関数
	BoundingBox();
	virtual ~BoundingBox();

	void setTexturePointer(TextureUV *src);	// テクスチャデータへのポインタ
	void changeVertexColor(unsigned char r, unsigned char g, unsigned char b);

	void drawPolygon(FCoord2D *coord);

	void setIsHit(bool state);		// privateへのアクセス
	void setIsActive(bool state);	// 〃

};
//----------------------------------------------------------
// リスト用
class BoundingBoxList : public BoundingBox {
public:
	BoundingBoxList *next;	// 次ポインタ
};

//==========================================================
// 壁判定用(画面端の判定)
class WallBoundingBox : public BoundingBox{
public:
	// 特になし
};

//----------------------------------------------------------
// リストで使用することは無いので、作らない

//==========================================================
// 体判定用BoundingBox
class BodyBoundingBox : public BoundingBox{
public:
	// 特になし

};

//----------------------------------------------------------
// リスト用
class BodyBoundingBoxList : public BodyBoundingBox{
public:
	BodyBoundingBoxList *next;
};

//==========================================================
// 喰らい判定用BoundingBox。(名前を分けるため)
class HurtBoundingBox : public BoundingBox{
public:
	// 特になし
};
//----------------------------------------------------------
// リスト用
class HurtBoundingBoxList : public HurtBoundingBox{
public:
	HurtBoundingBoxList *next;
};

//==========================================================
// 攻撃判定用BoundingBox
class AttackBoundingBox : public BoundingBox{
public:
	// その判定ボックスの攻撃情報
	AttackInfo attackInfo;
};
//----------------------------------------------------------
// リスト用
class AttackBoundingBoxList : public AttackBoundingBox{
public:
	AttackBoundingBoxList *next;
};

//==========================================================
// ガード判定用BoundingBox(相手をガードモーションにする判定)
class GuardBoundingBox : public BoundingBox{
public:

};

//----------------------------------------------------------
// リスト用
class GuardBoundingBoxList : public GuardBoundingBox{
public :
	GuardBoundingBoxList *next;
};

#endif // !_BOUNDINGBOX_H