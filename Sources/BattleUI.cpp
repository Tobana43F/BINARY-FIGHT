#include	"BattleUI.h"		// メソッドの定義を行うため
#include	"Actor.h"			// Actorクラスを引数に使用するため
#include	"MyTexture.h"		// テクスチャのラベルを参照するため
#include	"Polygon.h"			// ポリゴンを使用するため
#include	"ShareBaseFunc.h"	// ポリゴン描画の関数を使用するため
#include	"BattleConfigInfo.h"// バトル情報を受け渡すため
#include	"TextureLoader.h"	// テクスチャを扱うため

//==========================================================
// HitPointUIクラス
//----------------------------------------------------------
// コンストラクタ
HitPointBarUI::HitPointBarUI(Actor *player1, Actor *player2):
BAR_W(260 * WINDOW_WIDTH_RATIO),
BAR_H(40 * WINDOW_HEIGHT_RATIO),
PLAYER1_BAR_X(((WINDOW_WIDTH / 2.0f) - BAR_W) - (((WINDOW_WIDTH / 2.0f) - BAR_W) * 0.8f)),
PLAYER2_BAR_X((WINDOW_WIDTH / 2.0f) + (((WINDOW_WIDTH / 2.0f) - BAR_W) * 0.8f)),
BAR_Y((50) * WINDOW_HEIGHT_RATIO)
{
	// 参照するHP情報のポインタを設定
	player1MaxHitPoint = &player1->maxHitPoint;
	player1CurHitPoint = &player1->curHitPoint;

	player2MaxHitPoint = &player2->maxHitPoint;
	player2CurHitPoint = &player2->curHitPoint;

	pTextureLoader = new TextureLoader(MAX_TEX);
	// テクスチャラベルの設定
	pTextureLoader->setUseTextureLabel(HP_BAR_FRAME, textureNS::UI_HP_BAR_FRAME);
	pTextureLoader->setUseTextureLabel(HP_BAR_NORMAL, textureNS::UI_HP_BAR_NORMAL);
	pTextureLoader->setUseTextureLabel(HP_BAR_MAX, textureNS::UI_HP_BAR_MAX);
	
}

//----------------------------------------------------------
// デストラクタ
HitPointBarUI::~HitPointBarUI(){
	delete pTextureLoader;
	pTextureLoader = NULL;
}

//----------------------------------------------------------
// 初期化
void HitPointBarUI::init(){
	// ポリゴン情報初期化
	// player1
	polyPlayer1HpBar.setData(PLAYER1_BAR_X, BAR_Y, BAR_W, BAR_H);
	polyPlayer1HpBar.setTexturePointer(pTextureLoader->getTexturePointer(HP_BAR_MAX));
	polyPlayer1HpBar.setIsReverseX(true);

	polyPlayer1HpBarFrame.setData(PLAYER1_BAR_X, BAR_Y, BAR_W, BAR_H);
	polyPlayer1HpBarFrame.setTexturePointer(pTextureLoader->getTexturePointer(HP_BAR_FRAME));
	polyPlayer1HpBarFrame.setIsReverseX(true);

	// player2
	polyPlayer2HpBar.setData(PLAYER2_BAR_X, BAR_Y, BAR_W, BAR_H);
	polyPlayer2HpBar.setTexturePointer(pTextureLoader->getTexturePointer(HP_BAR_MAX));

	polyPlayer2HpBarFrame.setData(PLAYER2_BAR_X, BAR_Y, BAR_W, BAR_H);
	polyPlayer2HpBarFrame.setTexturePointer(pTextureLoader->getTexturePointer(HP_BAR_FRAME));
}

//----------------------------------------------------------
// テクスチャのロード
void HitPointBarUI::load(){
	pTextureLoader->loadUseTexture();	// テクスチャの読込
}

//----------------------------------------------------------
// 描画を行う
void HitPointBarUI::draw(){
	// 体力バーの表示
	float tmpU1 = ((float)*player1CurHitPoint / (float)*player1MaxHitPoint);						// UVの補正値を取得
	polyPlayer1HpBar.coord.w = BAR_W * ((float)*player1CurHitPoint / (float)*player1MaxHitPoint);	// 幅をUVに合わせて変える
	polyPlayer1HpBar.coord.x = PLAYER1_BAR_X + (BAR_W - polyPlayer1HpBar.coord.w);					// X座標をUVに合わせて移動

	drawQuadPrimitive(		// 描画
		&polyPlayer1HpBar,
		tmpU1, 0, 0, tmpU1,
		0.0f, 0.0f, 1.0f, 1.0f);				

	float tmpU2 = ((float)*player2CurHitPoint / (float)*player2MaxHitPoint);						// UVの補正値を取得
	polyPlayer2HpBar.coord.w = BAR_W * ((float)*player2CurHitPoint / (float)*player2MaxHitPoint);	// 幅をUVに合わせて変える

	drawQuadPrimitive(		// 描画
		&polyPlayer2HpBar,
		0, tmpU2, tmpU2, 0,
		0, 0, 1.0f, 1.0f);

	// フレームを表示
	polyPlayer1HpBarFrame.drawPolygon();
	polyPlayer2HpBarFrame.drawPolygon();
}

//----------------------------------------------------------
// 更新を行う
void HitPointBarUI::update(){
	// 使用するテクスチャの変更
	if (*player1CurHitPoint < *player1MaxHitPoint)				// 体力が最大でない場合
		polyPlayer1HpBar.setTexturePointer(pTextureLoader->getTexturePointer(HP_BAR_NORMAL));
	else
		polyPlayer1HpBar.setTexturePointer(pTextureLoader->getTexturePointer(HP_BAR_MAX));
	if (*player2CurHitPoint < *player2MaxHitPoint)
		polyPlayer2HpBar.setTexturePointer(pTextureLoader->getTexturePointer(HP_BAR_NORMAL));
	else
		polyPlayer2HpBar.setTexturePointer(pTextureLoader->getTexturePointer(HP_BAR_MAX));


	polyPlayer1HpBarFrame.updatePolygon();
	polyPlayer2HpBarFrame.updatePolygon();

	polyPlayer1HpBar.updatePolygon();
	polyPlayer2HpBar.updatePolygon();
}

//==========================================================
// TimerUIクラス
//----------------------------------------------------------
// コンストラクタ
TimerUI::TimerUI(BattleConfigInfo *_pBattleConfigInfo):
LETTER_NUM(2),
TIME_LETTER_WIDTH(40.0f),
TIME_LETTER_HEIGHT(80.0f),
TIME_LETTER_X((WINDOW_WIDTH / 2) - (TIME_LETTER_WIDTH*LETTER_NUM / 2.0f)),
TIME_LETTER_Y(20.0f)
{
	pTextureLoader = new TextureLoader(MAX_TEX);
	pTextureLoader->setUseTextureLabel(NUMBER_FONT, textureNS::FONT_DOT_AND_SHADOW);

	maxTime = _pBattleConfigInfo->getRoundTime();
	init();
}

//----------------------------------------------------------
// デストラクタ
TimerUI::~TimerUI(){
	delete pTextureLoader;
	pTextureLoader = NULL;
}

//----------------------------------------------------------
// 初期化
void TimerUI::init(){
	curTime = maxTime;
	timeCounter = 0;
	isTimeOver = false;
}

//----------------------------------------------------------
// ロード
void TimerUI::load(){
	pTextureLoader->loadUseTexture();
}


//----------------------------------------------------------
// 描画
void TimerUI::draw(){
	// 時間制限がない場合は描画しない
	if (maxTime == ROUND_TIME_NO_LIMIT)
		return;

	printfText(TIME_LETTER_X, TIME_LETTER_Y, TIME_LETTER_WIDTH, TIME_LETTER_HEIGHT,
		0xFF, 0xFF, 0xFF,
		pTextureLoader->getTexturePointer(NUMBER_FONT), "%02d", curTime);
}

//----------------------------------------------------------
// 更新
void TimerUI::update(){
	//----------------------------------------------------------
	// 時間制限がない場合は、時間の更新は行わない
	if (maxTime == ROUND_TIME_NO_LIMIT)
		return;

	//----------------------------------------------------------
	// 時間制限がある場合
	timeCounter++;
	if ((timeCounter > (char)DEF_FRAME_RATE)){// 59以下でない場合
		timeCounter = 0;	// カウンタをリセット
		curTime--;

		if (curTime < 0){
			curTime = 0;
			isTimeOver = true;
		}
	}
}

//==========================================================
// RoundIconUIクラス
//----------------------------------------------------------
// コンストラクタ
RoundIconUI::RoundIconUI(BattleConfigInfo *_pBattleConfigInfo):
ICON_WIDTH(20.0f),
ICON_HEIGHT(20.0f),
ICON_GAP_WIDTH(5.0f),
PLAYER1_ICON_BASE_X((WINDOW_WIDTH / 2) - (50.0f) - ICON_WIDTH),
PLAYER2_ICON_BASE_X((WINDOW_WIDTH / 2) + (50.0f)),
ICON_Y(30.0f)
{
	// テクスチャ初期化
	pTextureLoader = new TextureLoader(MAX_TEX);
	pTextureLoader->setUseTextureLabel(ROUND_ICON, textureNS::UI_ROUND_ICON);

	// ラウンド数からアイコンの数を求める
	maxRound = _pBattleConfigInfo->getMaxRound();
	
	//----------------------------------------------------------
	// アイコン数の取得
	switch (maxRound){
		case 1:
			iconNum = 1;
			break;
		case 3:
			iconNum = 2;
			break;
		case 5:
			iconNum = 3;
			break;
		case 7:
			iconNum = 4;
			break;
		default:
			// 仮)後々全てのcaseは定数にする
			break;
	}

	// 表示するアイコンの数のポリゴンクラスを確保
	polyPlayer1RoundIcon = new Polygon2D[iconNum];
	polyPlayer2RoundIcon = new Polygon2D[iconNum];

	init();
}

//----------------------------------------------------------
// デストラクタ
RoundIconUI::~RoundIconUI(){
	delete pTextureLoader;
	pTextureLoader = NULL;
	
	delete[] polyPlayer1RoundIcon;
	polyPlayer1RoundIcon = NULL;
	delete[] polyPlayer2RoundIcon;
	polyPlayer2RoundIcon = NULL;
}

//----------------------------------------------------------
// 初期化
void RoundIconUI::init(){
	// ポリゴンの座標を初期化
	for (int i = 0; i < iconNum; ++i){
		float tmpX = (ICON_WIDTH * i) + (ICON_GAP_WIDTH *i);
		polyPlayer1RoundIcon[i].setData(PLAYER1_ICON_BASE_X - tmpX, ICON_Y, ICON_WIDTH, ICON_HEIGHT);
		polyPlayer1RoundIcon[i].setTexturePointer(pTextureLoader->getTexturePointer(ROUND_ICON));
		polyPlayer1RoundIcon[i].anmNo = NOTHING;	// 特別な使い方をしている(アニメーションで使用しているわけではない)

		polyPlayer2RoundIcon[i].setData(PLAYER2_ICON_BASE_X + tmpX, ICON_Y, ICON_WIDTH, ICON_HEIGHT);
		polyPlayer2RoundIcon[i].setTexturePointer(pTextureLoader->getTexturePointer(ROUND_ICON));
		polyPlayer2RoundIcon[i].anmNo = NOTHING;
	}

	curRound = 1;
}


//----------------------------------------------------------
// ロード
void RoundIconUI::load(){
	pTextureLoader->loadUseTexture();
}


//----------------------------------------------------------
// あきばしょに次のアイコンをセット
// player1
void RoundIconUI::setPlayer1Icon(eTexRoundIconInfo _iconType){
	int index = searchPlayer1NextIconSpace();
	
	if (index >= iconNum)	// 一応エラー防止
		return;

	polyPlayer1RoundIcon[index].anmNo = _iconType;
}

// player2
void RoundIconUI::setPlayer2Icon(eTexRoundIconInfo _iconType){
	int index = searchPlayer2NextIconSpace();

	if (index >= iconNum)	// 一応エラー防止
		return;

	polyPlayer2RoundIcon[index].anmNo = _iconType;
}

//----------------------------------------------------------
// 次のアイコンのスペースを探索
// player1
char RoundIconUI::searchPlayer1NextIconSpace(){
	int i;
	for (i = 0; i < iconNum; ++i){
		if (polyPlayer1RoundIcon[i].anmNo == NOTHING)
			break;
	}
	return (char)i;
}

// player2
char RoundIconUI::searchPlayer2NextIconSpace(){
	int i;
	for (i = 0; i < iconNum; ++i){
		if (polyPlayer2RoundIcon[i].anmNo == NOTHING)
			break;
	}
	return (char)i;
}

//----------------------------------------------------------
// ラウンドを進める
void RoundIconUI::setNextRound(){
	curRound++;
}

//----------------------------------------------------------
// ゲームが終了したかどうかの状態取得
bool RoundIconUI::getBattleEndState(){
	// どちらかが、勝利数分のラウンドを取っているか
	if (searchPlayer1NextIconSpace() >= iconNum ||
		searchPlayer2NextIconSpace() >= iconNum){
		return true;
	}
	return false;
}


//----------------------------------------------------------
// 描画
void RoundIconUI::draw(){
	for (int i = 0; i < iconNum; ++i){
		drawQuadPrimitive(&polyPlayer1RoundIcon[i], polyPlayer1RoundIcon[i].anmNo);
		drawQuadPrimitive(&polyPlayer2RoundIcon[i], polyPlayer2RoundIcon[i].anmNo);
	}
}

//----------------------------------------------------------
// 更新
void RoundIconUI::update(){

}

//==========================================================
// コンボ
//----------------------------------------------------------
// コンストラクタ
ComboUI::ComboUI(Actor *_player1, Actor *_player2):
FONT_WIDTH(40),
FONT_HEIGHT(80),
DRAW_POS_Y(WINDOW_HEIGHT * 0.3f),
PLAYER1_DRAW_POS_X(40),
PLAYER2_DRAW_POS_X(WINDOW_WIDTH - 40 - (FONT_WIDTH * 2))
{
	// 参照するプレイヤー
	player1 = _player1;
	player2 = _player2;

	// テクスチャ
	pTextureLoader = new TextureLoader(MAX_TEX);
	pTextureLoader->setUseTextureLabel(NUMBER_FONT, textureNS::FONT_DOT_AND_SHADOW);
}

//----------------------------------------------------------
// デストラクタ
ComboUI::~ComboUI(){
	delete pTextureLoader;
	pTextureLoader = NULL;
}

//----------------------------------------------------------
// デストラクタ
void ComboUI::load(){
	pTextureLoader->loadUseTexture();
}

//----------------------------------------------------------
// コンボの描画
void ComboUI::drawCombo(Actor *_player){
	// 1以下は表示しない
	if (_player->cntCombo < 2)
		return;

	char fmt[21];
	float drawPosX;

	// プレイヤー１、２の判断(仮… コントローラーで指定している。変える)
	if (_player->battleInput.useControllerNo == inputNS::NO1){
		drawPosX = PLAYER1_DRAW_POS_X;
		stringCopy(fmt, "%2d");
	}
	else{
		drawPosX = PLAYER2_DRAW_POS_X;
		stringCopy(fmt, "%-2d");
	}

	printfText(
		drawPosX, DRAW_POS_Y,
		FONT_WIDTH, FONT_HEIGHT,
		0xFF, 0xFF, 0xFF,
		pTextureLoader->getTexturePointer(NUMBER_FONT),
		fmt, _player->cntCombo
		);

}

//----------------------------------------------------------
// 描画
void ComboUI::draw(){
	drawCombo(player1);
	drawCombo(player2);


}

//----------------------------------------------------------
// 更新
void ComboUI::update(){


}





