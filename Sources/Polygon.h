//==========================================================
// ポリゴン関係
#ifndef _POLYGON_H
#define	_POLYGON_H

#include	"MyDefinitions.h"
#include	"MyTexture.h"
#include	"MyAnimation.h"

//==========================================================
// 定数
//----------------------------------------------------------
// アニメーションの終端記号
#define	ANM_END			(0xFE)
#define	ANM_LOOP		(0xFF)
#define ANM_NULL		(0xFD)	// 空の場合

//==========================================================
// 構造体
//----------------------------------------------------------
// 頂点用構造体(DirectXの形式に従うため、変更禁止)
struct VERTEX2D{							
	float		x, y, z;
	float		rhw;
	GXColor		diffuse;
	float		tx, ty;
};

//----------------------------------------------------------
// テクスチャの情報を保持する構造体
class TextureUV{
private:

public:
	TPLPalettePtr		pTexPal;			// 実際にテクスチャを読み込む場所
	GXTexObj			tex;				// テクスチャ
	float				hLen, vLen;			// 1辺の長さ(UV値)
	int					hDiv, vDiv;			// 1辺の分割数(画像の枚数)

	// サイズは必須の情報ではないが、あれば便利かも
	int					hSize, vSize;		// 画像全体の大きさ(最大1024*1024)
	float				hDivSize, vDivSize;	// 分割後の1テクスチャあたりの大きさ(Size / Div)

	int					**anmData;			// アニメーションの配列データポインタを保持する変数
	int					anmNum;				// アニメーション数

	//==========================================================
	// 関数
	TextureUV();
	virtual ~TextureUV();

	//==========================================================
	// テクスチャ関係
	void loadTexture(const char *resource, int hDiv, int vDiv, unsigned short _hSize, unsigned short _vSize);	// 1画像に複数枚の画像がある場合
	void loadTexture(textureNS::eTextureLabel _label);
	void releaseTexture();

	//==========================================================
	// アニメーション関係
	void loadAnmData(animationNS::eAnimationLabel _label);	// アニメーションの読込(仮)
	void releaseAnmData();

	//==========================================================
	// ゲッター
	int  getAnmNo(int anmNo, int anmCnt);	// 指定のアニメーションのパターンから、テクスチャを取得

	float getHDivSizeRatio();	// 横の分割数の比率を取得(横:縦)
	float getVDivSizeRatio();	// ↑の逆…(逆数になる)
};

//----------------------------------------------------------
// ポリゴンの描画に必要な座標を格納する構造体(テクスチャのUV座標を含む)
// 全てfloat型
struct FCoord2D{
	float	x, y;					// ポリゴンの左上頂点の座標
	float	w, h;					// ポリゴンの幅と高さ
};

//==========================================================
// クラス
//----------------------------------------------------------
// ポリゴンに関するクラス
class Polygon2D{
public:
	FCoord2D			coord;		// ポリゴンの座標
	GXColor				col;		// 頂点カラー
	TextureUV			*pTex;		// テクスチャを読み込み、読み込んだテクスチャの情報を格納
	int					anmCnt;		// アニメーションカウンタを指定
	int					anmNo;		// アニメーションを指定
	bool				isAnmEnd;	// アニメーションの終了フラグ　true:終了(ANM_END) false:更新中
	bool				isReverseX;	// x方向反転フラグ	true:左向(反対方向) false:右向(正方向) 
	bool				isReverseY;	// y方向反転フラグ　true:下(反転)　false:上(正方向)

	//----------------------------------------------------------
	// 関数
	Polygon2D();
	virtual ~Polygon2D();

	// セッター系
	void setData(float x, float y, float w, float h);			// 座標、幅の指定
	void setTexturePointer(TextureUV *src);						// 使用するテクスチャへのポインタをセット

	void changeVertexColor(unsigned char r, unsigned char g, unsigned char b);
	void changeAnmNo(int _anmNo);								// アニメーション変更

	void setIsReverseX(bool state);
	void setIsReverseY(bool state);


	// ゲッター系
	int  getAnmNo();											// テクスチャを取得
	
	// 描画
	void drawPolygon();											// ポリゴンを描画

	// 更新
	void updatePolygon();										// ポリゴン更新(アニメーションの更新)
};

#endif