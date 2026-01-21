//==========================================================
// プレイヤーなど

#include	"Actor.h"			// Actorクラスを使用するため
#include	"Constants.h"		// inputNSなどの、入力用定数を使用するため
#include	"input.h"			// キャラクターを操作する関数ないで入力を使用するため(仮
#include	"ShareBaseFunc.h"	// battleInput.getBattleInputState関数を使用するため
#include	"BoundingBox.h"		// BoundingBoxクラスにアクセスするため
#include	"Debug.h"			// エラー処理を行うため
#include	"Common.h"
#include	<math.h>

#define		_CRT_SECURE_NO_DEPRECATE
#include	<stdio.h>			// FILE処理をするため

//==========================================================
// Actorクラス
//----------------------------------------------------------
// コンストラクタ
Actor::Actor() :
JUMP_FRAME(3)
{
	// 描画サイズの初期値
	initActorWidth = 100;	// 適当な値で初期化
	initActorHeight = 100;

	init();
}

//----------------------------------------------------------
/// デストラクタ
Actor::~Actor(){
	// 処理を記述
	// テクスチャ解放
	delete pTextureLoaderForGameReady;
	pTextureLoaderForGameReady = NULL;
	delete pTextureLoader;
	pTextureLoader = NULL;

	pOpponentActor = NULL;

}

void Actor::init(){
	// サイズを初期化
	actorWidth = initActorWidth;
	actorHeight = initActorHeight;

	// サイズの比率の初期化はキャラクターごとのコンストラクタで行う
	//actorWidthRatio = 1.0f;
	//actorHeightRatio = 1.0f;

	// 大きさの初期化(座標はステージクラスが初期化を行う)
	setData(0, 0, actorWidth, actorHeight);

	// 体力を初期化
	curHitPoint = maxHitPoint;

	// 現在のスピード
	curSpeed.x = 0;
	curSpeed.y = 0;

	// エラー防止のため初期化しておく
	for (int i = 0; i < BUFFER_MAX; i++){
		dirState[i] = RIGHT;
		posState[i] = LEFT;
	}

	// 攻撃情報の初期化
	receivedAttackNum = 0;
	for (int i = 0; i < RECEIVE_ATTACK_MAX; ++i){
		receivedAttackInfo[i].initData();	// 初期化
	}

	isStageLimitX = false;

	// KOフラグ
	isKO = false;	// 生きている状態に初期化

	// 着地フラグ
	isLand = false;

	isLanding = false;
	isLandCancelable = false;
	isAttackAction = false;
	isAttackHit = false;
	isAttackBlocked = false;

	// 位置関係が変わったときのフラグ
	flgChangePosState = false;
	flgTriggerChangePosState = false;	// 最初は固定されているため、変わっていない(適当な初期化)

	// アクションを起こしたときのトリガー
	flgTriggerChangeAction = true;	// 初期状態で変わるため(適当な初期化)
	flgTriggerChangeAnm = true;		// 

	isBlock = false;
	isCrossUpReceived = false;
	isCrossUpHitStun = false;
	isJuggle = false;

	isCollideGuardBox = 0;
	isReversing = false;

	// ガード状態
	guardState = NO_GUARD;

	// コンボ初期化
	cntCombo = 0;

	// 被攻撃情報
	hitBack = 0.0f;
	hitDamage = 0;
	attackHitStunCnt = 0;
	receivedAttackNum = 0;

	// 頂点カラー
	changeVertexColor(255, 255, 255);
}

//==========================================================
// 相手情報への参照の設定
void Actor::setOpponentActorPointer(Actor *_pActor){
	pOpponentActor = _pActor;
}

//==========================================================
// 当たり判定関係
//----------------------------------------------------------
// 当たり判定情報のメモリの確保
void Actor::allocHitBoxListArray(int _maxTex, int _maxState){
	wallBox = new WallBoundingBox[_maxTex];					// 他の当たり判定とは、データの持ち方が違う

	bodyBoxList = new BodyBoundingBoxList*[_maxState];
	hurtBoxList = new HurtBoundingBoxList*[_maxState];		// ポインタ配列の確保
	attackBoxList = new AttackBoundingBoxList*[_maxState];
	guardBoxList = new GuardBoundingBoxList*[_maxState];
}

//----------------------------------------------------------
// 当たり判定情報の二次元配列の開放
void Actor::freeHitBoxListArray(){
	delete[] wallBox;
	wallBox = NULL;

	delete[] bodyBoxList;
	bodyBoxList = NULL;
	delete[] hurtBoxList;
	hurtBoxList = NULL;
	delete[] attackBoxList;
	attackBoxList = NULL;
	delete[] guardBoxList;
	guardBoxList = NULL;
}

//----------------------------------------------------------
// リストの確保と、全HurtBoxデータへのポインタを代入
// ファイルからの読込
// 壁判定
void Actor::setWallBoxData(int _maxTex, hitBoxNS::eHitBoxLabel _label){
	MY_FILE file;
	char headerID[256];			// 先頭列のヘッダIDの読込先
	char checkEndID[256];		// 終端子の取得

	//==========================================================
	// 適当な初期化
	stringCopy(checkEndID, "");

	//==========================================================
	// ファイルオープン
	if (!fileOpen(&file, hitBoxNS::HITBOX_INFO[_label].getFilePath(), FILE_READ)){
		char buf[256];
		stringCopy(buf, hitBoxNS::HITBOX_INFO[_label].getFilePath());
		stringAdd(buf, "ファイルのオープンに失敗しました。(setWallBoxData関数内)");
		outErrorMessage(buf);
	}

	//==========================================================
	// 先頭のヘッダ読込処理
	int dataNum;
	fileRead(&file, "%[^,]", headerID);
	fileRead(&file, "%d", &dataNum);

	fileLineSkip(&file);

	if (dataNum != _maxTex){	// 必ず１
		char buf[256];
		stringCopy(buf, hitBoxNS::HITBOX_INFO[_label].getFilePath());
		stringAdd(buf, "の内容が正しくありません。(setWallBoxData関数内)");
		outErrorMessage(buf);
	}


	// 列の内容を示すヘッダを読み飛ばす
	fileLineSkip(&file);

	//==========================================================
	// ファイル読込
	for (int i = 0; i < dataNum; ++i){
		fileRead(&file, "%[^,]", headerID);
		fileRead(&file, "%[^,]", checkEndID);

		// 当たり判定取得
		fileRead(&file, "%f", &wallBox[i].coord.x);
		fileRead(&file, "%f", &wallBox[i].coord.y);
		fileRead(&file, "%f", &wallBox[i].coord.w);
		fileRead(&file, "%f", &wallBox[i].coord.h);

		wallBox[i].changeVertexColor(0xff, 0xff, 0xff);
		wallBox[i].setTexturePointer(&gTexAlpha);
		wallBox[i].isActive = true;
		wallBox[i].isHit = false;

		fileLineSkip(&file);
	}
	fileClose(&file);
}

// 体判定
void Actor::setBodyBoxData(int _maxState, hitBoxNS::eHitBoxLabel _label){
	MY_FILE file;
	char headerID[256];			// 先頭列のヘッダIDの読込先
	char checkEndID[256];		// 終端子の取得

	//==========================================================
	// 適当な初期化
	stringCopy(checkEndID, "");

	//==========================================================
	// ファイルオープン
	if (!fileOpen(&file, hitBoxNS::HITBOX_INFO[_label].getFilePath(), FILE_READ)){
		char buf[256];
		stringCopy(buf, hitBoxNS::HITBOX_INFO[_label].getFilePath());
		stringAdd(buf, "ファイルのオープンに失敗しました。(setBodyBoxData関数内)");
		outErrorMessage(buf);
	}

	//==========================================================
	// 先頭のヘッダ読込処理
	int dataNum;
	fileRead(&file, "%[^,]", headerID);
	fileRead(&file, "%d", &dataNum);

	fileLineSkip(&file);

	if (dataNum != _maxState){
		char buf[256];
		stringCopy(buf, hitBoxNS::HITBOX_INFO[_label].getFilePath());
		stringAdd(buf, "の内容が正しくありません。(setBodyBoxData関数内)");
		outErrorMessage(buf);
	}

	// 列の内容を示すヘッダを読み飛ばす
	fileLineSkip(&file);

	//==========================================================
	// ファイル読込
	for (int i = 0; i < dataNum; ++i){
		bodyBoxList[i] = new BodyBoundingBoxList;					// ダミー
		BodyBoundingBoxList *cur = bodyBoxList[i];

		// 1行動の複数の当たり判定を設定
		while (true){
			fileRead(&file, "%[^,]", headerID);
			fileRead(&file, "%[^,]", checkEndID);

			if (stringCompare(checkEndID, HITBOX_END)){	// 行動の終端子だった場合
				cur->next = NULL;						// リストの最後としてNULLをセット
				fileLineSkip(&file);					// 行読み飛ばし
				break;									// そのヒットボックスは以上
			}

			// 判定情報用メモリを確保
			cur->next = new BodyBoundingBoxList;
			cur = cur->next;

			// 当たり判定取得
			fileRead(&file, "%f", &cur->coord.x);
			fileRead(&file, "%f", &cur->coord.y);
			fileRead(&file, "%f", &cur->coord.w);
			fileRead(&file, "%f", &cur->coord.h);

			cur->changeVertexColor(BODYBOX_COLOR_R, BODYBOX_COLOR_G, BODYBOX_COLOR_B);
			cur->setTexturePointer(&gTexAlpha);
			cur->isActive = true;
			cur->isHit = false;

			fileLineSkip(&file);
		}
	}

	//==========================================================
	// 後処理
	fileClose(&file);
}

// 喰らい判定
void Actor::setHurtBoxData(int _maxState, hitBoxNS::eHitBoxLabel _label){
	MY_FILE file;
	char headerID[256];			// 先頭列のヘッダIDの読込先
	char checkEndID[256];		// 終端子の取得

	//==========================================================
	// 適当な初期化
	stringCopy(checkEndID, "");

	//==========================================================
	// ファイルオープン
	if (!fileOpen(&file, hitBoxNS::HITBOX_INFO[_label].getFilePath(), FILE_READ)){
		char buf[256];
		stringCopy(buf, hitBoxNS::HITBOX_INFO[_label].getFilePath());
		stringAdd(buf, "ファイルのオープンに失敗しました。(setHurtBoxData関数内)");
		outErrorMessage(buf);
	}

	//==========================================================
	// 先頭のヘッダ読込処理
	int dataNum;
	fileRead(&file, "%[^,]", headerID);
	fileRead(&file, "%d", &dataNum);

	fileLineSkip(&file);	// 行読み飛ばし

	if (dataNum != _maxState){
		char buf[256];
		stringCopy(buf, hitBoxNS::HITBOX_INFO[_label].getFilePath());
		stringAdd(buf, "ファイルの内容が正しくありません。(setHurtBoxData関数内)");
		outErrorMessage(buf);
	}

	// 列の内容を示すヘッダを読み飛ばす
	fileLineSkip(&file);	// 行読み飛ばし

	//==========================================================
	// ファイル読込
	for (int i = 0; i < dataNum; ++i){
		hurtBoxList[i] = new HurtBoundingBoxList;					// ダミー
		HurtBoundingBoxList *cur = hurtBoxList[i];

		// 1行動の複数の当たり判定を設定
		while (true){
			fileRead(&file, "%[^,]", headerID);
			fileRead(&file, "%[^,]", checkEndID);

			if (stringCompare(checkEndID, HITBOX_END)){	// 行動の終端子だった場合
				cur->next = NULL;						// リストの最後としてNULLをセット
				fileLineSkip(&file);					// 行読み飛ばし
				break;									// そのヒットボックスは以上
			}

			// 判定情報用メモリを確保
			cur->next = new HurtBoundingBoxList;
			cur = cur->next;

			// 当たり判定取得
			fileRead(&file, "%f", &cur->coord.x);
			fileRead(&file, "%f", &cur->coord.y);
			fileRead(&file, "%f", &cur->coord.w);
			fileRead(&file, "%f", &cur->coord.h);

			cur->changeVertexColor(HURTBOX_COLOR_R, HURTBOX_COLOR_G, HURTBOX_COLOR_B);
			cur->setTexturePointer(&gTexAlpha);
			cur->isActive = true;
			cur->isHit = false;

			fileLineSkip(&file);	// 行読み飛ばし
		}
	}

	//==========================================================
	// 後処理
	fileClose(&file);
}

// 攻撃判定
void Actor::setAttackBoxData(int _maxState, hitBoxNS::eHitBoxLabel _label){
	MY_FILE file;
	char headerID[256];			// 先頭列のヘッダIDの読込先
	char checkEndID[256];		// 終端子の取得

	//==========================================================
	// 適当な初期化
	stringCopy(checkEndID, "");

	//==========================================================
	// ファイルオープン
	if (!fileOpen(&file, hitBoxNS::HITBOX_INFO[_label].getFilePath(), FILE_READ)){
		char buf[256];
		stringCopy(buf, hitBoxNS::HITBOX_INFO[_label].getFilePath());
		stringAdd(buf, "ファイルのオープンに失敗しました。(setAttackBoxData関数内)");
		outErrorMessage(buf);
	}

	//==========================================================
	// 先頭のヘッダ読込処理
	int dataNum;
	fileRead(&file, "%[^,]", headerID);
	fileRead(&file, "%d", &dataNum);

	fileLineSkip(&file);		// 行の読み飛ばし

	if (dataNum != _maxState || false){
		char buf[256];
		stringCopy(buf, hitBoxNS::HITBOX_INFO[_label].getFilePath());
		stringAdd(buf, "ファイルの内容が正しくありません(setAttackBoxData関数内)");
		outErrorMessage(buf);
	}

	// 列の内容を示すヘッダを読み飛ばす
	fileLineSkip(&file);

	//==========================================================
	// ファイル読込
	for (int i = 0; i < dataNum; ++i){
		attackBoxList[i] = new AttackBoundingBoxList;					// ダミー
		AttackBoundingBoxList *cur = attackBoxList[i];

		// 1行動の複数の当たり判定を設定
		while (true){
			fileRead(&file, "%[^,]", headerID);
			fileRead(&file, "%[^,]", checkEndID);

			if (stringCompare(checkEndID, HITBOX_END)){		// 行動の終端子だった場合
				cur->next = NULL;							// リストの最後としてNULLをセット
				fileLineSkip(&file);							// 行の読み飛ばし
				break;										// そのヒットボックスは以上
			}

			// 判定情報用メモリを確保
			cur->next = new AttackBoundingBoxList;
			cur = cur->next;

			cur->attackInfo.initData();	// 初期化しておく

			// 当たり判定取得
			fileRead(&file, "%f", &cur->coord.x);
			fileRead(&file, "%f", &cur->coord.y);
			fileRead(&file, "%f", &cur->coord.w);
			fileRead(&file, "%f", &cur->coord.h);

			// 攻撃の属性
			fileRead(&file, "%d", (int*)&cur->attackInfo.strength);
			fileRead(&file, "%d", (int*)&cur->attackInfo.pos);
			fileRead(&file, "%d", (int*)&cur->attackInfo.type);
			fileRead(&file, "%d", (int*)&cur->attackInfo.attribute);

			// ダメージ量
			fileRead(&file, "%d", &cur->attackInfo.attackDamage);
			fileRead(&file, "%d", &cur->attackInfo.blockDamage);

			// ヒットストップ時間
			fileRead(&file, "%d", &cur->attackInfo.attackHitStopCnt);
			fileRead(&file, "%d", &cur->attackInfo.blockHitStopCnt);

			// ヒット硬直
			fileRead(&file, "%d", &cur->attackInfo.attackHitStunCnt);
			fileRead(&file, "%d", &cur->attackInfo.blockHitStunCnt);

			// ヒットバック
			fileRead(&file, "%f", &cur->attackInfo.attackHitBack);
			fileRead(&file, "%f", &cur->attackInfo.blockHitBack);

			cur->changeVertexColor(ATTACKBOX_COLOR_R, ATTACKBOX_COLOR_G, ATTACKBOX_COLOR_B);
			cur->setTexturePointer(&gTexAlpha);
			cur->isActive = true;
			cur->isHit = false;

			fileLineSkip(&file);		// 行の読み飛ばし
		}
	}

	//==========================================================
	// 後処理
	fileClose(&file);
}

// ガード判定
void Actor::setGuardBoxData(int _maxState, hitBoxNS::eHitBoxLabel _label){
	MY_FILE file;
	char headerID[256];			// 先頭列のヘッダIDの読込先
	char checkEndID[256];		// 終端子の取得

	//==========================================================
	// 適当な初期化
	stringCopy(checkEndID, "");

	//==========================================================
	// ファイルオープン
	if (!fileOpen(&file, hitBoxNS::HITBOX_INFO[_label].getFilePath(), FILE_READ)){
		char buf[256];
		stringCopy(buf, hitBoxNS::HITBOX_INFO[_label].getFilePath());
		stringAdd(buf, "ファイルのオープンに失敗しました。(setGuardBoxData関数内)");
		outErrorMessage(buf);
	}

	//==========================================================
	// 先頭のヘッダ読込処理
	int dataNum;
	fileRead(&file, "%[^,]", headerID);
	fileRead(&file, "%d", &dataNum);

	fileLineSkip(&file);	// 行読み飛ばし

	if (dataNum != _maxState){
		char buf[256];
		stringCopy(buf, hitBoxNS::HITBOX_INFO[_label].getFilePath());
		stringAdd(buf, "ファイルの内容が正しくありません。(setGuardBoxData関数内)");
		outErrorMessage(buf);
	}

	// 列の内容を示すヘッダを読み飛ばす
	fileLineSkip(&file);	// 行読み飛ばし

	//==========================================================
	// ファイル読込
	for (int i = 0; i < dataNum; ++i){
		guardBoxList[i] = new GuardBoundingBoxList;					// ダミー
		GuardBoundingBoxList *cur = guardBoxList[i];

		// 1行動の複数の当たり判定を設定
		while (true){
			fileRead(&file, "%[^,]", headerID);
			fileRead(&file, "%[^,]", checkEndID);

			if (stringCompare(checkEndID, HITBOX_END)){	// 行動の終端子だった場合
				cur->next = NULL;						// リストの最後としてNULLをセット
				fileLineSkip(&file);					// 行読み飛ばし
				break;									// そのヒットボックスは以上
			}

			// 判定情報用メモリを確保
			cur->next = new GuardBoundingBoxList;
			cur = cur->next;

			// 当たり判定取得
			fileRead(&file, "%f", &cur->coord.x);
			fileRead(&file, "%f", &cur->coord.y);
			fileRead(&file, "%f", &cur->coord.w);
			fileRead(&file, "%f", &cur->coord.h);

			cur->changeVertexColor(0x99, 0x00, 0xFF);
			cur->setTexturePointer(&gTexAlpha);
			cur->isActive = true;
			cur->isHit = false;

			fileLineSkip(&file);	// 行読み飛ばし
		}
	}

	//==========================================================
	// 後処理
	fileClose(&file);
}


//----------------------------------------------------------
// リストの解放
// 体判定
void Actor::freeBodyBoxList(int _maxState){
	BodyBoundingBoxList *cur, *work;

	for (int i = 0; i < _maxState; ++i){	// 全行動状態
		cur = bodyBoxList[i];				// 先頭をセット(ダミーデータの部分をセット) 

		while (cur != NULL){				// データの最後まで 
			work = cur->next;				// 次ポインタを保持
			delete cur;						// ポインタ開放
			cur = work;						// 次へ
		}
	}
}

// 喰らい判定
void Actor::freeHurtBoxList(int _maxState){
	HurtBoundingBoxList *cur, *work;

	for (int i = 0; i < _maxState; ++i){	// 全行動状態
		cur = hurtBoxList[i];				// 先頭をセット(ダミーデータの部分をセット) 

		while (cur != NULL){				// データの最後まで 
			work = cur->next;				// 次ポインタを保持
			delete cur;						// ポインタ開放
			cur = work;						// 次へ
		}
	}
}

// 攻撃判定
void Actor::freeAttackBoxList(int _maxState){
	AttackBoundingBoxList *cur, *work;

	for (int i = 0; i < _maxState; ++i){	// 全行動状態
		cur = attackBoxList[i];				// 先頭をセット(ダミーデータの部分をセット) 

		while (cur != NULL){				// データの最後まで 
			work = cur->next;				// 次ポインタを保持
			delete cur;						// ポインタ開放
			cur = work;						// 次へ
		}
	}
}

// ガード判定
void Actor::freeGuardBoxList(int _maxState){
	GuardBoundingBoxList *cur, *work;

	for (int i = 0; i < _maxState; ++i){	// 全行動状態
		cur = guardBoxList[i];				// 先頭をセット(ダミーデータの部分をセット) 

		while (cur != NULL){				// データの最後まで 
			work = cur->next;				// 次ポインタを保持
			delete cur;						// ポインタ開放
			cur = work;						// 次へ
		}
	}
}


//----------------------------------------------------------
// テクスチャのサイズに合わせたキャラクターポリゴンサイズを設定
// 使用するテクスチャを変更する際に呼び出す
void Actor::setCoordFromTextureDivRatio(){
	float widthRatio = pTextureLoader->getTexturePointer(curTexNo)->getHDivSizeRatio();		// 縦横比率の取得
	float heightRatio = pTextureLoader->getTexturePointer(curTexNo)->getVDivSizeRatio();

	//----------------------------------------------------------
	// サイズの設定
	if (widthRatio > heightRatio){						// 横のほうが長い
		actorHeight = initActorHeight;				// 短いほうに初期値を代入
		actorWidth = initActorWidth * widthRatio;		// 長いほうは、比率の分だけ、大きくする
	}
	else if (widthRatio < heightRatio){					// 縦のほうが長い
		actorWidth = initActorWidth;
		actorHeight = initActorHeight * heightRatio;
	}
	else{												// 1:1(同じ大きさ) 
		actorWidth = initActorWidth;					// 両方初期値
		actorHeight = initActorHeight;
	}

	//----------------------------------------------------------
	// 座標を設定
	// 現在(反映前)のポリゴンの基準点となる座標を取得
	float curCenterX = coord.x + coord.w / 2.0f;
	float curBottomY = coord.y + coord.h;

	// 変更後の座標を代入する変数
	float newX = curCenterX - actorWidth / 2.0f;	// 求めた幅からX座標を求める
	float newY = curBottomY - actorHeight;

	//----------------------------------------------------------
	// 描画用ポリゴンに座標を反映
	// 一応ネームスペースで解決しておく
	Polygon2D::setData(
		newX, newY,
		actorWidth, actorHeight
		);

	//----------------------------------------------------------
	// 比率の更新
	setWidthRatio();
	setHeightRatio();

}

//----------------------------------------------------------
// テクスチャの比率を取得(サイズを変更するときのみ呼び出す)
void Actor::setWidthRatio(){
	actorWidthRatio = actorWidth / pTextureLoader->getTexturePointer(curTexNo)->hDivSize;
}

void Actor::setHeightRatio(){
	actorHeightRatio = actorHeight / pTextureLoader->getTexturePointer(curTexNo)->vDivSize;
}


//----------------------------------------------------------
// 最大体力を設定する
void Actor::setMaxHitPointData(int maxHp){
	maxHitPoint = maxHp;
}

//----------------------------------------------------------
// スピードを設定する
void Actor::setMaxSpeedData(float _x, float _y){
	maxSpeed.x = _x;
	maxSpeed.y = _y;
}

//----------------------------------------------------------
// 使用するテクスチャの変更
void Actor::changeUseTexture(int no){
	curTexNo = no;
	setTexturePointer(pTextureLoader->getTexturePointer(curTexNo));

	// ここでサイズの変更
	setCoordFromTextureDivRatio();
}

//----------------------------------------------------------
// 仮)prevCoordを設定
void Actor::setPrevCoord(){
	for (int i = BUFFER_MAX - 1; i >= 0; --i)
		prevCoord[i] = prevCoord[i - 1];

	prevCoord[0] = coord; //[0]は現在座標ではない
}

//----------------------------------------------------------
// スピードを設定(引数は右向きを基準に)
void Actor::setCurSpeed(float _speedX, float _speedY){
	curSpeed.y = _speedY;
	curSpeed.x = _speedX;

	//if (getCurDirState() == RIGHT){
	//	curSpeed.x = _speedX;
	//}
	//else{
	//	curSpeed.x = _speedX;
	//}
}

//----------------------------------------------------------
// Actorの座標をセット
void Actor::setCoord(float x, float y){
	setPrevCoord();

	// Actor自体の座標
	coord.x = x;
	coord.y = y;

}

//----------------------------------------------------------
// 現在の座標に、引数の値分移動させる関数
// 位置関係で判断して、移動させる
void Actor::addCoordWithPosState(float x, float y){
	float tmpX = 0.0f;

	if (getCurPosState() == RIGHT)
		tmpX = -x;
	else
		tmpX = x;

	setCoord(coord.x + tmpX, coord.y + y);
}
// 向いている方向で判断して、移動させる(めくり処理で使用)
void Actor::addCoordWithDirState(float x, float y){
	float tmpX = 0.0f;

	if (getCurDirState() == LEFT)
		tmpX = -x;
	else
		tmpX = x;

	setCoord(coord.x + tmpX, coord.y + y);
}

//----------------------------------------------------------
// Actorを移動量分動かす
void Actor::moveCoord(){
	setPrevCoord();

	if (getCurDirState() == LEFT)
		coord.x -= curSpeed.x;
	else
		coord.x += curSpeed.x;

	coord.y += curSpeed.y;
}

//----------------------------------------------------------
// 向きを設定する
void Actor::setDirState(eDirectionState val){
	for (int i = BUFFER_MAX - 1; i > 0; --i)
		dirState[i] = dirState[i - 1];	// 現在の状態を前状態として保存

	dirState[0] = val;			// 現在の状態に代入
}

//----------------------------------------------------------
// 向きを設定する
void Actor::setPosState(eDirectionState val){
	for (int i = BUFFER_MAX - 1; i > 0; --i)
		posState[i] = posState[i - 1];	// 現在の状態を前状態として保存

	posState[0] = val;			// 現在の状態に代入
}

//----------------------------------------------------------
// PosStateの状態の変化を確認する
void Actor::checkPosState(){
	// posStateが変化したとき
	if (posState[0] != posState[1]){
		flgTriggerChangePosState = true;
		flgChangePosState = !flgChangePosState;	// 反転
	}
	else{
		flgTriggerChangePosState = false;
		// flgChangePosStateは戻さない
	}
}

//----------------------------------------------------------
// posStateの値により、dirStateを変更
void Actor::changeDirState(){
	if (posState[0] == LEFT)
		setDirState(RIGHT);
	else
		setDirState(LEFT);

	flgChangePosState = false;	// 変更完了
	isReversing = false;
}

//----------------------------------------------------------
// dirStateの値により、polygon2DのisReverseXを変更する
void Actor::changePolyReverseXState(){
	if (dirState[0] == LEFT)
		setIsReverseX(true);
	else
		setIsReverseX(false);
}

//----------------------------------------------------------
// 行動状態をセット
void Actor::changeActionState(int _no){
	for (int i = BUFFER_MAX - 1; i > 0; --i)
		actionState[i] = actionState[i - 1];	// 現在の状態を前状態として保存

	actionState[0] = _no;			// 現在の状態に代入

	flgTriggerChangeAction = true;	// 仮
}

//----------------------------------------------------------
// 入力状態から、現在のガード状態を設定する。
void Actor::checkGuardState(){
	// 空中にいる場合は、ガードできない
	if (!isLand || isAttackAction){
		guardState = NO_GUARD;
		return;
	}

	// ガードできない状態かどうかの判断
	int tmpActionState = getCurActionState();
	if (tmpActionState >= ST_GUARD &&				// ダメージ中はガードできない
		tmpActionState <= KO_END &&
		tmpActionState != ST_GUARD &&	// ただし、ガードしている場合はそのまま
		tmpActionState != CR_GUARD &&
		tmpActionState != ST_GUARD_END && 
		tmpActionState != CR_GUARD_END && 
		tmpActionState != ST_RECOVER &&	// ダメージ状態からの復帰時には、ガードができる状態になっている。
		tmpActionState != CR_RECOVER || 
		tmpActionState == LAND && !isLandCancelable ||
		tmpActionState == JUMP_BACK_START ||
		tmpActionState == JUMP_VIRTICAL_START ||
		tmpActionState == JUMP_FORWARD_START
		){
		guardState = NO_GUARD;
		return;
	}

	// 位置状態の違いでガード方向を変える(向きではなく位置、めくりのように向きが変わる前にガード方向を変えるため。)
	if (getCurPosState() == LEFT){
		if (battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_LEFT)){		// 後ろに入力している場合
			if (battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_DOWN))	// 下を入力している場合
				guardState = CROUCH_GUARD;		// しゃがみガード
			else
				guardState = STAND_GUARD;		// 立ちガード
		}
		else{
			if (attackHitStunCnt == 0)			// ヒット硬直中はガードが解除されることはない
				guardState = NO_GUARD;
		}
	}
	else{
		if (battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_RIGHT)){
			if (battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_DOWN))
				guardState = CROUCH_GUARD;
			else
				guardState = STAND_GUARD;
		}
		else
			if (attackHitStunCnt == 0)
				guardState = NO_GUARD;
	}
}

void Actor::checkLandingState(){
	int tmpCurActionState = getCurActionState();

	if (tmpCurActionState != LAND)
	{
		isLanding = false;	// 着地中ではなくなる
		isLandCancelable = false;	// 着地キャンセル可能フラグも変更しておく
	}
}

//----------------------------------------------------------
// 当り判定座標のワールド座標を求める
void Actor::calcAbsoluteCoord(FCoord2D *dest, const FCoord2D *srcHitBox){
	dest->x = (srcHitBox->x * actorWidthRatio) + coord.x;
	dest->y = (srcHitBox->y * actorHeightRatio) + coord.y;
	dest->w = (srcHitBox->w * actorWidthRatio);
	dest->h = (srcHitBox->h * actorHeightRatio);

	// 左向の場合、X座標を反転
	if (dirState[0] == LEFT)
		dest->x = (coord.x + coord.w) - ((srcHitBox->x * actorWidthRatio) + (srcHitBox->w * actorWidthRatio));
}

//----------------------------------------------------------
// 現在の行動状態を取得
int Actor::getCurActionState(){
	return actionState[0];
}


//----------------------------------------------------------
// 現在の行動状態を取得
int* Actor::getActionStateArray(){
	return actionState;
}

//----------------------------------------------------------
// 現在のdirStateを取得
Actor::eDirectionState Actor::getCurDirState(){
	return dirState[0];
}

//----------------------------------------------------------
// 現在のdirStateを取得
Actor::eDirectionState Actor::getCurPosState(){
	return posState[0];
}

//----------------------------------------------------------
// 現在のスピードの向いている方向を取得(動いてない場合、一応右を返すが、特に意味はない)
// 右向きを基準にする
Actor::eDirectionState Actor::getCurSpeedDirection(){
	if (getCurDirState() == RIGHT){
		if (curSpeed.x < 0)
			return LEFT;
		if (curSpeed.x > 0)
			return RIGHT;
	}
	else{
		if (curSpeed.x < 0)
			return RIGHT;
		if (curSpeed.x > 0)
			return LEFT;
	}

	return RIGHT;
}

// 
Actor::eGuardState Actor::getCurGuardState(){
	return guardState;
}

//==========================================================
// 描画
//----------------------------------------------------------
// 現在の当たり判定の描画
//----------------------------------------------------------
// 当り判定描画
void Actor::drawWallBox(){
	// 当り判定の相対座標にプレイヤーの座標を足し、絶対座標を求める
	FCoord2D absoluteCoord;

	// 現在のテクスチャに対応した座標
	calcAbsoluteCoord(&absoluteCoord, &wallBox[curTexNo].coord);
	wallBox[curTexNo].drawPolygon(&absoluteCoord);

}

void Actor::drawBodyBox(){
	// リスト配列(状態ごとの当り判定)の次がなくなるまで
	BodyBoundingBoxList *cur = bodyBoxList[getCurActionState()]->next;				// 現在位置を確保
	for (int i = 0; cur != NULL; ++i){
		// 当り判定の相対座標にプレイヤーの座標を足し、絶対座標を求める
		FCoord2D absoluteCoord;
		//calcAbsoluteCoord(&absoluteCoord, cur->pCoord);
		calcAbsoluteCoord(&absoluteCoord, &cur->coord);

		cur->drawPolygon(&absoluteCoord);	// 当り判定を描画
		cur = cur->next;					// 次へ
	}
}

void Actor::drawHurtBox(){
	// リスト配列(状態ごとの当り判定)の次がなくなるまで
	HurtBoundingBoxList *cur = hurtBoxList[getCurActionState()]->next;				// 現在位置を確保
	for (int i = 0; cur != NULL; ++i){
		// 当り判定の相対座標にプレイヤーの座標を足し、絶対座標を求める
		FCoord2D absoluteCoord;
		//calcAbsoluteCoord(&absoluteCoord, cur->pCoord);
		calcAbsoluteCoord(&absoluteCoord, &cur->coord);

#ifdef DEBUG_SHOW_HITBOX
		//// テスト用(バグあり)
		//if (cur->isHit == true)
		//	cur->changeVertexColor(0, 0, 255);
		//else
		//	cur->changeVertexColor(HURTBOX_COLOR_R, HURTBOX_COLOR_G, HURTBOX_COLOR_B);
#endif

		cur->drawPolygon(&absoluteCoord);	// 当り判定を描画
		cur = cur->next;					// 次へ
	}
}

void Actor::drawAttackBox(){
	// リスト配列(状態ごとの当り判定)の次がなくなるまで
	AttackBoundingBoxList *cur = attackBoxList[getCurActionState()]->next;				// 現在位置を確保
	for (int i = 0; cur != NULL; ++i){
		// 当り判定の相対座標にプレイヤーの座標を足し、絶対座標を求める
		FCoord2D absoluteCoord;
		//calcAbsoluteCoord(&absoluteCoord, cur->pCoord);
		calcAbsoluteCoord(&absoluteCoord, &cur->coord);

#ifdef DEBUG_SHOW_HITBOX
		//// テスト用(バグあり)
		//if (cur->isHit == true)
		//	cur->changeVertexColor(255,0,0);
		//else
		//	cur->changeVertexColor(ATTACKBOX_COLOR_R, ATTACKBOX_COLOR_G, ATTACKBOX_COLOR_B);
#endif

		cur->drawPolygon(&absoluteCoord);	// 当り判定を描画
		cur = cur->next;					// 次へ
	}
}

void Actor::drawGuardBox(){
	// リスト配列(状態ごとの当り判定)の次がなくなるまで
	GuardBoundingBoxList *cur = guardBoxList[getCurActionState()]->next;				// 現在位置を確保
	for (int i = 0; cur != NULL; ++i){
		// 当り判定の相対座標にプレイヤーの座標を足し、絶対座標を求める
		FCoord2D absoluteCoord;
		calcAbsoluteCoord(&absoluteCoord, &cur->coord);

		cur->drawPolygon(&absoluteCoord);	// 当り判定を描画
		cur = cur->next;					// 次へ
	}
}


//==========================================================
// Battleクラスから呼び出す関数
//----------------------------------------------------------
// 現在のActionStateの当り判定を初期化
// 喰らい判定
void Actor::initHurtBoxData(){
	HurtBoundingBoxList		*cur = hurtBoxList[getCurActionState()]->next;
	while (cur != NULL){
		cur->setIsHit(false);
		cur = cur->next;				// すすめる
	}
}

// 攻撃判定
void Actor::initAttackBoxData(){
	// 攻撃判定は行動が変わった時のみ、初期化する(攻撃があたっても判定が残ってしまうため)
	if (flgTriggerChangeAction == true)
	{
		AttackBoundingBoxList	*cur = attackBoxList[getCurActionState()]->next;
		while (cur != NULL){
			cur->setIsHit(false);
			cur = cur->next;			// すすめる
		}
	}
}


//----------------------------------------------------------
// 攻撃を受けたときの応答
void Actor::reactReceiveAttack(){
	int							damageAction = ST_GUARD;			// 受けた攻撃情報に対応した、アニメーションを格納する変数

	attackInfoNS::ePosition		damagePos = NOTHING_POS;			// 受けた攻撃を取得する(優先度がある)
	attackInfoNS::eStrength		damageStrength = NOTHING_STRENGTH;	
	attackInfoNS::eAttribute	damageAttribute = NO_ATTRIBUTE;

	//==========================================================
	// 初期化
	isBlock = false;	// ブロックしたフラグ

	//==========================================================
	// 攻撃を受けてない場合は、何もしない
	if (receivedAttackNum <= 0)
		return;

	//==========================================================
	// 受けた攻撃情報の整理
	for (int i = 0; i < receivedAttackNum; ++i) {
		if (isLand){	// 地上の処理
			// ダメージの強さを代入(強いダメージを優先するようにする)
			switch (receivedAttackInfo[i].strength){
				case attackInfoNS::LOW_STRENGTH:
					// より強い攻撃を受けていた場合は、それを優先する
					if (damageStrength > LOW_STRENGTH)
						break;

					// しゃがみ喰らいと、立ちくらい判断
					if (battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_DOWN))
						damageAction = CR_LOW_DAMAGE;
					else
						damageAction = ST_LOW_DAMAGE;
					break;

				case attackInfoNS::HIGH_STRENGTH:
					// より強い攻撃を受けていた場合は、それを優先する
					if (damageStrength > HIGH_STRENGTH)
						break;

					// しゃがみ喰らいと、立ちくらい判断
					if (battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_DOWN))
						damageAction = CR_HIGH_DAMAGE;
					else
						damageAction = ST_HIGH_DAMAGE;
					break;
			}
		} else{	// 空中なら、空中ダメージ
			damageAction = AIR_DAMAGE;
		}

		// テスト)空中追撃可能時に、攻撃をもらった場合は、強制的に空中ダメージにする
		if (isJuggle)
			damageAction = AIR_DAMAGE;

		//----------------------------------------------------------
		// ダメージの位置を代入(中段と下段が重なったら、上段とする。)
		switch (receivedAttackInfo[i].pos){
			case attackInfoNS::LOW_POS:
				if (damagePos == OVERHEAD_POS)
					damagePos = HIGH_POS;
				damagePos = LOW_POS;

				break;

			case attackInfoNS::OVERHEAD_POS:
				if (damagePos == LOW_POS)
					damagePos = HIGH_POS;
				damagePos = OVERHEAD_POS;
				break;

			case attackInfoNS::HIGH_POS:
				damagePos = HIGH_POS;
				break;

			case attackInfoNS::UNBLOCKABLE_POS:
				damagePos = UNBLOCKABLE_POS;
				break;
		}

		//----------------------------------------------------------
		// 属性を取得
		switch (receivedAttackInfo[i].attribute){
			case attackInfoNS::LAND_KNOCK_DOWN:
				damageAttribute = attackInfoNS::LAND_KNOCK_DOWN;
				break;

			case attackInfoNS::AIR_KNOCK_DOWN:
				if (damageAttribute == attackInfoNS::AIR_KNOCK_DOWN_JUGGLE)	// 上位互換なので、優先
					break;

				damageAttribute = attackInfoNS::AIR_KNOCK_DOWN;
				break;

			case attackInfoNS::AIR_KNOCK_DOWN_JUGGLE:
				damageAttribute = attackInfoNS::AIR_KNOCK_DOWN_JUGGLE;
				break;
		}
	}

	//==========================================================
	// 取得した攻撃情報から、プレイヤーの動作を決定する
	//----------------------------------------------------------
	// 攻撃をガードしたか、喰らったかを判定する。
	int tmpGuardState = getCurGuardState();	// ガード状態を取得し格納

	switch (damagePos){
		case UNBLOCKABLE_POS:
			// ガードできない
			break;

		case HIGH_POS:
			if (tmpGuardState == STAND_GUARD || tmpGuardState == CROUCH_GUARD){	// ガード方向へ入力していれば、ガード成功
				if (tmpGuardState == CROUCH_GUARD)	// しゃがみ状態ならしゃがみガード状態に変える
					damageAction = CR_GUARD;
				else								// 立ち状態へ
					damageAction = ST_GUARD;

				isBlock = true;
			}
			break;

		case OVERHEAD_POS:
			if (tmpGuardState == STAND_GUARD){
				damageAction = ST_GUARD;
				isBlock = true;
			}
			break;

		case LOW_POS:
			if (tmpGuardState == CROUCH_GUARD){
				damageAction = CR_GUARD;
				isBlock = true;
			}
	}

	//==========================================================
	// ガード、喰らい、でのダメージを取得する。
	for (int i = 0; i < receivedAttackNum; ++i) {
		if (isBlock){
			// 受けたダメージを保存しておく
			hitDamage += receivedAttackInfo[i].blockDamage;						// ダメージ量を加算

			// ヒット硬直を代入
			if (attackHitStunCnt < receivedAttackInfo[i].blockHitStunCnt)		// ヒット硬直時間が多いほうを代入
				attackHitStunCnt = receivedAttackInfo[i].blockHitStunCnt;		// ガード時のヒット硬直時間を格納

			// ヒットバックを代入
			if (fabs(hitBack) < fabs(receivedAttackInfo[i].blockHitBack))			// ヒットバックの大きいほうを代入
				hitBack = receivedAttackInfo[i].blockHitBack;					// ヒットバックを代入
		} 
		else{
			// 受けたダメージを保存しておく
			hitDamage += receivedAttackInfo[i].attackDamage;					// ダメージ量を加算

			// ヒット硬直を代入
			if (attackHitStunCnt < receivedAttackInfo[i].attackHitStunCnt)		// ヒット硬直時間が多いほうを代入
				attackHitStunCnt = receivedAttackInfo[i].attackHitStunCnt;		// ダメージ時のヒット硬直時間を格納

			// ヒットバックを代入
			if (fabs(hitBack) < fabs(receivedAttackInfo[i].attackHitBack))		// ヒットバックの大きいほうを代入
				hitBack = receivedAttackInfo[i].attackHitBack;					// ヒットバックを代入
		}
	}

	//==========================================================
	// 属性による、行動の変化を反映
	if (!isBlock){		// 攻撃を喰らったときは、属性の影響を受ける
		switch (damageAttribute){
			case attackInfoNS::LAND_KNOCK_DOWN:
				damageAction = Actor::LAND_KNOCK_DOWN;
				break;

			case attackInfoNS::AIR_KNOCK_DOWN:
				if (isLand)	// 自分が空中の場合のみ
					break;

				damageAction = AIR_KNOCK_DOWN;
				break;

			case attackInfoNS::AIR_KNOCK_DOWN_JUGGLE:
				if (isLand)	// 自分が空中の場合のみ
					break; 

				damageAction = AIR_KNOCK_DOWN;
				isJuggle = true;		// 空中追撃可能状態
				break;
		}
	}

	//==========================================================
	// 音を鳴らす
	if (isBlock)
		playSound(soundNS::TEST_BLOCK_SE);
	else
		playSound(soundNS::TEST_HIT_SE);

	//----------------------------------------------------------
	// 反映させる情報を取得
	// アニメーション、状態の変更(ダメージの強さにより変える)
	changeAnmNo(damageAction);				// ダメージアニメーションに変更する

	// 念のためスピードをリセット
	curSpeed.y = 0;
	curSpeed.x = 0;
}

//----------------------------------------------------------
// GuardBoxと判定があったときの処理(ガードモーションにする処理)
void Actor::reactReceiveGuardBox(){
	if (!isCollideGuardBox || receivedAttackNum > 0 || isAttackAction)
		return;

	//----------------------------------------------------------
	// 判定が有った場合
	// ガード状態を参照
	eGuardState	tmpCurGuardState = getCurGuardState();
	int	tmpCurActionState = getCurActionState();
	// ガードできない場合、ガードしている状態から、処理の中断判断
	if (tmpCurGuardState == Actor::NO_GUARD ||		// ガードできない状態場合
		tmpCurActionState >= Actor::ST_GUARD && tmpCurActionState <= KO_END)	// ダメージを食らっている場合(ガードの状態も含む)
	{
		return;
	}

	// ガード開始状態にする
	switch (tmpCurGuardState){
		case Actor::STAND_GUARD:
			changeAnmNo(ST_GUARD);
			break;

		case Actor::CROUCH_GUARD:
			changeAnmNo(CR_GUARD);
			break;
	}
}


//----------------------------------------------------------
// キャラ操作の標準を用意しておく
void Actor::operateActor(){
	switch (getCurActionState()){
		case ST_START:		// 通常状態関係
			operateActorST_START();
			break;

		case ST_COMPLETE:
			operateActorST_COMPLETE();
			break;

		case ST_REVERSE:
			operateActorST_REVERSE();
			break;

		case CR_START:
			operateActorCR_START();
			break;

		case CR_COMPLETE:
			operateActorCR_COMPLETE();
			break;

		case CR_REVERSE:
			operateActorCR_REVERSE();
			break;

		case WALK_FORWARD:
			operateActorWALK_FORWARD();
			break;

		case WALK_BACK:
			operateActorWALK_BACK();
			
			break;

		case JUMP_BACK_START:
			operateActorJUMP_BACK_START();
			break;

		case JUMP_VIRTICAL_START:
			operateActorJUMP_VIRTICAL_START();
			break;

		case JUMP_FORWARD_START:
			operateActorJUMP_FORWARD_START();
			break;


		case JUMP_BACK:
			operateActorJUMP_BACK();
			break;

		case JUMP_VIRTICAL:
			
			operateActorJUMP_VIRTICAL();
			break;

		case JUMP_FORWARD:
			
			operateActorJUMP_FORWARD();
			break;

		case LAND:
			operateActorLAND();
			
			break;


		case ST_GUARD:		// ダメージ関係
			operateActorST_GUARD();
			break;
		case ST_GUARD_END:
			operateActorST_GUARD_END();
			break;

		case ST_LOW_DAMAGE:
		case ST_HIGH_DAMAGE:
			operateActorST_DAMAGE();
			break;
		case ST_RECOVER:
			operateActorST_RECOVER();
			break;

		case CR_GUARD:
			operateActorCR_GUARD();
			break;
		case CR_GUARD_END:
			operateActorCR_GUARD_END();
			break;


		case CR_LOW_DAMAGE:
		case CR_HIGH_DAMAGE:
			operateActorCR_DAMAGE();
			break;
		case CR_RECOVER:
			operateActorCR_RECOVER();
			break;

		case AIR_DAMAGE:
			operateActorAIR_DAMAGE();
			break;

		case AIR_RECOVER:
			operateActorAIR_RECOVER();
			break;

		case LAND_KNOCK_DOWN:
			operateActorLAND_KNOCK_DOWN();
			break;
		case AIR_KNOCK_DOWN:
			operateActorAIR_KNOCK_DOWN();
			break;

		case KNOCK_DOWN_COMPLETE:
			operateActorKNOCK_DOWN_COMPLETE();
			break;
		case KNOCK_DOWN_RECOVER:
			operateActorKNOCK_DOWN_RECOVER();
			break;

		case KO_START:
			operateActorKO_START();
			break;
		case KO_COMPLETE:
			operateActorKO_COMPLETE();
			break;
		case KO_END:
			break;

		case POSE_ROUND_WIN_1:		// ポーズ関係
		case POSE_BATTLE_WIN_1:
			setCurSpeed(0, 0);
			break;

		case POSE_PROVOKE_1:
			operatePOSE();
			break;

		default:
			break;
	}
}

//==========================================================
// キャラクターの基本操作(どのキャラでも使用できる操作)
void Actor::operateActorST_START(){
	setCurSpeed(0, 0);

	if (isAnmEnd){
		changeAnmNo(ST_COMPLETE);
	}
	// 下ボタンが押されていたらCR_COMPLETEへ移行
	if (battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_DOWN)){
		changeAnmNo(CR_COMPLETE);	// たち
	}
	changeDirState();	// 強制で向きを変える

	operatePartAttackStart();
}

void Actor::operateActorCR_START(){
	setCurSpeed(0, 0);

	if (isAnmEnd){
		changeAnmNo(CR_COMPLETE);
	}

	// 下ボタンが押されてなかったらSTAND_COMPLETEへ移行
	if (!battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_DOWN)){
		changeAnmNo(ST_COMPLETE);	// たち
	}
	changeDirState();	// 強制で向きを変える

	operatePartAttackStart();
}

void Actor::operateActorST_REVERSE(){
	// 振り向き際は一度停止する
	setCurSpeed(0, 0);

	// 振り向き中フラグ
	isReversing = true;

	if (isAnmEnd == true){
		changeDirState();
		changeAnmNo(ST_COMPLETE);
	}
	operatePartAttackStart();
}

void Actor::operateActorCR_REVERSE(){
	// 振り向き際は一度停止する
	setCurSpeed(0, 0);

	isReversing = true;

	if (isAnmEnd == true){
		changeDirState();
		changeAnmNo(CR_COMPLETE);
	}
	operatePartAttackStart();
}

void Actor::operateActorST_COMPLETE(){
	setCurSpeed(0, 0);
	operatePartJumpStart();
	operatePartWalkStart();
	operatePartCrouchStart();
	operatePartStandReverseStart();
	operatePartAttackStart();
}

void Actor::operateActorCR_COMPLETE(){
	setCurSpeed(0, 0);

	if (!battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_DOWN)){
		operatePartStandStart();
		operatePartWalkStart();
		operatePartJumpStart();
	}

	operatePartCrouchReverseStart();
	operatePartAttackStart();
}

void Actor::operateActorWALK_FORWARD(){
	float tmpSpeed = 0;
	if (getCurDirState() == RIGHT){
		// 右ボタンが押されているときに
		if (battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_RIGHT))
			tmpSpeed = speedWalkForward;
	}
	// 左向きの場合
	else if (getCurDirState() == LEFT){
		// 左ボタンが押されているときに、左方向へ加速
		if (battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_LEFT))
			tmpSpeed = speedWalkForward;	// 加速処理
	}
	// スピードを反映
	curSpeed.x = tmpSpeed;

	operatePartStandStop();
	operatePartJumpStart();
	operatePartCrouchStart();
	operatePartStandReverseStart();
	operatePartAttackStart();
}

void Actor::operateActorWALK_BACK(){
	float tmpSpeed = 0;
	if (getCurDirState() == RIGHT){
		// 右ボタンが押されているときに
		if (battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_LEFT))
			tmpSpeed = -speedWalkBack;
	}
	// 左向きの場合
	else if (getCurDirState() == LEFT){
		// 左ボタンが押されているときに、左方向へ加速
		if (battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_RIGHT))
			tmpSpeed = -speedWalkBack;	// 加速処理
	}
	// スピードを反映
	curSpeed.x = tmpSpeed;

	operatePartStandStop();
	operatePartJumpStart();
	operatePartCrouchStart();
	operatePartStandReverseStart();
	operatePartAttackStart();
}

void Actor::operateActorJUMP_BACK_START(){
	setCurSpeed(0, 0);
	if (isAnmEnd){
		setCurSpeed(-speedJumpX, speedJumpY);
		changeAnmNo(JUMP_BACK);
	}
}

void Actor::operateActorJUMP_VIRTICAL_START(){
	setCurSpeed(0, 0);
	if (isAnmEnd){
		setCurSpeed(0, speedJumpY);
		changeAnmNo(JUMP_VIRTICAL);
	}
}

void Actor::operateActorJUMP_FORWARD_START(){
	setCurSpeed(0, 0);
	if (isAnmEnd){
		setCurSpeed(speedJumpX, speedJumpY);
		changeAnmNo(JUMP_FORWARD);
	}
}

void Actor::operateActorJUMP_BACK(){
	operatePartAttackStart();
	operatePartLandStart();	// 
}

void Actor::operateActorJUMP_VIRTICAL(){
	operatePartAttackStart();
	operatePartLandStart();	// 
}

void Actor::operateActorJUMP_FORWARD(){
	operatePartAttackStart();
	operatePartLandStart();	// 
}

void Actor::operateActorLAND(){
	setCurSpeed(0, 0);

	isLanding = true;

	if (isAnmEnd){
		if (battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_DOWN)){
			changeAnmNo(CR_COMPLETE);
			operatePartCrouchReverseStart();
		}
		else{
			changeAnmNo(ST_COMPLETE);
			operatePartStandReverseStart();
		}
		isLanding = false;
	}

	// キャンセルできるとき(共通項目はここに書くが、攻撃などは、キャラクター固有で処理する)
	if (isLandCancelable){
		operatePartCrouchStart();
		operatePartWalkStart();
		operatePartJumpStart();
		operatePartAttackStart();
	}
}

void Actor::operateActorST_GUARD(){
	setCurSpeed(0, 0);
	
	// めくりのときは、向きを変えない()
	if (!isCrossUpHitStun)
		changeDirState();

	if (attackHitStunCnt == 0 && !isCollideGuardBox){
		changeAnmNo(ST_GUARD_END);
	}

	if (attackHitStunCnt == 0){
		operatePartJumpStart();	// ガードモーション中(モーションのみ)は特定の行動でキャンセルできる
		operatePartWalkForwardStart();
		operatePartAttackStart();
	}
}

void Actor::operateActorST_GUARD_END(){
	setCurSpeed(0, 0);

	if (isAnmEnd){
		changeAnmNo(ST_COMPLETE);
	}

	operatePartJumpStart();	// ガードモーション中(モーションのみ)は特定の行動でキャンセルできる
	operatePartWalkStart();
	operatePartAttackStart();

}

void Actor::operateActorCR_GUARD(){
	setCurSpeed(0, 0);

	// めくりのときは、向きを変えない()
	if (!isCrossUpHitStun)
		changeDirState();

	if (attackHitStunCnt == 0 && !isCollideGuardBox){
		changeAnmNo(CR_GUARD_END);
	}

	if (attackHitStunCnt == 0){
		operatePartJumpStart();	// ガードモーション中(モーションのみ)は特定の行動でキャンセルできる
		operatePartWalkForwardStart();
		operatePartAttackStart();
	}
}

void Actor::operateActorCR_GUARD_END(){
	setCurSpeed(0, 0);

	if (isAnmEnd){
		changeAnmNo(CR_COMPLETE);
	}

	operatePartJumpStart();	// ガードモーション中(モーションのみ)は特定の行動でキャンセルできる
	operatePartWalkStart();
	operatePartAttackStart();
}

void Actor::operateActorST_DAMAGE(){
	bool isHitStupEnd = false;	// アニメーションエンドフラグ

	// めくりのときは、向きを変えない()
	if (!isCrossUpHitStun)
		changeDirState();

	// ヒット硬直が無くなったら
	if (attackHitStunCnt == 0){
		isHitStupEnd = true;
		isCrossUpHitStun = false;
	}

	if (isHitStupEnd)
		changeAnmNo(ST_RECOVER);
}

void Actor::operateActorCR_DAMAGE(){
	bool isHitStupEnd = false;	// アニメーションエンドフラグ

	if (!isCrossUpHitStun)
		changeDirState();

	// ヒット硬直が無くなったら
	if (attackHitStunCnt == 0){
		isHitStupEnd = true;
		isCrossUpHitStun = false;
	}

	if (isHitStupEnd)
		changeAnmNo(CR_RECOVER);
}

void Actor::operateActorAIR_DAMAGE(){
	changeDirState();		// 無条件で、相手の方向を向く

	operatePartAirDamageStart();

	// ヒット硬直が無くなったら
	if (isAnmEnd)
		changeAnmNo(AIR_RECOVER);

	if (isLand)	// 先に着地したら、立ち状態にする
		changeAnmNo(LAND);
}

void Actor::operateActorST_RECOVER(){
	if (isAnmEnd)
		changeAnmNo(ST_COMPLETE);
}

void Actor::operateActorCR_RECOVER(){
	if (isAnmEnd)
		changeAnmNo(CR_COMPLETE);
}

void Actor::operateActorAIR_RECOVER(){
	if (isLand)
		changeAnmNo(LAND);
}

void Actor::operateActorLAND_KNOCK_DOWN(){
	if (isAnmEnd)
		changeAnmNo(KNOCK_DOWN_COMPLETE);
}

void Actor::operateActorAIR_KNOCK_DOWN(){
	operatePartAirDamageStart();

	if (isLand){
		changeAnmNo(KNOCK_DOWN_COMPLETE);
	}
}

void Actor::operateActorKNOCK_DOWN_COMPLETE(){
	setCurSpeed(0, 0);
	if (isAnmEnd)
		changeAnmNo(KNOCK_DOWN_RECOVER);
}

void Actor::operateActorKNOCK_DOWN_RECOVER(){
	setCurSpeed(0, 0);

	if (isAnmEnd)
		changeAnmNo(ST_COMPLETE);
}

void Actor::operateActorKO_START(){
	// 仮)
	if (anmCnt <= 1){
		setCurSpeed(-4, -12);
		return;
	}

	if (isLand)
		changeAnmNo(KO_COMPLETE);
}

void Actor::operateActorKO_COMPLETE(){
	setCurSpeed(0, 0);

	if (isAnmEnd)
		changeAnmNo(KO_END);
}

void Actor::operatePOSE(){
	setCurSpeed(0, 0);
	if (isAnmEnd)
		changeAnmNo(ST_COMPLETE);
}



//----------------------------------------------------------
// 行動内の操作関数
void Actor::operatePartStandStart(){
	// 下ボタンが押されてなかったらSTAND_COMPLETEへ移行
	if (!battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_DOWN))
		changeAnmNo(ST_START);
}

void Actor::operatePartCrouchStart(){
	if (!battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_DOWN))
		return;

	// 下ボタンが押されていたらしゃがみ
	changeAnmNo(CR_START);
}

void Actor::operatePartStandReverseStart(){
	// 向きが変わったフラグがtrueの場合
	if (flgChangePosState)
		changeAnmNo(ST_REVERSE);
}

void Actor::operatePartCrouchReverseStart(){
	// 向きが変わったフラグがtrueの場合
	if (flgChangePosState)
		changeAnmNo(CR_REVERSE);
}

void Actor::operatePartStandStop(){
	// 完全に止まったらSTAND_COMPLETEへ移行
	if (fabs(curSpeed.x) < 1.0f)		// float のため範囲で指定(スピードの絶対値が1未満の場合)
		changeAnmNo(ST_COMPLETE);
}

void Actor::operatePartWalkStart(){
	if (battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_DOWN))
		return;

	operatePartWalkBackStart();
	operatePartWalkForwardStart();
}

void Actor::operatePartWalkForwardStart(){
	// 右向きの場合
	if (getCurDirState() == RIGHT){
		// 右ボタンが押されていれば、FOWERDWALKへいこう
		if (battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_RIGHT))
			changeAnmNo(WALK_FORWARD);	// 歩き
	}
	// 左向きの場合
	else if (getCurDirState() == LEFT){
		// 左ボタンが押されていれば、FOWERDWALKへ移行
		if (battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_LEFT))
			changeAnmNo(WALK_FORWARD);
	}
}

void Actor::operatePartWalkBackStart(){
	// 右向きの場合
	if (getCurDirState() == RIGHT){
		// 左ボタンが押されていれば、BACKWALKへ移行
		if (battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_LEFT))
			changeAnmNo(WALK_BACK);
	}
	// 左向きの場合
	else if (getCurDirState() == LEFT){
		// 右ボタンが押されていれば、BACKWALKへいこう
		if (battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_RIGHT))
			changeAnmNo(WALK_BACK);	// 歩き
	}
}


void Actor::operatePartJumpStart(){
	// 上ボタンが押されていないなら、この関数に用はない
	if (!battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_UP))
		return;

	// 右を向いているとき
	if (getCurDirState() == Actor::RIGHT){
		// 左が押されていたら
		if (battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_LEFT))
			changeAnmNo(JUMP_BACK_START);
		// 右がおされていたら
		else if (battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_RIGHT))
			changeAnmNo(JUMP_FORWARD_START);
		else
			changeAnmNo(JUMP_VIRTICAL_START);
	}
	else if (getCurDirState() == Actor::LEFT){
		// 左が押されていたら
		if (battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_LEFT))
			changeAnmNo(JUMP_FORWARD_START);
		// 右がおされていたら
		else if (battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_RIGHT))
			changeAnmNo(JUMP_BACK_START);
		else
			changeAnmNo(JUMP_VIRTICAL_START);
	}
}

void Actor::operatePartLandStart(){
	if (isLand == true){	// 前状態が空中のとき。という判断がないとバグが起こる可能性がある
		// 着地状態をキャンセルできる状態かどうかをチェック
		if (!isAttackAction)				// 攻撃を出していないとき
			isLandCancelable = true;
		if (isAttackAction && isAttackHit)	// 攻撃を当てたとき
			isLandCancelable = true;

		setCurSpeed(0, 0);
		changeAnmNo(LAND);
	}
}

void Actor::operatePartAirDamageStart(){
	if (isLand)
		return;

	if (anmCnt <= 1)
		setCurSpeed(-4, -8);
}

void Actor::operatePartAttackStartInit(){
	// 攻撃前の初期化
	isAttackBoxEnable = true;	// 攻撃を開始したときは、有効にしておく
	isAttackHit = false;		// 攻撃はまだ当たっていない
}

void Actor::operatePartReverseCancel(){
	int tmpActionState = getCurActionState();

	// 強制振り向き
	if (isReversing &&	// 振り向き中に、振り向き行動がキャンセルされた場合(攻撃のみ)
		tmpActionState != ST_REVERSE &&
		tmpActionState != CR_REVERSE && 
		isAttackAction)
	{
		changeDirState();
	}
}

void Actor::operatePartLandCancelWidthAttackReverse(){
	if (!isLandCancelable)
		return;

	int tmpCurActionState = getCurActionState();
	if (isAttackAction ||								// 攻撃で、着地モーションをキャンセルした場合
		tmpCurActionState == JUMP_BACK_START ||			// ジャンプでキャンセルした場合
		tmpCurActionState == JUMP_VIRTICAL_START ||
		tmpCurActionState == JUMP_FORWARD_START)
		changeDirState();								// 強制で振り向かせる


	// 仮)(実装の仕方が場当たり的)向きが変わったときは、ジャンプ方向が変化するので、プログラムで変更する
	if (dirState[0] != dirState[1]){
		if (tmpCurActionState == JUMP_BACK_START)
			changeAnmNo(JUMP_FORWARD_START);
		if (tmpCurActionState == JUMP_FORWARD_START)
			changeAnmNo(JUMP_BACK_START);
	}
}