#include	"BoundingBox.h"
#include	"ShareBaseFunc.h"

#include	"Common.h"	// グローバル変数

//==========================================================
// 関数定義
//----------------------------------------------------------
// コンストラクタ
BoundingBox::BoundingBox(){
	isActive = false;
	isHit = false;
	changeVertexColor(0xFF, 0xFF, 0xFF);
	pTex = NULL;
}

//----------------------------------------------------------
// デストラクタ
BoundingBox::~BoundingBox(){

}

//----------------------------------------------------------
// Setter
void BoundingBox::setTexturePointer(TextureUV *src){
	pTex = src;
}

void BoundingBox::changeVertexColor(unsigned char r, unsigned char g, unsigned char b){
	setMyGXColor(&col, r, g, b);
}

//----------------------------------------------------------
// ポリゴン描画
void BoundingBox::drawPolygon(FCoord2D *coord){
#ifdef DIRECTX
	setVertexFormat();																// 頂点フォーマットを指定
#endif
	// if(テクスチャパターンが存在しているとき)
	if (pTex != NULL)
		setTexture(pTex->tex);			// 描画するにテクスチャをセット

	unsigned char r = getMyGXColor(&col, RED);
	unsigned char g = getMyGXColor(&col, GREEN);
	unsigned char b = getMyGXColor(&col, BLUE);

	drawQuadPrimitive(coord->x, coord->y, coord->w, coord->h,
		r,g,b,
		pTex,
		0
		);
}

//----------------------------------------------------------
// メンバへのアクセス
void BoundingBox::setIsHit(bool state){
	isHit = state;
}

void BoundingBox::setIsActive(bool state){
	isActive = state;
}
