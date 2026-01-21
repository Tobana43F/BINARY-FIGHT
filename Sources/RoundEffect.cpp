#include	"RoundEffect.h"		// メソッド定義
#include	"TextureLoader.h"	// テクスチャを読み込むため
#include	"MyTexture.h"		// テクスチャを使用するため

#include	"ShareBaseFunc.h"	// 実験)ラウンド数表示

//==========================================================
// BattleEndEffectクラス
//----------------------------------------------------------
// コンストラクタ
BattleEndEffect::BattleEndEffect(RoundIconUI *_pRoundIconUI) :
PLAYER_WIN_WIDTH(512),
PLAYER_WIN_HEIGHT(128),
PLAYER_WIN_X(WINDOW_WIDTH / 2.0f - PLAYER_WIN_WIDTH / 2.0f),
PLAYER_WIN_Y(WINDOW_HEIGHT * 0.325f),
DRAW_GAME_WIDTH(512),
DRAW_GAME_HEIGHT(128),
DRAW_GAME_X(WINDOW_WIDTH / 2.0f - PLAYER_WIN_WIDTH / 2.0f),
DRAW_GAME_Y(WINDOW_HEIGHT * 0.325f),
FLASH_NUM(4),
FLASH_INTERVAL(3)
{
	// テクスチャの設定
	pTextureLoader = new TextureLoader(MAX_TEX);
	pTextureLoader->setUseTextureLabel(PLAYER1_WIN_TEX, textureNS::ROUND_EFFECT_PLAYER1_WIN);
	pTextureLoader->setUseTextureLabel(PLAYER2_WIN_TEX, textureNS::ROUND_EFFECT_PLAYER2_WIN);
	pTextureLoader->setUseTextureLabel(DRAW_GAME_TEX, textureNS::ROUND_EFFECT_DRAW_GAME);


	// 参照先ポインタの設定
	pRoundIconUI = _pRoundIconUI;

	init();
}

//----------------------------------------------------------
// デストラクタ
BattleEndEffect::~BattleEndEffect(){
	delete pTextureLoader;
	pTextureLoader = NULL;
}

//----------------------------------------------------------
// 初期化
void BattleEndEffect::init(){
	isEffectEnd = false;
	scene = CHECK_END_STATE;
	effectType = INIT;
	curFlashCnt = 0;
	isDrawFlash = true;
	cnt = 0;
	cntStart = cnt;
}

//----------------------------------------------------------
// ロード
void BattleEndEffect::load(){
	pTextureLoader->loadUseTexture();
}

//----------------------------------------------------------
// 描画
void BattleEndEffect::draw(){
	switch (scene){
		case CHECK_END_STATE:
			// 何もしない
			break;

		case SET_DATA:
			break;

		case DRAW_EFFECT_START:
			if (isDrawFlash)
				polyEffect.drawPolygon();
			break;

		case DRAW_EFFECT:
			polyEffect.drawPolygon();
			break;

		case END:
			polyEffect.drawPolygon();	// 一応終了後も描画を続けるようにしておく
			break;
	}
}

//----------------------------------------------------------
// 更新
void BattleEndEffect::update(){
	switch (scene){
		case CHECK_END_STATE:
			checkEndState();
			cntStart = cnt;
			scene = SET_DATA;
			// breakしない

		case SET_DATA:
			setData();
			cntStart = cnt;
			scene = DRAW_EFFECT_START;
			// break;しない

		case DRAW_EFFECT_START:
			if ((cnt - cntStart) % FLASH_INTERVAL == 0){
				isDrawFlash = !isDrawFlash;	// 反転
				if (isDrawFlash)	// 点灯、消灯で1回とみなすため
					curFlashCnt++;
			}

			if (curFlashCnt > FLASH_NUM){
				cntStart = cnt;
				scene = DRAW_EFFECT;
			}

			break;

		case DRAW_EFFECT:
			if (cnt - cntStart >= (int)(DEF_FRAME_RATE * 2.0f)){
				scene = END;
			}
			break;

		case END:
			isEffectEnd = true;
			break;
	}

	cnt++;
}

//----------------------------------------------------------
// 終了状態を調べる
void BattleEndEffect::checkEndState(){
	// プレイヤー１が勝利(ラウンドを多く取得している)(ラウンド数が上限に達したかは、RoundEndEffectでわかる)
	if (pRoundIconUI->searchPlayer1NextIconSpace() > pRoundIconUI->searchPlayer2NextIconSpace()){
		effectType = PLAYER1_WIN;
	}
	// プレイヤー２が勝利
	else if (pRoundIconUI->searchPlayer1NextIconSpace() < pRoundIconUI->searchPlayer2NextIconSpace()){
		effectType = PLAYER2_WIN;
	}
	// 引き分け
	else{
		effectType = DRAW_GAME;
	}

}

//----------------------------------------------------------
// ポリゴンの座標などの設定
void BattleEndEffect::setData(){
	switch (effectType){
		case PLAYER1_WIN:
			polyEffect.setData(PLAYER_WIN_X, PLAYER_WIN_Y, PLAYER_WIN_WIDTH, PLAYER_WIN_HEIGHT);
			polyEffect.setTexturePointer(pTextureLoader->getTexturePointer(PLAYER1_WIN_TEX));
			break;

		case PLAYER2_WIN:
			polyEffect.setData(PLAYER_WIN_X, PLAYER_WIN_Y, PLAYER_WIN_WIDTH, PLAYER_WIN_HEIGHT);
			polyEffect.setTexturePointer(pTextureLoader->getTexturePointer(PLAYER2_WIN_TEX));
			break;

		case DRAW_GAME:
			polyEffect.setData(DRAW_GAME_X, DRAW_GAME_Y, DRAW_GAME_WIDTH, DRAW_GAME_HEIGHT);
			polyEffect.setTexturePointer(pTextureLoader->getTexturePointer(DRAW_GAME_TEX));
			break;
	}
}

//==========================================================
// RoundStartEffectクラス
//----------------------------------------------------------
// コンストラクタ
RoundStartEffect::RoundStartEffect(int *_curRound, int *_maxRound):
INIT_ROUND_WIDTH(400),
INIT_ROUND_HEIGHT(200),
INIT_ROUND_X(WINDOW_WIDTH / 2.0f - INIT_ROUND_WIDTH / 2.0f),
INIT_ROUND_Y(WINDOW_HEIGHT * 0.3f - INIT_ROUND_HEIGHT / 2.0f),

INIT_NUMBER_WIDTH(100),
INIT_NUMBER_HEIGHT(200),
INIT_NUMBER_X(WINDOW_WIDTH / 2.0f - INIT_NUMBER_WIDTH / 2.0f),
INIT_NUMBER_Y(WINDOW_HEIGHT * 0.65f - INIT_NUMBER_HEIGHT / 2.0f),

INIT_FIGHT_WIDTH(600),
INIT_FIGHT_HEIGHT(300),
INIT_FIGHT_X(WINDOW_WIDTH / 2.0f - INIT_FIGHT_WIDTH / 2.0f),
INIT_FIGHT_Y(WINDOW_HEIGHT / 2.0f - INIT_FIGHT_HEIGHT / 2.0f)
{
	// 参照先を設定
	curRound = _curRound;
	maxRound = _maxRound;

	// テクスチャ
	pTextureLoader = new TextureLoader(MAX_TEX);
	pTextureLoader->setUseTextureLabel(FONT_ROUND, textureNS::ROUND_EFFECT_ROUND);
	pTextureLoader->setUseTextureLabel(FONT_NUMBER, textureNS::ROUND_EFFECT_NUMBER);
	pTextureLoader->setUseTextureLabel(FONT_FIGHT, textureNS::ROUND_EFFECT_FIGHT);

	// 初期化
	init();
}

//----------------------------------------------------------
// デストラクタ
RoundStartEffect::~RoundStartEffect(){
	// 参照先を無くす
	curRound = NULL;
	maxRound = NULL;

	delete pTextureLoader;
	pTextureLoader = NULL;
}

//----------------------------------------------------------
// 初期化
void RoundStartEffect::init(){
	// ポリゴンの初期化
	polyFontRound.setData(INIT_ROUND_X, INIT_ROUND_Y, INIT_ROUND_WIDTH, INIT_ROUND_HEIGHT);
	polyFontRound.setTexturePointer(pTextureLoader->getTexturePointer(FONT_ROUND));

	polyFontNumber.setData(INIT_NUMBER_X, INIT_NUMBER_Y, INIT_NUMBER_WIDTH, INIT_NUMBER_HEIGHT);
	polyFontNumber.setTexturePointer(pTextureLoader->getTexturePointer(FONT_NUMBER));
	polyFontNumber.anmNo = *curRound;

	polyFontFight.setData(INIT_FIGHT_X, INIT_FIGHT_Y, INIT_FIGHT_WIDTH, INIT_FIGHT_HEIGHT);
	polyFontFight.setTexturePointer(pTextureLoader->getTexturePointer(FONT_FIGHT));

	// 制御
	scene = WAIT;
	cnt = 0;
	isRoundStarted = false;
}

//----------------------------------------------------------
// ロード
void RoundStartEffect::load(){
	pTextureLoader->loadUseTexture();
}

//----------------------------------------------------------
// 描画
void RoundStartEffect::draw(){
	switch (scene){
		case WAIT:
			// ちょっとだけ、待ち時間
			break;

		case ROUND_CALL:
			polyFontRound.drawPolygon();
			//polyFontNumber.drawPolygon();
			drawQuadPrimitive(&polyFontNumber, polyFontNumber.anmNo);
			break;

		case FIGHT_CALL:
			polyFontFight.drawPolygon();
			break;

		case END:
			break;
	}
}

//----------------------------------------------------------
// 更新
void RoundStartEffect::update(){
	switch (scene){
		case WAIT:
			if (cnt > (int)(DEF_FRAME_RATE * 0.1)){
				scene = ROUND_CALL;
				cnt = 0;
			}
			break;

		case ROUND_CALL:
			if (cnt > (int)(DEF_FRAME_RATE * 1.5f)){	// 1.5秒後
				scene = FIGHT_CALL;
				cnt = 0;	// カウンタリセット(設定しやすいように)
			}
			break;

		case FIGHT_CALL:
			if (cnt > (int)(DEF_FRAME_RATE * 1.0f)){	// 0.5秒後
				scene = END;
			}
			break;

		case END:
			isRoundStarted = true;
			break;
	}

	if (!isRoundStarted)
		cnt++;
}


//==========================================================
// RoundKOEffectクラス
//----------------------------------------------------------
// コンストラクタ
RoundKOEffect::RoundKOEffect() :
INIT_KO_WIDTH(600),
INIT_KO_HEIGHT(300),
INIT_KO_X(WINDOW_WIDTH / 2.0f - INIT_KO_WIDTH / 2.0f),
INIT_KO_Y(WINDOW_HEIGHT / 2.0f - INIT_KO_HEIGHT / 2.0f),
SLOW_UPDATE_INTERVAL(3)
{
	// テクスチャ
	pTextureLoader = new TextureLoader(MAX_TEX);
	pTextureLoader->setUseTextureLabel(FONT_KO, textureNS::ROUND_EFFECT_KO);

	init();
}

//----------------------------------------------------------
// デストラクタ
RoundKOEffect::~RoundKOEffect(){
	delete pTextureLoader;
	pTextureLoader = NULL;
}

//----------------------------------------------------------
// 初期化
void RoundKOEffect::init(){
	// 遷移の初期化
	scene = KO_STOP_AND_DRAW;
	cnt = 0;
	isNeedUpdate = false;
	isKOEffectEnd = false;

	// ポリゴンの初期化
	polyFontKO.setData(INIT_KO_X, INIT_KO_Y, INIT_KO_WIDTH, INIT_KO_HEIGHT);
	polyFontKO.setTexturePointer(pTextureLoader->getTexturePointer(FONT_KO));

}

//----------------------------------------------------------
// ロード
void RoundKOEffect::load(){
	pTextureLoader->loadUseTexture();
}

//----------------------------------------------------------
// 描画
void RoundKOEffect::draw(){
	switch (scene){
		case KO_STOP_AND_DRAW:
			polyFontKO.drawPolygon();
			break;

		case KO_SLOW:
			break;

		case END:
			break;
	}
}

//----------------------------------------------------------
// 更新
void RoundKOEffect::update(){
	switch (scene){
		case KO_STOP_AND_DRAW:
			isNeedUpdate = false;

			// 遷移処理
			if (cnt > (int)(DEF_FRAME_RATE * 1.0f)){
				scene = KO_SLOW;
				cnt = 0;
			}
			break;

		case KO_SLOW:
			if (cnt % SLOW_UPDATE_INTERVAL == 0)
				isNeedUpdate = true;
			else
				isNeedUpdate = false;

			// 遷移処理
			if (cnt > (int)(DEF_FRAME_RATE * 1.0f)){
				scene = END;
				cnt = 0;
			}

			break;

		case END:
			isNeedUpdate = true;
			isKOEffectEnd = true;
			break;
	}

	if (!isKOEffectEnd)
	cnt++;
}



//==========================================================
// RoundEndEffectクラス
//----------------------------------------------------------
// コンストラクタ
RoundEndEffect::RoundEndEffect(TimerUI *_pTimerUI, RoundIconUI *_pRoundIconUI, Actor *_pPlayer1, Actor *_pPlayer2):
DRAW_WIDTH(400),
DRAW_HEIGHT(200),
TIME_OVER_WIDTH(400),
TIME_OVER_HEIGHT(100),
PERFECT_WIDTH(400),
PERFECT_HEIGHT(200)
{
	pTimerUI = _pTimerUI;
	pRoundIconUI = _pRoundIconUI;
	pPlayer1 = _pPlayer1;
	pPlayer2 = _pPlayer2;

	// テクスチャの設定
	pTextureLoader = new TextureLoader(MAX_TEX);
	pTextureLoader->setUseTextureLabel(FONT_DRAW_TEX, textureNS::ROUND_EFFECT_DRAW);
	pTextureLoader->setUseTextureLabel(FONT_TIME_OVER_TEX, textureNS::ROUND_EFFECT_TIME_OVER);
	pTextureLoader->setUseTextureLabel(FONT_PERFECT_TEX, textureNS::ROUND_EFFECT_PERFECT);

	init();
}

//----------------------------------------------------------
// デストラクタ
RoundEndEffect::~RoundEndEffect(){
	pTimerUI = NULL;
	pRoundIconUI = NULL;
	pPlayer1 = NULL;
	pPlayer2 = NULL;

	delete pTextureLoader;
	pTextureLoader = NULL;
}

//----------------------------------------------------------
// 初期化
void RoundEndEffect::init(){
	scene = CHECK_END_STATE;
	effectType = PLAYER_KO;	// 適当な初期化

	cnt = 0;
	isRoundEndEffectEnd = false;
}

//----------------------------------------------------------
// ロード
void RoundEndEffect::load(){
	pTextureLoader->loadUseTexture();
}

//----------------------------------------------------------
// 描画
void RoundEndEffect::draw(){
	switch (scene){
		case CHECK_END_STATE:
			break;

		case SET_EFFECT:
			break;

		case WAIT_TIME:
			break;

		case DRAW_EFFECT:
			drawEffect();
			break;

		case END:
			break;
	}
}

//----------------------------------------------------------
// 更新
void RoundEndEffect::update(){
	switch (scene){
		case CHECK_END_STATE:
			checkEndState();
			scene = WAIT_TIME;
			cnt = 0;
			break;

		case SET_EFFECT:
			setEffect();
			setRoundIcon();
			scene = DRAW_EFFECT;
			cnt = 0;
			break;


		case WAIT_TIME:
			updateWaitTime();
			break;

		case DRAW_EFFECT:
			updateEffect();
			break;
		case END:
			isRoundEndEffectEnd = true;
			break;
	}

	cnt++;
}

//----------------------------------------------------------
// 終了状態の取得
void RoundEndEffect::checkEndState(){
	effectType = PLAYER_KO;

	if (pTimerUI->getIsTimeOver())
		effectType = TIME_OVER;

	if (pPlayer1->isKO && pPlayer2->isKO)
		effectType = DRAW;
}

//----------------------------------------------------------
// 状態ごとの設定
void RoundEndEffect::setEffect(){
	switch (effectType){
		case TIME_OVER:{
			// ポリゴンに値を入れる。
			polyEffect.setData(
				WINDOW_WIDTH / 2.0f - TIME_OVER_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f - TIME_OVER_HEIGHT / 2.0f,
				TIME_OVER_WIDTH, TIME_OVER_HEIGHT);
			polyEffect.setTexturePointer(pTextureLoader->getTexturePointer(FONT_TIME_OVER_TEX));
			break;
		}
		case DRAW:{
			polyEffect.setData(
				WINDOW_WIDTH / 2.0f - DRAW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f - DRAW_HEIGHT / 2.0f,
				DRAW_WIDTH, DRAW_HEIGHT);
			polyEffect.setTexturePointer(pTextureLoader->getTexturePointer(FONT_DRAW_TEX));
			break;
		}
		case PLAYER_KO:{
			// 一応パーフェクトフォントをセットしておく
			polyEffect.setData(
				WINDOW_WIDTH / 2.0f - PERFECT_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f - PERFECT_HEIGHT / 2.0f,
				PERFECT_WIDTH, PERFECT_HEIGHT);
			polyEffect.setTexturePointer(pTextureLoader->getTexturePointer(FONT_PERFECT_TEX));

			// どちらかが体力満タン状態だったら
			if (pPlayer1->curHitPoint == pPlayer1->maxHitPoint ||
				pPlayer2->curHitPoint == pPlayer2->maxHitPoint){
				effectType = PLAYER_PERFECT;
			}

			break;
		}
	}



}

//----------------------------------------------------------
// ラウンドアイコンの設定(仮)
void RoundEndEffect::setRoundIcon(){
	if (pTimerUI->getIsTimeOver() == false){	// 決着がついた場合
		// 両者の勝利判定
		if (pPlayer1->isKO == false){		// player1の勝利
			if (pPlayer1->curHitPoint == pPlayer1->maxHitPoint)
				pRoundIconUI->setPlayer1Icon(RoundIconUI::PERFECT);
			else
				pRoundIconUI->setPlayer1Icon(RoundIconUI::VICTORY);
		}
		else if (pPlayer2->isKO == false){// player2の勝利
			if (pPlayer2->curHitPoint == pPlayer2->maxHitPoint)
				pRoundIconUI->setPlayer2Icon(RoundIconUI::PERFECT);
			else
				pRoundIconUI->setPlayer2Icon(RoundIconUI::VICTORY);
		}
		else{								// 引き分け
			pRoundIconUI->setPlayer1Icon(RoundIconUI::DRAW);
			pRoundIconUI->setPlayer2Icon(RoundIconUI::DRAW);
		}
	}
	else{	// 時間切れにより終了した場合
		if (pPlayer1->curHitPoint > pPlayer2->curHitPoint){
			pRoundIconUI->setPlayer1Icon(RoundIconUI::TIME_UP);
		}
		else if (pPlayer1->curHitPoint < pPlayer2->curHitPoint){
			pRoundIconUI->setPlayer2Icon(RoundIconUI::TIME_UP);
		}
		else{
			pRoundIconUI->setPlayer1Icon(RoundIconUI::DRAW);
			pRoundIconUI->setPlayer2Icon(RoundIconUI::DRAW);
		}
	}
}

//----------------------------------------------------------
// 状態ごとの描画
void RoundEndEffect::drawEffect(){
	switch (effectType){
	case TIME_OVER:
		polyEffect.drawPolygon();
		break;

	case DRAW:
		polyEffect.drawPolygon();
		break;

	case PLAYER_KO:
		break;

	case PLAYER_PERFECT:
		polyEffect.drawPolygon();
		break;
	}
}

//----------------------------------------------------------
// 状態ごとの更新
void RoundEndEffect::updateEffect(){
	bool isEffectEnd = false;

	switch (effectType){
	case TIME_OVER:
		if (cnt > (int)(DEF_FRAME_RATE * 3.0f))
			isEffectEnd = true;
		break;

	case DRAW:
		if (cnt > (int)(DEF_FRAME_RATE * 2.5f))
			isEffectEnd = true;
		break;

	case PLAYER_KO:
		if (cnt > (int)(DEF_FRAME_RATE * 1.8f))
			isEffectEnd = true;
		break;

	case PLAYER_PERFECT:
		if (cnt > (int)(DEF_FRAME_RATE * 2.0f))
			isEffectEnd = true;
		break;
	}

	if (isEffectEnd)
		scene = END;
}

//----------------------------------------------------------
// 待ち時間の制御
void RoundEndEffect::updateWaitTime(){
	bool isWaitEnd = false;

	switch (effectType){
	case TIME_OVER:
		isWaitEnd = true;
		break;

	case DRAW:
		if (cnt > (int)(DEF_FRAME_RATE * 1.0f))
			isWaitEnd = true;
		break;

	case PLAYER_KO:
		if (cnt > (int)(DEF_FRAME_RATE * 1.0f))
			isWaitEnd = true;
		break;

	case PLAYER_PERFECT:
		if (cnt > (int)(DEF_FRAME_RATE * 1.0f))
			isWaitEnd = true;
		break;
	}

	// 画面の遷移
	if (isWaitEnd){
		scene = SET_EFFECT;
		cnt = 0;
	}
}