#include	"Battle.h"	// Battleクラスのメソッドを定義するため
#include	"Actor.h"	// Actorを引数として扱うため
#include	"ShareBaseFunc.h"	// 音声のグローバル変数、定数を使うため
#include	<math.h>

#include	"Common.h"	// フォントテクスチャを簡単に利用するため
#include	"BattleConfigInfo.h"
#include	"BattleUI.h"
#include	"GameReady.h"

#include	"Debug.h"	// デバッグの切り替えマクロを参照

//==========================================================
// 関数定義
//----------------------------------------------------------
// プレイヤーを含めた初期化
Battle::Battle(GameReady *_pGameReady){
	isSceneEnd = false;

	battleScene = INIT_ONCE;	// 初期化からはじめる
	cnt = 0;

	// バトル情報のポインタを代入
	pBattleConfigInfo = _pGameReady->pBattleConfigInfo;

	// 戦うプレイヤーの初期化
	player[0] = _pGameReady->pPlayer1;
	player[1] = _pGameReady->pPlayer2;

	for (int i = 0; i < PLAYER_NUM; ++i){
		player[i]->battleInput.setBattleCntPointer(&cnt);
	}

	// ステージの初期化
	pStage = _pGameReady->pStage;

	// キャラの影
	pCharacterShadow = _pGameReady->pCharacterShadow;

	// UIの初期化
	pTimerUI = _pGameReady->pTimerUI;
	pRoundIconUI = _pGameReady->pRoundIconUI;
	pHitPointBarUI = _pGameReady->pHitPointBarUI;
	pComboUI = _pGameReady->pComboUI;
	pBattleEffect = _pGameReady->pBattleEffect;
	pRoundStartEffect = _pGameReady->pRoundStartEffect;
	pRoundKOEffect = _pGameReady->pRoundKOEffect;
	pRoundEndEffect = _pGameReady->pRoundEndEffect;
	pBattleEndEffect = _pGameReady->pBattleEndEffect;
}

//----------------------------------------------------------
// デストラクタ
Battle::~Battle(){
	// 一応音楽をとめる
	stopSound(pStage->getUseBGMLabel());

	//----------------------------------------------------------
	// ここで開放するのはGameReadyで確保したもの
	for (int i = 0; i < PLAYER_NUM; ++i){
		delete player[i];	// プレイヤーを開放
		player[i] = NULL;
	}

	delete pStage;			// ステージ開放
	pStage = NULL;
	delete pCharacterShadow;
	pCharacterShadow = NULL;
	delete pTimerUI;		// タイマー
	pTimerUI = NULL;
	delete pHitPointBarUI;	// HPバー
	pHitPointBarUI = NULL;
	delete pRoundIconUI;	// ラウンドアイコン
	pRoundIconUI = NULL;
	delete pComboUI;		
	pComboUI = NULL;
	delete pBattleEffect;
	pBattleEffect = NULL;
	delete pRoundStartEffect;
	pRoundStartEffect = NULL;
	delete pRoundKOEffect;
	pRoundKOEffect = NULL;
	delete pRoundEndEffect;
	pRoundEndEffect = NULL;
	delete pBattleEndEffect;
	pBattleEndEffect = NULL;
}

//----------------------------------------------------------
// 初期化
void Battle::init(){
	// バトルクラスの初期化
	pTimerUI->init();
	pHitPointBarUI->init();

	// プレイヤーの初期化
	for (int i = 0; i < PLAYER_NUM; ++i){
		player[i]->init();				// キャラ共通の初期化
		player[i]->initCharacter();		// キャラ固有の初期化
	}
	
	//----------------------------------------------------------
	// バトル中に使用するほかのクラスの初期化
	pStage->init();	// ステージの初期化
	pCharacterShadow->init();
	pBattleEffect->init();
	pRoundStartEffect->init();
	pRoundKOEffect->init();
	pRoundEndEffect->init();
	pBattleEndEffect->init();

	hitStopCnt = 0;
}

//----------------------------------------------------------
// 対戦するプレイヤーを渡す
void Battle::initBattlePlayer(Actor *_player1, Actor *_player2){
	// プレイヤーをセット
	player[0] = _player1;
	player[1] = _player2;
}

//----------------------------------------------------------
// 体判定同士の、衝突を判定、(めり込み処理をも行う)
void Battle::checkCollisionBodyBox(Actor *_player1, Actor *_player2){
	BodyBoundingBoxList *curPlayer1;	// _player1用のリストを参照するポインタ変数
	BodyBoundingBoxList *curPlayer2;	// _player2用の〃

	FCoord2D actualBox1 = { 999999, 999999, 0, 0 };	// 実際に判定に使う、Box
	FCoord2D actualBox2 = { 999999, 999999, 0, 0 };	//  適当な初期化

	bool isCollide = false;					// 衝突が起こったフラグ

	//----------------------------------------------------------
	// 衝突判定を総当り
	curPlayer1 = _player1->bodyBoxList[_player1->getCurActionState()]->next;		// 先頭ポインタをセット
	for (int i = 0; curPlayer1 != NULL; ++i){
		FCoord2D absoluteCoord1;
		_player1->calcAbsoluteCoord(&absoluteCoord1, &curPlayer1->coord);

		// 先頭に戻す
		curPlayer2 = _player2->bodyBoxList[_player2->getCurActionState()]->next;	// 先頭ポインタをセット
		for (int j = 0; curPlayer2 != NULL; ++j){
			FCoord2D absoluteCoord2;
			_player2->calcAbsoluteCoord(&absoluteCoord2, &curPlayer2->coord);

			//----------------------------------------------------------
			// 体の衝突を判定
			if (!collisionDetector.getCollisionState(&absoluteCoord1, &absoluteCoord2)){ // 衝突が起こっていない場合は、処理をしない
				curPlayer2 = curPlayer2->next;	// 何もせず 次へ
				continue;
			}
			
			//----------------------------------------------------------
			// 衝突が起こっている場合
			// プレイヤー１の判定に使う実際の座標をセット
			if (actualBox1.x > absoluteCoord1.x)
				actualBox1.x = absoluteCoord1.x;
			if (actualBox1.y > absoluteCoord1.y)
				actualBox1.y = absoluteCoord1.y;

			//  幅ではなく、座標で判断する
			if (actualBox1.w < (absoluteCoord1.x + absoluteCoord1.w) - actualBox1.x)
				actualBox1.w = (absoluteCoord1.x + absoluteCoord1.w) - actualBox1.x;
			if (actualBox1.h < (absoluteCoord1.y + absoluteCoord1.h) - actualBox1.y)
				actualBox1.h = (absoluteCoord1.y + absoluteCoord1.h) - actualBox1.y;

			// プレイヤー２の判定に使う実際の座標をセット
			if (actualBox2.x > absoluteCoord2.x)
				actualBox2.x = absoluteCoord2.x;
			if (actualBox2.y > absoluteCoord2.y)
				actualBox2.y = absoluteCoord2.y;

			//  幅ではなく、座標で判断する
			if (actualBox2.w < (absoluteCoord2.x + absoluteCoord2.w) - actualBox2.x)
				actualBox2.w = (absoluteCoord2.x + absoluteCoord2.w) - actualBox2.x;
			if (actualBox2.h < (absoluteCoord2.y + absoluteCoord2.h) - actualBox2.y)
				actualBox2.h = (absoluteCoord2.y + absoluteCoord2.h) - actualBox2.y;
					
			isCollide = true;	// 衝突フラグの設定

			curPlayer2 = curPlayer2->next;	// 次へ
		}
		curPlayer1 = curPlayer1->next;	// 次へ
	}

	//----------------------------------------------------------
	// 衝突が起こっていない場合は、何もしない
	if (isCollide == false)	// 衝突が起こってないので、処理をしない
		return;

	//----------------------------------------------------------
	// 衝突が起こっているボックスを求める
	FCoord2D collideBox;
	
	// x.w
	if (actualBox1.x > actualBox2.x){
		collideBox.x = actualBox1.x;
		collideBox.w = (actualBox2.x + actualBox2.w) - collideBox.x;
	}
	else{
		collideBox.x = actualBox2.x;
		collideBox.w = (actualBox1.x + actualBox1.w) - collideBox.x;
	}
	// y.h
	if (actualBox1.y > actualBox2.y){
		collideBox.y = actualBox1.y;
		collideBox.h = (actualBox2.y + actualBox2.h) - collideBox.y;
	}
	else{
		collideBox.y = actualBox2.y;
		collideBox.h = (actualBox1.y + actualBox1.h) - collideBox.y;
	}

	//----------------------------------------------------------
	// めり込み処理
	_player1->addCoordWithPosState(-(collideBox.w / 2),0.0f);	// 衝突が起こっているボックスの半分を引く
	_player2->addCoordWithPosState(-(collideBox.w / 2),0.0f);

}

//----------------------------------------------------------
// 喰らい判定、攻撃判定の当り判定
void Battle::checkCollisionAttackBox(Actor *defense, Actor *offense){
	HurtBoundingBoxList		*curDefense;	// defense側のリストを参照するポインタ変数
	AttackBoundingBoxList	*curOffense;	// offense側の〃


	//==========================================================
	// 例外)相手が空中追撃不可能時は、判定はしなくて良い
	if (defense->getCurActionState() == Actor::AIR_KNOCK_DOWN && !defense->isJuggle)
		return;

	//----------------------------------------------------------
	// 総当りで判定
	curOffense = offense->attackBoxList[offense->getCurActionState()]->next;	// 先頭ポインタをセット
	for (int i = 0; curOffense != NULL; ++i){
		// 当り判定の相対座標にプレイヤーの座標を足し、絶対座標を求める
		FCoord2D absoluteCoord1;
		offense->calcAbsoluteCoord(&absoluteCoord1, &curOffense->coord);

		// 先頭に戻す
		curDefense = defense->hurtBoxList[defense->getCurActionState()]->next;	// 先頭ポインタをセット
		for (int j = 0; curDefense != NULL; ++j){
			// 当り判定の相対座標にプレイヤーの座標を足し、絶対座標を求める
			FCoord2D absoluteCoord2;
			defense->calcAbsoluteCoord(&absoluteCoord2, &curDefense->coord);

			//----------------------------------------------------------
			// 衝突状態を判定
			if (!collisionDetector.getCollisionState(&absoluteCoord1, &absoluteCoord2))		// 衝突が起こったとき
			{
				curDefense = curDefense->next;	// 次へ
				continue;
			}

			//----------------------------------------------------------
			// 衝突が起こっている場合
			// offense側の判定処理
			if (curOffense->isHit == false){			// その部分の衝突判定が初めて起こったとき
				curOffense->setIsHit(true);

				// (複数の判定を持つ)1つの攻撃が当たった時
				if (offense->isAttackBoxEnable){		// 一連の攻撃の判定が有効なとき
					offense->isAttackBoxEnable = false;	// 攻撃があたれば、これ以降の一連の攻撃は判定を受け付けない(連続ヒットさせる場合は、攻撃判定を出す前に、初期化する)
					offense->isAttackHit = true;		// 攻撃があたった

					//----------------------------------------------------------
					// 相手に攻撃情報を渡す
					defense->receivedAttackInfo[defense->receivedAttackNum] = curOffense->attackInfo;
					// 攻撃情報に衝突した座標を代入…CollisionBoundingBoxのメソッドとして定義する？
					// x.w
					if (absoluteCoord1.x > absoluteCoord2.x){
						defense->receivedAttackInfo[defense->receivedAttackNum].collideBox.x = absoluteCoord1.x;
						defense->receivedAttackInfo[defense->receivedAttackNum].collideBox.w = (absoluteCoord2.x + absoluteCoord2.w) - defense->receivedAttackInfo[defense->receivedAttackNum].collideBox.x;
					} else{
						defense->receivedAttackInfo[defense->receivedAttackNum].collideBox.x = absoluteCoord2.x;
						defense->receivedAttackInfo[defense->receivedAttackNum].collideBox.w = (absoluteCoord1.x + absoluteCoord1.w) - defense->receivedAttackInfo[defense->receivedAttackNum].collideBox.x;
					}
					// y.h
					if (absoluteCoord1.y > absoluteCoord2.y){
						defense->receivedAttackInfo[defense->receivedAttackNum].collideBox.y = absoluteCoord1.y;
						defense->receivedAttackInfo[defense->receivedAttackNum].collideBox.h = (absoluteCoord2.y + absoluteCoord2.h) - defense->receivedAttackInfo[defense->receivedAttackNum].collideBox.y;
					} else{
						defense->receivedAttackInfo[defense->receivedAttackNum].collideBox.y = absoluteCoord2.y;
						defense->receivedAttackInfo[defense->receivedAttackNum].collideBox.h = (absoluteCoord1.y + absoluteCoord1.h) - defense->receivedAttackInfo[defense->receivedAttackNum].collideBox.y;
					}

					// めくり攻撃判定が起こる場合(相手の向いている方向でノックバック方向が決まる？)
					if (offense->getCurDirState() == offense->getCurPosState()){	// 位置関係と向きが同じ側の場合(右、右　:　左、左)
						defense->isCrossUpReceived = true;
						// 例外的処理) 相手を無理やり攻撃側の反対側を向かせる
						if (offense->getCurDirState() == Actor::RIGHT)
							defense->setDirState(Actor::LEFT);
						else
							defense->setDirState(Actor::RIGHT);
						defense->flgChangePosState = true;
					}

					if (defense->isCrossUpReceived)
						defense->isCrossUpHitStun = true;
					else
						defense->isCrossUpHitStun = false;

					//----------------------------------------------------------
					// 攻撃情報のカーソルを次へ
					defense->receivedAttackNum++;
					if (defense->receivedAttackNum > defense->RECEIVE_ATTACK_MAX)
						defense->receivedAttackNum = defense->RECEIVE_ATTACK_MAX;		// 上限値で固定(エラー防止)

					
					//----------------------------------------------------------
					// バトルクラスに直接渡す値
					// BattleクラスにHitStopの情報のみ渡す
					if (hitStopCnt < curOffense->attackInfo.attackHitStopCnt)
						hitStopCnt = curOffense->attackInfo.attackHitStopCnt;			// ヒットストップを代入(情報を渡すだけ、実際に反映される用のヒットストップなどには反映されていない)

					//----------------------------------------------------------
					// 攻撃側の処理
					// 仮)コンボ判定(相手がガードしたかが反映されていない→ガード後にコンボを減算)
					if (offense->cntCombo == 0){
						offense->cntCombo++;
					}
					else if (defense->attackHitStunCnt >= 0){
						offense->cntCombo++;
					}
				}
			}

			// defense側の判定処理
			if (curDefense->isHit == false){		// 初めて衝突が起こるとき
				curDefense->setIsHit(true);
			}

			curDefense = curDefense->next;	// 次へ
		}
		curOffense = curOffense->next;		// 次へ
	}
}

//----------------------------------------------------------
// 喰らい判定、防御判定の当たり判定
void Battle::checkCollisionGuardBox(Actor *defense, Actor *offense){
	HurtBoundingBoxList		*curDefense;	// defense側のリストを参照するポインタ変数
	GuardBoundingBoxList	*curOffense;	// offense側の〃

	//==========================================================
	// 初期化
	defense->isCollideGuardBox = false;	// 初期化

	//----------------------------------------------------------
	// 総当りで判定
	curOffense = offense->guardBoxList[offense->getCurActionState()]->next;	// 先頭ポインタをセット
	for (int i = 0; curOffense != NULL; ++i){
		// 当り判定の相対座標にプレイヤーの座標を足し、絶対座標を求める
		FCoord2D absoluteCoord1;
		offense->calcAbsoluteCoord(&absoluteCoord1, &curOffense->coord);

		// 先頭に戻す
		curDefense = defense->hurtBoxList[defense->getCurActionState()]->next;	// 先頭ポインタをセット
		for (int j = 0; curDefense != NULL; ++j){
			// 当り判定の相対座標にプレイヤーの座標を足し、絶対座標を求める
			FCoord2D absoluteCoord2;
			defense->calcAbsoluteCoord(&absoluteCoord2, &curDefense->coord);

			//----------------------------------------------------------
			// 衝突状態を判定
			if (!collisionDetector.getCollisionState(&absoluteCoord1, &absoluteCoord2))		// 衝突が起こったとき
			{
				curDefense = curDefense->next;	// 次へ
				continue;
			}

			//----------------------------------------------------------
			// 衝突が起こっている場合
			// 相手のガードくらい判定を変更
			defense->isCollideGuardBox = true;
			return;	// 衝突が見つかり次第、処理を中断

			curDefense = curDefense->next;	// 次へ
		}
		curOffense = curOffense->next;		// 次へ
	}
}


//----------------------------------------------------------
// ステージの壁、との判定
void Battle::checkCollisionScreenLimit(Actor *_player){
	FCoord2D absoluteCoord;
	_player->calcAbsoluteCoord(&absoluteCoord, &_player->wallBox[_player->curTexNo].coord);

	if (absoluteCoord.x <= 0.0f){
		// 座標を戻す
		_player->coord.x = 0.0f - (absoluteCoord.x - _player->coord.x);
		_player->isStageLimitX = true;
	}
	else if (absoluteCoord.x >= WINDOW_WIDTH - absoluteCoord.w - 1){	// -1は誤差
		// 座標を戻す
		_player->coord.x = WINDOW_WIDTH - ((absoluteCoord.x + absoluteCoord.w) - _player->coord.x);
		_player->isStageLimitX = true;
	} else
		_player->isStageLimitX = false;
}

//----------------------------------------------------------
// プレイヤーの位置関係を判定
void Battle::checkPlayerPosition(){
	// キャラクターの中心座標で、判断する
	// player1 , player2
	if (player[0]->coord.x + player[0]->coord.w / 2.0f < player[1]->coord.x + player[1]->coord.w/ 2.0f){
		player[0]->setPosState(Actor::LEFT);
		player[1]->setPosState(Actor::RIGHT);
	}
	// player2, player1 
	else if (player[0]->coord.x + player[0]->coord.w / 2.0f > player[1]->coord.x + player[1]->coord.w / 2.0f){
		player[0]->setPosState(Actor::RIGHT);
		player[1]->setPosState(Actor::LEFT);
	}

}

//----------------------------------------------------------
// 描画関数
void Battle::draw(){
	switch (battleScene){
		case INIT_ONCE:
			drawGameScreen();
			break;

		case INIT:
			drawGameScreen();
			break;
		case ROUND_START:
			//----------------------------------------------------------
			// ゲーム画面は表示しておく
			drawGameScreen();
			// ラウンドコールの表示
			pRoundStartEffect->draw();
			break;

		case BATTLE_PLAY:
			drawGameScreen();
			break;

		case PLAYER_KO:
			drawGameScreen();

			//----------------------------------------------------------
			// KO文字の描画
			pRoundKOEffect->draw();

			break;

		case ROUND_END:
			drawGameScreen();

			pRoundEndEffect->draw();
			break;

		case BATTLE_END:
			drawGameScreen();
			pBattleEndEffect->draw();
			break;

		case END:
			break;

	}
}

void Battle::drawGameScreen(){
	pStage->draw();

	pTimerUI->draw();
	pHitPointBarUI->draw();
	pRoundIconUI->draw();
	pComboUI->draw();

	pCharacterShadow->draw();
	for (int i = 0; i < PLAYER_NUM; ++i)
		player[i]->drawPolygon();	// キャラクター
	
#ifdef DEBUG_SHOW_HITBOX
	drawHitBox();					// キャラクターの当たり判定
#endif

	pBattleEffect->draw();

#ifdef DEBUG_SHOW_PLAYER_INFO
	drawDebugInfo();
#endif

}

//----------------------------------------------------------
// 当たり判定の描画
void Battle::drawHitBox(){
	for (int i = 0; i < PLAYER_NUM; ++i){
		player[i]->drawWallBox();	// 壁判定
		player[i]->drawBodyBox();	// 体判定

		if (!(player[i]->getCurActionState() == Actor::AIR_KNOCK_DOWN && !player[i]->isJuggle))	// 空中ダメージ時に追撃不可能のときは、分かりやすくするため、描画しない
			player[i]->drawHurtBox();	// 喰らい判定描画

		player[i]->drawGuardBox();	// ガード判定
		player[i]->drawAttackBox();	// 攻撃判定の描画

	}
}

//----------------------------------------------------------
// 更新関数
void Battle::update(){
	//==========================================================
	// 入力状態の更新(常に更新し続ける)
	for (int i = 0; i < PLAYER_NUM; ++i)
		player[i]->battleInput.update();

	switch (battleScene){
		case INIT_ONCE:{ // 最初の一度のみ
			playSound(pStage->getUseBGMLabel());	// BGM開始
			battleScene = INIT;
		}
		case INIT:{
			init();		// バトルの初期化
			battleScene = ROUND_START;	
		}
		case ROUND_START:{
			// 画面の更新のみ行う
			updateGameScreen();

			pCharacterShadow->update();	// キャラの影
			pBattleEffect->update();	// エフェクト

			//----------------------------------------------------------
			// ラウンド開始コール
			pRoundStartEffect->update();

			// 遷移
			if (pRoundStartEffect->getIsRoundStarted())
				battleScene = BATTLE_PLAY;
			break;
		}
		case BATTLE_PLAY:{
			//----------------------------------------------------------
			// 戦闘処理の更新
			updateBattlePlay();

			// キャラの影
			pCharacterShadow->update();

			//----------------------------------------------------------
			// UIの更新
			if (!player[0]->isKO && !player[1]->isKO)
				pTimerUI->update();
			pHitPointBarUI->update();
			pRoundIconUI->update();
			pComboUI->update();

			pBattleEffect->update();

			//----------------------------------------------------------
			// KO判断とラウンド終了フラグのセット
			if (player[0]->isKO == true || player[1]->isKO == true){	// プレイヤーのKOでの終了
				battleScene = PLAYER_KO;
			}
			else if (pTimerUI->getIsTimeOver() == true){			// タイムオーバー時
				battleScene = ROUND_END;
			}

			break;
		}
		case PLAYER_KO:{
			bool isKOActionEnd = false;
			bool isPlayerLand = false;

			//----------------------------------------------------------
			// KO時の制御
			pRoundKOEffect->update();
			if (!pRoundKOEffect->getIsNeedUpdate())
				break;

			//----------------------------------------------------------
			// 戦闘処理の更新
			updateBattleAfterKO();

			// キャラの影
			pCharacterShadow->update();

			//----------------------------------------------------------
			// UIの更新
			pHitPointBarUI->update();
			pRoundIconUI->update();
			pComboUI->update();

			pBattleEffect->update();

			//----------------------------------------------------------
			// KOしたキャラが地面に着地(倒れこむ)した状態になったら
			if (player[0]->isKO == true){		// player1のみKO
				if (player[0]->getCurActionState() == +Actor::KO_END)
					isKOActionEnd = true;
			}
			else if (player[1]->isKO == true){	// player2のみKO
				if (player[1]->getCurActionState() == Actor::KO_END)
					isKOActionEnd = true;
			}
			else{								// 両者KO
				if (player[0]->getCurActionState() == Actor::KO_END &&
					player[1]->getCurActionState() == Actor::KO_END)
					isKOActionEnd = true;
			}

			//----------------------------------------------------------
			// 両者が着地状態かどうか
			if (player[0]->isLand && player[1]->isLand)
				isPlayerLand = true;

			//----------------------------------------------------------
			// 遷移判断
			if (isKOActionEnd == true && isPlayerLand){
				battleScene = ROUND_END;

				// 仮)勝利ポーズをとる
				for (int i = 0; i < PLAYER_NUM; ++i){
					if (player[i]->isKO)
						continue;
					// 勝者は、勝利ポーズ
					player[i]->changeAnmNo(Actor::POSE_ROUND_WIN_1);
				}
			}
			break;
		}
		case ROUND_END:{
			// 終了状態を判断したり、それに応じた演出
			// アイコンの更新処理など
			updateRoundEnd();			// 画面
			pCharacterShadow->update();	// キャラの影
			pBattleEffect->update();	// エフェクト
			pRoundEndEffect->update();

			//----------------------------------------------------------
			// 終了状態の判断だけ行う
			if (pRoundEndEffect->getIsRoundEndEffectEnd())
			{
				// 次のラウンドへ
				pRoundIconUI->setNextRound();

				//  ラウンドが残っていたら、勝負続行
				//  ラウンドを全て取っていたら、バトルエンドへ
				if (pRoundIconUI->getBattleEndState() == false){
					battleScene = INIT;
				}
				else{
					battleScene = BATTLE_END;
					// 仮)勝利ポーズをとる
					for (int i = 0; i < PLAYER_NUM; ++i){
						if (player[i]->isKO)
							continue;
						// 勝者は、勝利ポーズ
						player[i]->changeAnmNo(Actor::POSE_BATTLE_WIN_1);
					}
				}
			}

			break;
		}
		case BATTLE_END:
			updateRoundEnd();			// 画面
			pCharacterShadow->update();	// キャラの影
			pBattleEffect->update();	// エフェクト
			pBattleEndEffect->update();
			if (pBattleEndEffect->getIsEffectEnd()){
				battleScene = END;
				stopSound(pStage->getUseBGMLabel());	// BGM停止
			}			
			break;

		case END:
			//----------------------------------------------------------
			// 画面遷移の処理
			if (updateGameScene()){ return; }

			break;

	}

	cnt++;		// バトルクラス内のカウンタを更新。
}

bool Battle::updateGameScene(){
	isSceneEnd = true;

	return true;	// 旧verの名残
}

void Battle::updateBattlePlay(){
	//----------------------------------------------------------
	// ヒットストップがある場合は更新処理をしない
	if (hitStopCnt != 0){
		// ヒットストップの減算
		hitStopCnt--;
		if (hitStopCnt < 0)
			hitStopCnt = 0;

		return;
	}

	//----------------------------------------------------------
	// ステージ
	pStage->update();

	//----------------------------------------------------------
	// 更新処理
	// 行動より前に初期化しないといけない処理
	for (int i = 0; i < PLAYER_NUM; ++i){
		// トリガーリセット(仮)
		player[i]->flgTriggerChangeAction = false;
		player[i]->flgTriggerChangeAnm = false;
	}

	//----------------------------------------------------------
	// 行動
	// プレイヤーの状態、アニメーションの更新
	for (int i = 0; i < PLAYER_NUM; ++i){
		player[i]->updatePolygon();	// アニメーションの更新処理

		// ヒット硬直を減算(仮)
		player[i]->attackHitStunCnt--;
		if (player[i]->attackHitStunCnt < 0){
			player[i]->attackHitStunCnt = 0;
		}

		// 行動
		player[i]->operateActor();

		player[i]->moveCoord();		// 座標にスピードを反映

		// 地上、空中の状態を取得
		if (player[i]->coord.y < pStage->groundPosY - player[i]->coord.h){	// 地面の座標より上にいるとき
			player[i]->isLand = false;
			player[i]->curSpeed.y += 1.0f;
		} else{															// 地上にいるとき
			player[i]->isLand = true;
			float tmpY = pStage->groundPosY - player[i]->coord.h;
			player[i]->setCoord(player[i]->coord.x, tmpY);

			player[i]->isJuggle = false;	// 確実に初期化できるので、ここで行う
		}

		// ガード状態を取得
		player[i]->checkGuardState();
	}

	//----------------------------------------------------------
	// 行動後の初期化
	// プレイヤーのバトル時の情報を初期化
	for (int i = 0; i < PLAYER_NUM; ++i){
		// 当り判定の初期化(行動をした後にしなければならない)
		player[i]->initHurtBoxData();
		player[i]->initAttackBoxData();

		// 攻撃行動フラグの取得
		player[i]->changeIsAttackAction();
		
		// 攻撃された情報を初期化
		player[i]->receivedAttackNum = 0;
		for (int j = 0; j < player[i]->RECEIVE_ATTACK_MAX; ++j)
			player[i]->receivedAttackInfo[j].initData();

		// 食らったダメージを初期化
		player[i]->hitDamage = 0;

		// ガードフラグを初期化
		player[i]->isBlock = false;
		player[i]->isCrossUpReceived = false;


		// 攻撃判定よりも前にやることで、攻撃を受けた瞬間はその場にとどまる(見た目的な問題　+　めくり処理の不具合軽減)
		// ヒットバックを減算(仮)
		if (fabs(player[i]->hitBack) > 0){
			if (pStage->isStageLimitX == false)
				player[i]->addCoordWithDirState(player[i]->hitBack, 0);
			else{
				// プレイヤーが画面にめり込んだ場合
				if (player[i]->isStageLimitX == true)
					player[i^PLAYER_CHANGER]->addCoordWithDirState(player[i]->hitBack, 0);	// 相手を押し出す。
				else
					player[i]->addCoordWithDirState(player[i]->hitBack, 0);
			}

			if (player[i]->hitBack < 0)
				player[i]->hitBack++;
			else
				player[i]->hitBack--;
		}
	}

	//----------------------------------------------------------
	// 判定処理
	// プレイヤー同士の衝突判定
	checkCollisionBodyBox(player[0], player[1]);

	for (int i = 0; i < PLAYER_NUM; ++i){
		// 攻撃判定のチェック
		checkCollisionAttackBox(player[i], player[i^PLAYER_CHANGER]);
		checkCollisionGuardBox(player[i], player[i^PLAYER_CHANGER]);
	}

	//----------------------------------------------------------
	// 判定応答処理など
	for (int i = 0; i < PLAYER_NUM; ++i){
		// 判定応答
		player[i]->reactReceiveGuardBox();
		player[i]->reactReceiveAttack();


		// エフェクトの追加
		for (int j = 0; j < player[i]->receivedAttackNum; ++j){
			pBattleEffect->addEffectList(player[i], &player[i]->receivedAttackInfo[j]);
		}

		// エフェクトの追加
		if (player[i]->isBlock == true){
			// 相手の攻撃ガードされたフラグを変更
			player[i^PLAYER_CHANGER]->isAttackBlocked = true;
		} 
		else{
			// 相手の攻撃ガードされたフラグを変更
			player[i^PLAYER_CHANGER]->isAttackBlocked = false;
		}

		// 実際に体力を減らす(仮)
		player[i]->curHitPoint -= player[i]->hitDamage;
		if (player[i]->curHitPoint < 0){	// 死亡したら
			player[i]->curHitPoint = 0;
			player[i]->isKO = true;			// 死亡

			// KO_START状態
			if (player[i]->getCurActionState() != Actor::KO_START){	// KO_STARTモーションに入っていない場合
				player[i]->changeAnmNo(Actor::KO_START);
				player[i]->isJuggle = true;	// 空中追撃可能状態にしておく
			}
		}
	}

	//----------------------------------------------------------
	// 判定応答後処理
	for (int i = 0; i < PLAYER_NUM; ++i){
		// コンボの終了判定
		if (player[i^PLAYER_CHANGER]->attackHitStunCnt == 0 && !player[PLAYER_CHANGER]->isJuggle){	// 相手のヒット硬直がなく、空中追撃もない場合、コンボが切れる
			player[i]->cntCombo = 0;
		}

		// 相手がガードしていた場合はコンボを減算し、なかったことにする。
		for (int j = 0; j < player[i^PLAYER_CHANGER]->receivedAttackNum; ++j){
			if (player[i^PLAYER_CHANGER]->isBlock == true){
				player[i]->cntCombo--;
			}
		}

		checkCollisionScreenLimit(player[i]);
	}

	// 位置関係をチェック
	checkPlayerPosition();

	for (int i = 0; i < PLAYER_NUM; ++i){
		player[i]->checkPosState();				// 位置関係の確認(トリガーの変更)
		player[i]->changePolyReverseXState();	// 向きを変える,(実際に描画を反転する)

		player[i]->operatePartLandCancelWidthAttackReverse();	// isLandCancelableフラグをリセットする前に行う必要がある
		player[i]->checkLandingState();			// 着地モーションのキャンセル
		player[i]->operatePartReverseCancel();	// 振り向き行動のキャンセル
	}
}

void Battle::updateBattleAfterKO(){
	//----------------------------------------------------------
	// ヒットストップがある場合は更新処理をしない
	if (hitStopCnt != 0){
		// ヒットストップの減算
		hitStopCnt--;
		if (hitStopCnt < 0)
			hitStopCnt = 0;

		return;
	}

	//----------------------------------------------------------
	// ステージ
	pStage->update();

	//----------------------------------------------------------
	// 更新処理
	// 行動より前に初期化しないといけない処理
	for (int i = 0; i < PLAYER_NUM; ++i){
		// トリガーリセット(仮)
		player[i]->flgTriggerChangeAction = false;
		player[i]->flgTriggerChangeAnm = false;
	}

	//----------------------------------------------------------
	// 行動
	// プレイヤーの状態、アニメーションの更新
	for (int i = 0; i < PLAYER_NUM; ++i){
		player[i]->updatePolygon();	// アニメーションの更新処理

		// ヒット硬直を減算(仮)
		player[i]->attackHitStunCnt--;
		if (player[i]->attackHitStunCnt < 0){
			player[i]->attackHitStunCnt = 0;
		}

		// 行動
		player[i]->operateActor();

		player[i]->moveCoord();		// 座標にスピードを反映

		// 地上、空中の状態を取得
		if (player[i]->coord.y < pStage->groundPosY - player[i]->coord.h){	// 地面の座標より上にいるとき
			player[i]->isLand = false;
			player[i]->curSpeed.y += 1.0f;
		}
		else{															// 地上にいるとき
			player[i]->isLand = true;
			float tmpY = pStage->groundPosY - player[i]->coord.h;
			player[i]->setCoord(player[i]->coord.x, tmpY);

			player[i]->isJuggle = false;	// 確実に初期化できるので、ここで行う
		}

		// ガード状態を取得
		player[i]->checkGuardState();
	}

	//----------------------------------------------------------
	// 行動後の初期化
	// プレイヤーのバトル時の情報を初期化
	for (int i = 0; i < PLAYER_NUM; ++i){
		// 当り判定の初期化(行動をした後にしなければならない)
		player[i]->initHurtBoxData();
		player[i]->initAttackBoxData();

		// 攻撃行動フラグの取得
		player[i]->changeIsAttackAction();

		// 攻撃された情報を初期化
		player[i]->receivedAttackNum = 0;
		for (int j = 0; j < player[i]->RECEIVE_ATTACK_MAX; ++j)
			player[i]->receivedAttackInfo[j].initData();

		// 食らったダメージを初期化
		player[i]->hitDamage = 0;

		// ガードフラグを初期化
		player[i]->isBlock = false;
		player[i]->isCrossUpReceived = false;


		// 攻撃判定よりも前にやることで、攻撃を受けた瞬間はその場にとどまる(見た目的な問題　+　めくり処理の不具合軽減)
		// ヒットバックを減算(仮)
		if (fabs(player[i]->hitBack) > 0){
			if (pStage->isStageLimitX == false)
				player[i]->addCoordWithDirState(player[i]->hitBack, 0);
			else{
				// プレイヤーが画面にめり込んだ場合
				if (player[i]->isStageLimitX == true)
					player[i^PLAYER_CHANGER]->addCoordWithDirState(player[i]->hitBack, 0);	// 相手を押し出す。
				else
					player[i]->addCoordWithDirState(player[i]->hitBack, 0);
			}

			if (player[i]->hitBack < 0)
				player[i]->hitBack++;
			else
				player[i]->hitBack--;
		}
	}

	//----------------------------------------------------------
	// 判定処理
	// プレイヤー同士の衝突判定
	checkCollisionBodyBox(player[0], player[1]);

	for (int i = 0; i < PLAYER_NUM; ++i){
		// 攻撃判定のチェック
		checkCollisionAttackBox(player[i], player[i^PLAYER_CHANGER]);
	}

	//----------------------------------------------------------
	// 判定応答処理など
	for (int i = 0; i < PLAYER_NUM; ++i){
		// 判定応答
		player[i]->reactReceiveAttack();

		// エフェクトの追加
		for (int j = 0; j < player[i]->receivedAttackNum; ++j){
			pBattleEffect->addEffectList(player[i], &player[i]->receivedAttackInfo[j]);
		}

		// エフェクトの追加
		if (player[i]->isBlock == true){
			// 相手の攻撃ガードされたフラグを変更
			player[i^PLAYER_CHANGER]->isAttackBlocked = true;
		} 
		else{
			// 相手の攻撃ガードされたフラグを変更
			player[i^PLAYER_CHANGER]->isAttackBlocked = false;
		}

		// 実際に体力を減らす(仮)
		if (player[i]->isKO){
			player[i]->curHitPoint -= player[i]->hitDamage;
			if (player[i]->curHitPoint < 0){	// 死亡したら
				player[i]->curHitPoint = 0;
				player[i]->isKO = true;			// 死亡

				// KO_START状態
				if (player[i]->getCurActionState() != Actor::KO_START){	// KO_STARTモーションに入っていない場合
					player[i]->changeAnmNo(Actor::KO_START);
					player[i]->isJuggle = true;	// 空中追撃可能状態にしておく
				}
			}
		}
	}

	//----------------------------------------------------------
	// 判定応答後処理
	for (int i = 0; i < PLAYER_NUM; ++i){
		// コンボの終了判定
		if (player[i^PLAYER_CHANGER]->attackHitStunCnt == 0 && !player[PLAYER_CHANGER]->isJuggle){	// 相手のヒット硬直がなくなった場合、コンボが切れる
			player[i]->cntCombo = 0;
		}

		// 相手がガードしていた場合はコンボを減算し、なかったことにする。
		for (int j = 0; j < player[i^PLAYER_CHANGER]->receivedAttackNum; ++j){
			if (player[i^PLAYER_CHANGER]->isBlock == true){
				player[i]->cntCombo--;
			}
		}

		checkCollisionScreenLimit(player[i]);
	}

	// 位置関係をチェック
	checkPlayerPosition();

	for (int i = 0; i < PLAYER_NUM; ++i){
		player[i]->checkPosState();				// 位置関係の確認(トリガーの変更)
		player[i]->changePolyReverseXState();	// 向きを変える,(実際に描画を反転する)

		player[i]->operatePartLandCancelWidthAttackReverse();	// isLandCancelableフラグをリセットする前に行う必要がある
		player[i]->checkLandingState();			// 着地モーションのキャンセル
		player[i]->operatePartReverseCancel();	// 振り向き行動のキャンセル
	}
}

// 仮)
void Battle::updateRoundEnd(){
	//----------------------------------------------------------
	// ヒットストップがある場合は更新処理をしない
	if (hitStopCnt != 0){
		// ヒットストップの減算
		hitStopCnt--;
		if (hitStopCnt < 0)
			hitStopCnt = 0;

		return;
	}

	//----------------------------------------------------------
	// ステージ
	pStage->update();

	//----------------------------------------------------------
	// 更新処理
	// 行動より前に初期化しないといけない処理
	for (int i = 0; i < PLAYER_NUM; ++i){
		// トリガーリセット(仮)
		player[i]->flgTriggerChangeAction = false;
		player[i]->flgTriggerChangeAnm = false;
	}

	//----------------------------------------------------------
	// 行動
	// プレイヤーの状態、アニメーションの更新
	for (int i = 0; i < PLAYER_NUM; ++i){
		player[i]->updatePolygon();	// アニメーションの更新処理

		// ヒット硬直を減算(仮)
		player[i]->attackHitStunCnt--;
		if (player[i]->attackHitStunCnt < 0){
			player[i]->attackHitStunCnt = 0;
		}

		// 行動
		player[i]->operateActor();

		player[i]->moveCoord();		// 座標にスピードを反映

		// 地上、空中の状態を取得
		if (player[i]->coord.y < pStage->groundPosY - player[i]->coord.h){	// 地面の座標より上にいるとき
			player[i]->isLand = false;
			player[i]->curSpeed.y += 1.0f;
		}
		else{															// 地上にいるとき
			player[i]->isLand = true;
			float tmpY = pStage->groundPosY - player[i]->coord.h;
			player[i]->setCoord(player[i]->coord.x, tmpY);

			player[i]->isJuggle = false;	// 確実に初期化できるので、ここで行う
		}

		// ガード状態を取得
		player[i]->checkGuardState();
	}

	//----------------------------------------------------------
	// 行動後の初期化
	// プレイヤーのバトル時の情報を初期化
	for (int i = 0; i < PLAYER_NUM; ++i){
		// 当り判定の初期化(行動をした後にしなければならない)
		player[i]->initHurtBoxData();
		player[i]->initAttackBoxData();

		// 攻撃行動フラグの取得
		player[i]->changeIsAttackAction();

		// 攻撃された情報を初期化
		player[i]->receivedAttackNum = 0;
		for (int j = 0; j < player[i]->RECEIVE_ATTACK_MAX; ++j)
			player[i]->receivedAttackInfo[j].initData();

		// 食らったダメージを初期化
		player[i]->hitDamage = 0;

		// ガードフラグを初期化
		player[i]->isBlock = false;
		player[i]->isCrossUpReceived = false;


		// 攻撃判定よりも前にやることで、攻撃を受けた瞬間はその場にとどまる(見た目的な問題　+　めくり処理の不具合軽減)
		// ヒットバックを減算(仮)
		if (fabs(player[i]->hitBack) > 0){
			if (pStage->isStageLimitX == false)
				player[i]->addCoordWithDirState(player[i]->hitBack, 0);
			else{
				// プレイヤーが画面にめり込んだ場合
				if (player[i]->isStageLimitX == true)
					player[i^PLAYER_CHANGER]->addCoordWithDirState(player[i]->hitBack, 0);	// 相手を押し出す。
				else
					player[i]->addCoordWithDirState(player[i]->hitBack, 0);
			}

			if (player[i]->hitBack < 0)
				player[i]->hitBack++;
			else
				player[i]->hitBack--;
		}
	}

	//----------------------------------------------------------
	// 判定処理
	// プレイヤー同士の衝突判定
	checkCollisionBodyBox(player[0], player[1]);

	for (int i = 0; i < PLAYER_NUM; ++i){
		// 攻撃判定のチェック
		checkCollisionAttackBox(player[i], player[i^PLAYER_CHANGER]);
	}

	//----------------------------------------------------------
	// 判定応答処理など
	for (int i = 0; i < PLAYER_NUM; ++i){
		// 判定応答
		player[i]->reactReceiveAttack();

		// エフェクトの追加
		for (int j = 0; j < player[i]->receivedAttackNum; ++j){
			pBattleEffect->addEffectList(player[i], &player[i]->receivedAttackInfo[j]);
		}

		// エフェクトの追加
		if (player[i]->isBlock == true){
			// 相手の攻撃ガードされたフラグを変更
			player[i^PLAYER_CHANGER]->isAttackBlocked = true;
		} else{
			// 相手の攻撃ガードされたフラグを変更
			player[i^PLAYER_CHANGER]->isAttackBlocked = false;
		}

		// 実際に体力を減らす(仮)
		if (player[i]->isKO){
			player[i]->curHitPoint -= player[i]->hitDamage;
			if (player[i]->curHitPoint < 0){	// 死亡したら
				player[i]->curHitPoint = 0;
				player[i]->isKO = true;			// 死亡

				// KO_START状態
				if (player[i]->getCurActionState() != Actor::KO_START){	// KO_STARTモーションに入っていない場合
					player[i]->changeAnmNo(Actor::KO_START);
				}
			}
		}
	}

	//----------------------------------------------------------
	// 判定応答後処理
	for (int i = 0; i < PLAYER_NUM; ++i){
		// コンボの終了判定
		if (player[i^PLAYER_CHANGER]->attackHitStunCnt == 0 && !player[PLAYER_CHANGER]->isJuggle){	// 相手のヒット硬直がなくなった場合、コンボが切れる
			player[i]->cntCombo = 0;
		}

		// 相手がガードしていた場合はコンボを減算し、なかったことにする。
		for (int j = 0; j < player[i^PLAYER_CHANGER]->receivedAttackNum; ++j){
			if (player[i^PLAYER_CHANGER]->isBlock == true){
				player[i]->cntCombo--;
			}
		}

		checkCollisionScreenLimit(player[i]);
	}

	// 位置関係をチェック
	checkPlayerPosition();

	for (int i = 0; i < PLAYER_NUM; ++i){
		player[i]->checkPosState();				// 位置関係の確認(トリガーの変更)
		player[i]->changePolyReverseXState();	// 向きを変える,(実際に描画を反転する)

		player[i]->operatePartLandCancelWidthAttackReverse();	// isLandCancelableフラグをリセットする前に行う必要がある
		player[i]->checkLandingState();			// 着地モーションのキャンセル
		player[i]->operatePartReverseCancel();	// 振り向き行動のキャンセル
	}
}

void Battle::updateGameScreen(){
	//----------------------------------------------------------
	// ヒットストップがある場合は更新処理をしない
	if (hitStopCnt != 0){
		// ヒットストップの減算
		hitStopCnt--;
		if (hitStopCnt < 0)
			hitStopCnt = 0;

		return;
	}

	//----------------------------------------------------------
	// ステージ
	pStage->update();

	//----------------------------------------------------------
	// 更新処理
	// 行動より前に初期化しないといけない処理
	for (int i = 0; i < PLAYER_NUM; ++i){
		// トリガーリセット(仮)
		player[i]->flgTriggerChangeAction = false;
		player[i]->flgTriggerChangeAnm = false;
	}

	//----------------------------------------------------------
	// 行動
	// プレイヤーの状態、アニメーションの更新
	for (int i = 0; i < PLAYER_NUM; ++i){
		player[i]->updatePolygon();	// アニメーションの更新処理

		// 行動
		//player[i]->operateActor();

		player[i]->moveCoord();		// 座標にスピードを反映

		// 地上、空中の状態を取得
		if (player[i]->coord.y < pStage->groundPosY - player[i]->coord.h){	// 地面の座標より上にいるとき
			player[i]->isLand = false;
			player[i]->curSpeed.y += 1.0f;
		} else{															// 地上にいるとき
			player[i]->isLand = true;
			float tmpY = pStage->groundPosY - player[i]->coord.h;
			player[i]->setCoord(player[i]->coord.x, tmpY);

			player[i]->isJuggle = false;	// 確実に初期化できるので、ここで行う
		}
	}

	//----------------------------------------------------------
	// 行動後の初期化
	// プレイヤーのバトル時の情報を初期化
	for (int i = 0; i < PLAYER_NUM; ++i){
		// 当り判定の初期化(行動をした後にしなければならない)
		player[i]->initHurtBoxData();
		player[i]->initAttackBoxData();

		// 攻撃された情報を初期化
		player[i]->receivedAttackNum = 0;
		for (int j = 0; j < player[i]->RECEIVE_ATTACK_MAX; ++j)
			player[i]->receivedAttackInfo[j].initData();

		// 食らったダメージを初期化
		player[i]->hitDamage = 0;

		// ガードフラグを初期化
		player[i]->isBlock = false;
	}

	//----------------------------------------------------------
	// 判定処理
	// プレイヤー同士の衝突判定
	checkCollisionBodyBox(player[0], player[1]);

	for (int i = 0; i < PLAYER_NUM; ++i){
		// 攻撃判定のチェック
		checkCollisionAttackBox(player[i], player[i^PLAYER_CHANGER]);
	}

	//----------------------------------------------------------
	// 判定応答処理など
	for (int i = 0; i < PLAYER_NUM; ++i){
		// 判定応答
		player[i]->reactReceiveAttack();

		// エフェクトの追加
		if (player[i]->isBlock == true){
			// ガードエフェクト
		} else{
			// 攻撃エフェクト
			for (int j = 0; j < player[i]->receivedAttackNum; ++j){
				float centerX =
					player[i]->receivedAttackInfo[j].collideBox.x +
					player[i]->receivedAttackInfo[j].collideBox.w / 2.0f;

				float centerY =
					player[i]->receivedAttackInfo[j].collideBox.y +
					player[i]->receivedAttackInfo[j].collideBox.h / 2.0f;

				pBattleEffect->addEffectList(centerX, centerY, &player[i]->receivedAttackInfo[j]);
			}
		}

		// ヒットバックを減算(仮)
		if (fabs(player[i]->hitBack) > 0){
			if (pStage->isStageLimitX == false)
				player[i]->addCoordWithPosState(player[i]->hitBack, 0);
			else{
				// プレイヤーが画面にめり込んだ場合
				if (player[i]->isStageLimitX == true)
					player[i^PLAYER_CHANGER]->addCoordWithPosState(player[i]->hitBack, 0);	// 相手を押し出す。
				else
					player[i]->addCoordWithPosState(player[i]->hitBack, 0);
			}
			player[i]->hitBack++;
		}
	}

	//----------------------------------------------------------
	// 判定応答後処理
	for (int i = 0; i < PLAYER_NUM; ++i){
		// コンボの終了判定
		if (player[i^PLAYER_CHANGER]->attackHitStunCnt == 0 && !player[PLAYER_CHANGER]->isJuggle){	// 相手のヒット硬直がなくなった場合、コンボが切れる
			player[i]->cntCombo = 0;
		}

		// 相手がガードしていた場合はコンボを減算し、なかったことにする。
		for (int j = 0; j < player[i^PLAYER_CHANGER]->receivedAttackNum; ++j){
			if (player[i^PLAYER_CHANGER]->isBlock == true){
				player[i]->cntCombo--;
			}
		}

		checkCollisionScreenLimit(player[i]);
	}

	// 位置関係をチェック
	checkPlayerPosition();

	for (int i = 0; i < PLAYER_NUM; ++i){
		player[i]->checkPosState();				// 位置関係の確認(トリガーの変更)
		player[i]->changePolyReverseXState();	// 向きを変える,(実際に描画を反転する)
	}
}



void Battle::drawDebugInfo(){
	int   textY[2] = { 0, 0 };
	float textX[2] = {0, WINDOW_WIDTH / 2.0f};
	float textWidth = 8;
	float textHeight = 12;
	char buf[10];	// true, false 
	TextureUV *pUseTex = &gTexFont;	// 使用するフォントへのポインタ

	for (int i = 0; i < PLAYER_NUM; ++i){
		printfText(
			textX[i], textHeight * textY[i], textWidth, textHeight,
			0xFF, 0xFF, 0xFF,
			pUseTex,
			"Player isLandCancelable = %d", player[i]->isLandCancelable
			);
		textY[i]++;

		printfText(
			textX[i], textHeight * textY[i], textWidth, textHeight,
			0xFF, 0xFF, 0xFF,
			pUseTex,
			"Player isAttacAction = %d", player[i]->isAttackAction
			);
		textY[i]++;

		printfText(
			textX[i], textHeight * textY[i], textWidth, textHeight,
			0xFF, 0xFF, 0xFF,
			pUseTex,
			"Player isAttackBoxEnable = %d", player[i]->isAttackBoxEnable
			);
		textY[i]++;

		printfText(
			textX[i], textHeight * textY[i], textWidth, textHeight,
			0xFF, 0xFF, 0xFF,
			pUseTex,
			"Player isAttackHit = %d", player[i]->isAttackHit
			);
		textY[i]++;

		if (player[i]->getCurDirState() == Actor::RIGHT)stringCopy(buf, "RIGHT");
		else stringCopy(buf, "LEFT");
		printfText(
			textX[i], textHeight * textY[i], textWidth, textHeight,
			0xFF, 0xFF, 0xFF,
			pUseTex,
			"Player dirState = %s", buf
			);
		textY[i]++;

		if (player[i]->getCurPosState() == Actor::RIGHT)stringCopy(buf, "RIGHT");
		else stringCopy(buf, "LEFT");
		printfText(
			textX[i], textHeight * textY[i], textWidth, textHeight,
			0xFF, 0xFF, 0xFF,
			pUseTex,
			"Player posState = %s", buf
			);
		textY[i]++;

		if (player[i]->isCrossUpReceived)stringCopy(buf, "TRUE");
		else stringCopy(buf, "FLASE");
		printfText(
			textX[i], textHeight * textY[i], textWidth, textHeight,
			0xFF, 0xFF, 0xFF,
			pUseTex,
			"Player isCrossUpReceived = %s", buf
			);
		textY[i]++;

		if (player[i]->isCrossUpHitStun)stringCopy(buf, "TRUE");
		else stringCopy(buf, "FLASE");
		printfText(
			textX[i], textHeight * textY[i], textWidth, textHeight,
			0xFF, 0xFF, 0xFF,
			pUseTex,
			"Player isCrossUpHitStun = %s", buf
			);
		textY[i]++;

		if (player[i]->flgChangePosState)stringCopy(buf, "TRUE");
		else stringCopy(buf, "FLASE");
		printfText(
			textX[i], textHeight * textY[i], textWidth, textHeight,
			0xFF, 0xFF, 0xFF,
			pUseTex,
			"Player flgChangePosState = %s", buf
			);
		textY[i]++;

		if (player[i]->flgTriggerChangePosState)stringCopy(buf, "TRUE");
		else stringCopy(buf, "FLASE");
		printfText(
			textX[i], textHeight * textY[i], textWidth, textHeight,
			0xFF, 0xFF, 0xFF,
			pUseTex,
			"Player flgTriggerChangePosState = %s", buf
			);
		textY[i]++;

		if (player[i]->isCollideGuardBox)stringCopy(buf, "TRUE");
		else stringCopy(buf, "FLASE");
		printfText(
			textX[i], textHeight * textY[i], textWidth, textHeight,
			0xFF, 0xFF, 0xFF,
			pUseTex,
			"Player isCollideGuardBox = %s", buf
			);
		textY[i]++;

		printfText(
			textX[i], textHeight * textY[i], textWidth, textHeight,
			0xFF, 0xFF, 0xFF,
			pUseTex,
			"Player hitStunCnt = %d", player[i]->attackHitStunCnt
			);
		textY[i]++;

		printfText(
			textX[i], textHeight * textY[i], textWidth, textHeight,
			0xFF, 0xFF, 0xFF,
			pUseTex,
			"Player hitBack = %.1f", player[i]->hitBack
			);
		textY[i]++;

		printfText(
			textX[i], textHeight * textY[i], textWidth, textHeight,
			0xFF, 0xFF, 0xFF,
			pUseTex,
			"Player combo = %d", player[i]->cntCombo
			);
		textY[i]++;

		printfText(
			textX[i], textHeight * textY[i], textWidth, textHeight,
			0xFF, 0xFF, 0xFF,
			pUseTex,
			"rawInput = %x", player[i]->battleInput.curData.rawInputState
			);
		textY[i]++;

		printfText(
			textX[i], textHeight * textY[i], textWidth, textHeight,
			0xFF, 0xFF, 0xFF,
			pUseTex,
			"U D L R  1 2  A B  + -"
			);
		textY[i]++;

		printfText(
			textX[i], textHeight * textY[i], textWidth, textHeight,
			0xFF, 0xFF, 0xFF,
			pUseTex,
			"%d %d %d %d  %d %d  %d %d  %d %d (release)",
			player[i]->battleInput.release.buttonUp,
			player[i]->battleInput.release.buttonDown,
			player[i]->battleInput.release.buttonLeft,
			player[i]->battleInput.release.buttonRight,
			player[i]->battleInput.release.button1,
			player[i]->battleInput.release.button2,
			player[i]->battleInput.release.buttonA,
			player[i]->battleInput.release.buttonB,
			player[i]->battleInput.release.buttonPlus,
			player[i]->battleInput.release.buttonMinus
			);
		textY[i]++;

		printfText(
			textX[i], textHeight * textY[i], textWidth, textHeight,
			0xFF, 0xFF, 0xFF,
			pUseTex,
			"%d %d %d %d  %d %d  %d %d  %d %d (hold)",
			player[i]->battleInput.hold.buttonUp,
			player[i]->battleInput.hold.buttonDown,
			player[i]->battleInput.hold.buttonLeft,
			player[i]->battleInput.hold.buttonRight,
			player[i]->battleInput.hold.button1,
			player[i]->battleInput.hold.button2,
			player[i]->battleInput.hold.buttonA,
			player[i]->battleInput.hold.buttonB,
			player[i]->battleInput.hold.buttonPlus,
			player[i]->battleInput.hold.buttonMinus
			);
		textY[i]++;

		printfText(
			textX[i], textHeight * textY[i], textWidth, textHeight,
			0xFF, 0xFF, 0xFF,
			pUseTex,
			"%d %d %d %d  %d %d  %d %d  %d %d (trigger)",
			player[i]->battleInput.trigger.buttonUp,
			player[i]->battleInput.trigger.buttonDown,
			player[i]->battleInput.trigger.buttonLeft,
			player[i]->battleInput.trigger.buttonRight,
			player[i]->battleInput.trigger.button1,
			player[i]->battleInput.trigger.button2,
			player[i]->battleInput.trigger.buttonA,
			player[i]->battleInput.trigger.buttonB,
			player[i]->battleInput.trigger.buttonPlus,
			player[i]->battleInput.trigger.buttonMinus
			);
		textY[i]++;

		printfText(
			textX[i], textHeight * textY[i], textWidth, textHeight,
			0xFF, 0xFF, 0xFF,
			pUseTex,
			"%d %d %d %d  %d %d  %d %d  %d %d (curData)", 
			player[i]->battleInput.curData.buttonUp,
			player[i]->battleInput.curData.buttonDown,
			player[i]->battleInput.curData.buttonLeft,
			player[i]->battleInput.curData.buttonRight,
			player[i]->battleInput.curData.button1,
			player[i]->battleInput.curData.button2,
			player[i]->battleInput.curData.buttonA,
			player[i]->battleInput.curData.buttonB,
			player[i]->battleInput.curData.buttonPlus,
			player[i]->battleInput.curData.buttonMinus
			);
		textY[i]++;

		printfText(
			textX[i], textHeight * textY[i], textWidth, textHeight,
			0xFF, 0xFF, 0xFF,
			pUseTex,
			"%d %d %d %d  %d %d  %d %d  %d %d",
			player[i]->battleInput.pPrevData->buttonUp,
			player[i]->battleInput.pPrevData->buttonDown,
			player[i]->battleInput.pPrevData->buttonLeft,
			player[i]->battleInput.pPrevData->buttonRight,
			player[i]->battleInput.pPrevData->button1,
			player[i]->battleInput.pPrevData->button2,
			player[i]->battleInput.pPrevData->buttonA,
			player[i]->battleInput.pPrevData->buttonB,
			player[i]->battleInput.pPrevData->buttonPlus,
			player[i]->battleInput.pPrevData->buttonMinus
			);
		textY[i]++;

		BattleInputData *cur = player[i]->battleInput.pPrevData;
		for (int cnt = 0; cur != NULL && cnt < 10; cur = cur->next, cnt++){
			printfText(
				textX[i], textHeight * textY[i], textWidth, textHeight,
				0xFF, 0xFF, 0xFF,
				pUseTex,
				"cnt:%d, dir:%d", cur->cntInputUpdated, cur->directionKey
				);
			textY[i]++;
		}

	}
		
}