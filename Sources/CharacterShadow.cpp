#include	"CharacterShadow.h"	// メソッド定義
#include	"MyTexture.h"		// テクスチャ

//==========================================================
// CharacterShadowクラス
//----------------------------------------------------------
// コンストラクタ
CharacterShadow::CharacterShadow(Actor *_pPlayer1, Actor *_pPlayer2, float *_pGroundPosY):
SMALL_RATIO(400.0f)
{
	// テクスチャ
	pTextureLoader = new TextureLoader(MAX_TEX);
	pTextureLoader->setUseTextureLabel(SHADOW, textureNS::CHARACTER_SHADOW);

	// 参照先を設定
	pGroundPosY = _pGroundPosY;
	pPlayer1 = _pPlayer1;
	pPlayer2 = _pPlayer2;

	// 初期値を代入 仮)キャラクターが個別に持つようにする(Widthのみ)
	initPlayer1ShadowSize.x = 200;
	initPlayer1ShadowSize.y = 50;

	initPlayer2ShadowSize.x = 200;
	initPlayer2ShadowSize.y = 50;

	// テクスチャをセット
	polyPlayer1.setTexturePointer(pTextureLoader->getTexturePointer(SHADOW));
	polyPlayer2.setTexturePointer(pTextureLoader->getTexturePointer(SHADOW));

	init();
}

//----------------------------------------------------------
// デストラクタ
CharacterShadow::~CharacterShadow(){
	// 参照するだけなので、参照しなくするだけ。
	pGroundPosY = NULL;
	pPlayer1 = NULL;
	pPlayer2 = NULL;

	// テクスチャは開放
	delete pTextureLoader;
	pTextureLoader = NULL;
}

//----------------------------------------------------------
// 初期化
void CharacterShadow::init(){
	polyPlayer1.setData(0, 0, initPlayer1ShadowSize.x, initPlayer1ShadowSize.y);
	polyPlayer2.setData(0, 0, initPlayer2ShadowSize.x, initPlayer2ShadowSize.y);

	updateShadowPos();
}

//----------------------------------------------------------
// テクスチャのロード
void CharacterShadow::load(){
	pTextureLoader->loadUseTexture();
}

//----------------------------------------------------------
// 影ポリゴンの座標の更新
void CharacterShadow::updateShadowPos(){
	float player1CenterPosX;	// キャラの中心X座標
	float player2CenterPosX;

	float player1GroundPosYDistance;	// キャラと地面の距離
	float player2GroundPosYDistance;

	// 中心座標を求める
	player1CenterPosX = pPlayer1->coord.x + pPlayer1->coord.w / 2.0f;	
	player2CenterPosX = pPlayer2->coord.x + pPlayer2->coord.w / 2.0f;

	// キャラと地面までの距離を求める
	player1GroundPosYDistance = *pGroundPosY - pPlayer1->coord.y - pPlayer1->coord.h;
	player2GroundPosYDistance = *pGroundPosY - pPlayer2->coord.y - pPlayer2->coord.h;

	// ポリゴンに反映
	// p1 
	polyPlayer1.coord.w = initPlayer1ShadowSize.x * (1.0f - (player1GroundPosYDistance / SMALL_RATIO));
	if (polyPlayer1.coord.w < 0)
		polyPlayer1.coord.w = 0;
	polyPlayer1.coord.h = initPlayer1ShadowSize.y * (1.0f - (player1GroundPosYDistance / SMALL_RATIO));
	if (polyPlayer1.coord.h < 0)
		polyPlayer1.coord.h = 0;

	polyPlayer1.coord.x = player1CenterPosX - polyPlayer1.coord.w / 2.0f;								// 中心座標から、影の幅の半分を引く
	polyPlayer1.coord.y = *pGroundPosY - polyPlayer1.coord.h / 2.0f - initPlayer1ShadowSize.y * 0.2f;	// 0.2は微調整

	// p2
	polyPlayer2.coord.w = initPlayer2ShadowSize.x * (1.0f - (player2GroundPosYDistance / SMALL_RATIO));
	if (polyPlayer2.coord.w < 0)
		polyPlayer2.coord.w = 0;
	polyPlayer2.coord.h = initPlayer2ShadowSize.y * (1.0f - (player2GroundPosYDistance / SMALL_RATIO));
	if (polyPlayer2.coord.h < 0)
		polyPlayer2.coord.h = 0;

	polyPlayer2.coord.x = player2CenterPosX - polyPlayer2.coord.w / 2.0f;
	polyPlayer2.coord.y = *pGroundPosY - polyPlayer2.coord.h / 2.0f - initPlayer2ShadowSize.y * 0.2f;
}

//----------------------------------------------------------
// 描画
void CharacterShadow::draw(){
	polyPlayer1.drawPolygon();
	polyPlayer2.drawPolygon();
}

//----------------------------------------------------------
// 更新
void CharacterShadow::update(){
	updateShadowPos();
}
