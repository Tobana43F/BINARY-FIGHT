#include	"AbstractStage.h"	// クラスのメソッドを定義するため。

#include	"ShareBaseFunc.h"	// 描画関数用
#include	"Constants.h"		// 画面サイズを知るため
#include	"Polygon.h"			// ポリゴンを使用するため
#include	"TextureLoader.h"	// テクスチャを使用するため
#include	"MyTexture.h"		// テクスチャファイルの指定に使用する
#include	"Actor.h"			// Actorクラスを引数に持つため

#include	"Debug.h"			// エラー処理

#include	<math.h>			// abs()をつかつため

//==========================================================
// AbstractStage
//----------------------------------------------------------
// コンストラクタ
AbstractStage::AbstractStage() :
INIT_GROUND_POS_Y(WINDOW_HEIGHT * 0.9),
INIT_PLAYER_GAP_WIDTH(WINDOW_WIDTH * 0.5f),
STAGE_HEIGHT(800.0f),	//800f
STAGE_WIDTH(1600.0f),	//1600
INIT_STAGE_POS_X(WINDOW_WIDTH - STAGE_WIDTH / 2.0f - WINDOW_WIDTH / 2.0f),
INIT_STAGE_POS_Y(WINDOW_HEIGHT - STAGE_HEIGHT),
SCROLL_LIMIT_POS_LEFT((0.0f)),
SCROLL_LIMIT_POS_RIGHT((WINDOW_WIDTH - STAGE_WIDTH)),
SCROLL_LIMIT_POS_TOP((0.0f)),
SCROLL_LIMIT_POS_BOTTOM((WINDOW_HEIGHT - STAGE_HEIGHT)),
SCROLL_POS_LEFT((WINDOW_WIDTH / 2.0f) - (WINDOW_WIDTH / 80.0f)),
SCROLL_POS_RIGHT((WINDOW_WIDTH / 2.0f) + (WINDOW_WIDTH / 80.0f)),
SCROLL_POS_TOP((WINDOW_HEIGHT * 0.4f)),
SCROLL_POS_BOTTOM((WINDOW_HEIGHT * 0.4f)),
HEIGHT_RATIO(WINDOW_HEIGHT / STAGE_HEIGHT),		// メンバ宣言の時の順番に気をつける。
WIDTH_RATIO(WINDOW_WIDTH / STAGE_WIDTH)
{
	// プレイヤーの初期化状態をセット
	isPlayerInited = false;

	// 適当な初期化
	texU = 0.0f;
	texV = 0.0f;

	isStageLimitX = false;
	isStageLimitY = false;

	soundLabelBGM = soundNS::STAGE_TRAINING;	// 適当な初期化(無音のファイルがあるならそれのほうが良いと思う)
}

//----------------------------------------------------------
// デストラクタ
AbstractStage::~AbstractStage(){
	delete pTextureLoader;
	pTextureLoader = NULL;
}

//----------------------------------------------------------
// プレイヤーの参照設定。(コンストラクタの次に呼び出す必要がある。)
void AbstractStage::initPlayerPointer(Actor *_player1, Actor *_player2){
	// 参照するプレイヤーの座標のポインタを設定
	player1 = _player1;
	player2 = _player2;
	isPlayerInited = true;
}

//----------------------------------------------------------
// 初期化(プレイヤーの参照を設定以降に呼び出す)
void AbstractStage::init(){
	// 一応エラー処理
	if (!isPlayerInited)
		outErrorMessage("ステージクラスの参照プレイヤーが設定されていません。");

	// 地面の初期座標を代入
	groundPosY = INIT_GROUND_POS_Y;

	// 右側のプレイヤーの初期状態を設定(仮)
	*player2->posState = Actor::RIGHT;
	*player2->dirState = Actor::LEFT;

	// プレイヤーを初期座標に移動 
	player1->coord.y = (groundPosY + player1->coord.h);
	player2->coord.y = (groundPosY + player2->coord.h);

	player1->coord.x = (WINDOW_WIDTH / 2.0f) - (INIT_PLAYER_GAP_WIDTH / 2.0f) - (player1->coord.w / 2.0f);
	player2->coord.x = (WINDOW_WIDTH / 2.0f) + (INIT_PLAYER_GAP_WIDTH / 2.0f) - (player2->coord.w / 2.0f);

	// 基準座標を計算
	updateBaseCoord();
	charBaseCoord.w = 0;	// 点として使用する
	charBaseCoord.h = 0;

	// 背景位置をリセット
	initStageScroll();
	stageBaseCoord.w = 0;	// 点として使用する
	stageBaseCoord.h = 0;

	// 背景用ポリゴンの初期化										// 背景用ポリゴンを確保
	polyBackGround.setData(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);		// ポリゴンの大きさは、画面に合わせる。
	
	// 頂点カラーの変更
	polyBackGround.changeVertexColor(0xFF, 0xFF, 0xFF);
}

//----------------------------------------------------------
// 初期化
void AbstractStage::load(){
	// 読み込み
	pTextureLoader->loadUseTexture();
}

//----------------------------------------------------------
// 画面スクロールのリセット
void AbstractStage::initStageScroll(){
	stageBaseCoord.x = INIT_STAGE_POS_X;
	stageBaseCoord.y = INIT_STAGE_POS_Y;

	updateStagePolygonUV();

	isStageLimitX = false;
	isStageLimitY = false;
}

//----------------------------------------------------------
// 基準座標を更新する
void AbstractStage::updateBaseCoord(){
	// 判定しやすいように、中心座標を使う
	float player1X = player1->coord.x + (player1->coord.w / 2.0f);
	float player1Y = player1->coord.y + (player1->coord.h / 2.0f);
	float player2X = player2->coord.x + (player2->coord.w / 2.0f);
	float player2Y = player2->coord.y + (player2->coord.h / 2.0f);

	charBaseCoord.x = (player2X - player1X) / 2.0f + player1X;
	charBaseCoord.y = (player2Y - player1Y) / 2.0f + player1Y;
}

//----------------------------------------------------------
// 画面のスクロール
void AbstractStage::updateScroll(){
	//==========================================================
	// X軸方向のスクロール
	//  左
	if (charBaseCoord.x < SCROLL_POS_LEFT){
		float diffX = SCROLL_POS_LEFT - charBaseCoord.x;	// スクロール開始座標との差を取得

		// ステージ(背景)を動かす(変更)
		stageBaseCoord.x += diffX;

		// ステージ上のオブジェクトを全て動かす
		player1->coord.x += diffX;
		player2->coord.x += diffX;
	}
	// 右
	else if (charBaseCoord.x > SCROLL_POS_RIGHT){
		float diffX = SCROLL_POS_RIGHT - charBaseCoord.x;	// スクロール開始座標との差を取得

		// ステージ(背景)を動かす
		stageBaseCoord.x += diffX;

		// ステージ上のオブジェクトを全て動かす
		player1->coord.x += diffX;
		player2->coord.x += diffX;
	}
	//==========================================================
	// Y軸
	// 上
	if (charBaseCoord.y < SCROLL_POS_TOP){
		float diffY = SCROLL_POS_TOP - charBaseCoord.y;
		// ステージのスクロール
		stageBaseCoord.y += diffY;

		player1->coord.y += diffY;
		player2->coord.y += diffY;

		groundPosY += diffY;
	}
	else if (charBaseCoord.y > SCROLL_POS_BOTTOM){
		float diffY = SCROLL_POS_BOTTOM - charBaseCoord.y;
		stageBaseCoord.y += diffY;

		player1->coord.y += diffY;
		player2->coord.y += diffY;

		groundPosY += diffY;
	}
}

//----------------------------------------------------------
// スクロール上限(下限)を超えた場合の処理。
// 処理はほぼスクロールと同じ
void AbstractStage::updateScrollLimit(){
	//==========================================================
	// X
	if (stageBaseCoord.x > SCROLL_LIMIT_POS_LEFT){
		float diffX = SCROLL_LIMIT_POS_LEFT - stageBaseCoord.x;

		// ステージ(背景)を動かす
		stageBaseCoord.x += diffX;

		// ステージ上のオブジェクトを全て動かす
		player1->coord.x += diffX;
		player2->coord.x += diffX;

		// 画面端フラグをセット
		isStageLimitX = true;
	}
	else if (stageBaseCoord.x < SCROLL_LIMIT_POS_RIGHT){
		float diffX = SCROLL_LIMIT_POS_RIGHT - stageBaseCoord.x;

		// ステージ(背景)を動かす
		stageBaseCoord.x += diffX;

		// ステージ上のオブジェクトを全て動かす
		player1->coord.x += diffX;
		player2->coord.x += diffX;

		// 画面端フラグをセット
		isStageLimitX = true;
	}
	else{
		isStageLimitX = false;
	}
	//==========================================================
	// Y
	if (stageBaseCoord.y > SCROLL_LIMIT_POS_TOP){
		float diffY = SCROLL_LIMIT_POS_TOP - stageBaseCoord.y;

		// ステージ(背景)を動かす
		stageBaseCoord.y += diffY;

		// ステージ上のオブジェクトを全て動かす
		player1->coord.y += diffY;
		player2->coord.y += diffY;

		// 地面
		groundPosY += diffY;
		
		isStageLimitY = true;
	}
	else if (stageBaseCoord.y < SCROLL_LIMIT_POS_BOTTOM){
		float diffY = SCROLL_LIMIT_POS_BOTTOM - stageBaseCoord.y;

		// ステージ(背景)を動かす
		stageBaseCoord.y += diffY;

		// ステージ上のオブジェクトを全て動かす
		player1->coord.y += diffY;
		player2->coord.y += diffY;

		// 地面
		groundPosY += diffY;

		isStageLimitY = true;
	}
	else{
		isStageLimitY = false;
	}
}

//----------------------------------------------------------
// 背景用ポリゴンのUV値の更新
void AbstractStage::updateStagePolygonUV(){
	texU = (float)((fabs(stageBaseCoord.x)) / STAGE_WIDTH);
	texV = (float)((fabs(stageBaseCoord.y)) / STAGE_HEIGHT);

	// 誤差の補正(念のため)
	if (texU < 0.0f)
		texU = 0.0f;
	else if (texU + WIDTH_RATIO > 1.0f)
		texU = 1.0f - WIDTH_RATIO;
	if (texV < 0)
		texV = 0;
	else if (texV + HEIGHT_RATIO > 1.0f)
		texV = 1.0f - HEIGHT_RATIO;
}

//----------------------------------------------------------
// 描画
void AbstractStage::draw(){
	//pPolyBackGround->drawPolygon();
	drawQuadPrimitive(
		&polyBackGround,
		texU, texU + WIDTH_RATIO, texU + WIDTH_RATIO, texU,
		texV, texV, texV + HEIGHT_RATIO, texV + HEIGHT_RATIO);

}

//----------------------------------------------------------
// 更新
void AbstractStage::update(){
	polyBackGround.updatePolygon();

	updateBaseCoord();		// 基準点の更新
	updateScroll();			// スクロール
	updateScrollLimit();	// 下限・上限でスクロールをとめる

	updateStagePolygonUV();	// 実際に絵をスクロールさせる
}
