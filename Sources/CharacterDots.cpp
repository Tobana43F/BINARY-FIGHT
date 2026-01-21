//==========================================================
// インクルード
#include	"Actor.h"			// Actorクラスを使用するため
#include	"CharacterDots.h"	// このクラスの定義

#include	"Constants.h"		// BattleInputManagerなどの、入力用定数を使用するため
#include	"input.h"			// キャラクターを操作する関数ないで入力を使用するため(仮
#include	"ShareBaseFunc.h"	// getInputState関数を使用するため
#include	"BoundingBox.h"		// BoundingBoxクラスにアクセスするため
#include	"Common.h"
#include	<math.h>

//==========================================================
// メソッド定義
//----------------------------------------------------------
// コンストラクタ
CharDots::CharDots():
MAX_HP(1000),
SPEED_WALK_FORWARD(5.5f),
SPEED_WALK_BACK(5.0f),
SPEED_JUMP_Y(-18.5f),
SPEED_JUMP_X(5.0f)
{
	// VS画面で表示するテクスチャ(このテクスチャだけは、コンストラクタで読み込んでおく。)
	pTextureLoaderForGameReady = new TextureLoader(MAX_VS_TEX);
	//pTextureLoaderForGameReady->setUseTextureLabel(VS_TEX, textureNS::GAME_READY_DOTS);
	pTextureLoaderForGameReady->setUseTextureLabel(VS_TEX, textureNS::CHARACTER_SELECT_DOTS);
	pTextureLoaderForGameReady->loadUseTexture();

	// キャラクターのメインのテクスチャ
	pTextureLoader = new TextureLoader(MAX_TEX);
	// テクスチャの設定
	pTextureLoader->setUseTextureLabel(NORMAL_TEX, textureNS::CHAR_DOTS_NORMAL);
	pTextureLoader->setUseTextureLabel(DAMAGE_TEX, textureNS::CHAR_DOTS_DAMAGE);
	pTextureLoader->setUseTextureLabel(ATTACK_NORMAL_TEX, textureNS::CHAR_DOTS_ATTACK_NORMAL);
	pTextureLoader->setUseTextureLabel(POSE_TEX, textureNS::CHAR_DOTS_POSE);

	// アニメーションの設定
	pTextureLoader->setUseAnimationLabel(NORMAL_TEX, animationNS::CHAR_DOTS_NORMAL);
	pTextureLoader->setUseAnimationLabel(DAMAGE_TEX, animationNS::CHAR_DOTS_DAMAGE);
	pTextureLoader->setUseAnimationLabel(ATTACK_NORMAL_TEX, animationNS::CHAR_DOTS_ATTACK_NORMAL);
	pTextureLoader->setUseAnimationLabel(POSE_TEX, animationNS::CHAR_DOTS_POSE);

	// 定数としての設定
	initActorWidth = 250;
	initActorHeight = 250;

	initCharacter();// 固有の初期化
}

//----------------------------------------------------------
// デストラクタ
CharDots::~CharDots(){
	// 当たり判定用メモリを解放
	freeBodyBoxList(MAX_STATE);
	freeHurtBoxList(MAX_STATE);
	freeAttackBoxList(MAX_STATE);
	freeGuardBoxList(MAX_STATE);

	// 旗利判定配列を解放
	freeHitBoxListArray();	// リスト解放以降
}

//==========================================================
// Actorの関数をオーバーライドした関数
//----------------------------------------------------------
// 初期化
void CharDots::initCharacter(){
	for (int i = 0; i < BUFFER_MAX; ++i)
		actionState[i] = ST_COMPLETE;

	changeAnmNo(ST_COMPLETE);
	
	// 移動速度などを設定
	//setMaxSpeedData(SPEED_X, JUMP_POW);

	//----------------------------------------------------------
	// 定数を上位クラスの変数に代入
	// 体力を設定
	setMaxHitPointData(MAX_HP);
	speedJumpY = SPEED_JUMP_Y;
	speedJumpX = SPEED_JUMP_X;
	speedWalkForward = SPEED_WALK_FORWARD;
	speedWalkBack = SPEED_WALK_BACK;
}

//----------------------------------------------------------
// ロード
void CharDots::load(){
	// キャラクターテクスチャの読み込み
	pTextureLoader->loadUseTexture();
	pTextureLoader->loadUseAnimation();

	// 当たり判定確保
	allocHitBoxListArray(MAX_TEX, MAX_STATE);
	setWallBoxData(MAX_TEX, hitBoxNS::DOTS_WALL_BOX);			// 壁判定
	setBodyBoxData(MAX_STATE, hitBoxNS::DOTS_BODY_BOX);			// 体判定のデータ
	setHurtBoxData(MAX_STATE, hitBoxNS::DOTS_HURT_BOX);			// 当り判定データの設定
	setAttackBoxData(MAX_STATE, hitBoxNS::DOTS_ATTACK_BOX);		// 攻撃判定データ
	setGuardBoxData(MAX_STATE, hitBoxNS::DOTS_GUARD_BOX);
}


//----------------------------------------------------------
// アニメーションナンバーの変更
void CharDots::changeAnmNo(int _anmNo){
	int tmpAnmNo;
	int tmpUseTex;

	// テクスチャの選択とアニメーションの設定
	if (_anmNo >= ST_ATTACK_NT_LOW_START){
		tmpUseTex = ATTACK_NORMAL_TEX;
		tmpAnmNo = _anmNo - ST_ATTACK_NT_LOW_START;
	}
	else if (_anmNo >= POSE_ROUND_WIN_1){
		tmpUseTex = POSE_TEX;
		tmpAnmNo = _anmNo - POSE_ROUND_WIN_1;
	}
	else if (_anmNo >= ST_GUARD){
		tmpUseTex = DAMAGE_TEX;
		tmpAnmNo = _anmNo - ST_GUARD;
	}
	else if (_anmNo >= ST_START){
		tmpUseTex = NORMAL_TEX;
		tmpAnmNo = _anmNo - ST_START;
	}

	// フラグの変更
	flgTriggerChangeAnm = true;

	// テクスチャ、アニメーション番号を実際に変更
	changeActionState(_anmNo);
	changeUseTexture(tmpUseTex);
	Polygon2D::changeAnmNo(tmpAnmNo);
}

//----------------------------------------------------------
// 攻撃行動フラグの変更
void CharDots::changeIsAttackAction(){
	int tmpActionState = getCurActionState();
	if (tmpActionState >= ST_ATTACK_NT_LOW_START &&			// 行動リストの、攻撃の最小値
		tmpActionState <= FJ_ATTACK_NT_HIGH_ATTACK_END){	// 行動リストの、攻撃の最大値
		isAttackAction = true;
	}
	else{
		isAttackAction = false;
	}
}

//----------------------------------------------------------
// キャラクターの状態ごとの操作(仮:人間の操作しかできない(COMが使えない))
void CharDots::operateActor(){
	//==========================================================
	// 全てのキャラクター共通の操作
	Actor::operateActor();

	//==========================================================
	// キャラクター固有の操作
	switch (getCurActionState()){
		case ST_ATTACK_NT_LOW_START:
			setCurSpeed(0, 0);
			if (isAnmEnd){ changeAnmNo(ST_ATTACK_NT_LOW_ATTACK_ON_1); }
			break;
		case ST_ATTACK_NT_LOW_ATTACK_ON_1:
			if (isAnmEnd){ changeAnmNo(ST_ATTACK_NT_LOW_ATTACK_OFF_1); }
			break;
		case ST_ATTACK_NT_LOW_ATTACK_OFF_1:
			if (isAnmEnd){ changeAnmNo(ST_ATTACK_NT_LOW_ATTACK_END); }
			
			break;
		case ST_ATTACK_NT_LOW_ATTACK_END:
			if (isAnmEnd){ changeAnmNo(ST_COMPLETE); }
			break;

		case ST_ATTACK_FW_LOW_START:
			setCurSpeed(0, 0);
			if (isAnmEnd){ changeAnmNo(ST_ATTACK_FW_LOW_ATTACK_ON_1); }
			break;
		case ST_ATTACK_FW_LOW_ATTACK_ON_1:
			if (isAnmEnd){ changeAnmNo(ST_ATTACK_FW_LOW_ATTACK_OFF_1); }
			break;
		case ST_ATTACK_FW_LOW_ATTACK_OFF_1:
			if (isAnmEnd){ changeAnmNo(ST_ATTACK_FW_LOW_ATTACK_END); }
			break;
		case ST_ATTACK_FW_LOW_ATTACK_END:
			if (isAnmEnd){ changeAnmNo(ST_COMPLETE); }
			break;

		case CR_ATTACK_NT_LOW_START:
			setCurSpeed(0, 0);
			if (isAnmEnd){ changeAnmNo(CR_ATTACK_NT_LOW_ATTACK_ON_1); }
			break;
		case CR_ATTACK_NT_LOW_ATTACK_ON_1:
			if (isAnmEnd){ changeAnmNo(CR_ATTACK_NT_LOW_ATTACK_OFF_1); }
			break;
		case CR_ATTACK_NT_LOW_ATTACK_OFF_1:
			if (isAnmEnd){ changeAnmNo(CR_ATTACK_NT_LOW_ATTACK_END); }
			break;
		case CR_ATTACK_NT_LOW_ATTACK_END:
			if (isAnmEnd){ changeAnmNo(CR_COMPLETE); }
			break;

		case CR_ATTACK_FW_LOW_START:
			setCurSpeed(0, 0);
			if (isAnmEnd){ changeAnmNo(CR_ATTACK_FW_LOW_ON_1); }
			break;
		case CR_ATTACK_FW_LOW_ON_1:
			if (isAnmEnd){ changeAnmNo(CR_ATTACK_FW_LOW_OFF_1); }
			break;
		case CR_ATTACK_FW_LOW_OFF_1:
			if (isAnmEnd){ changeAnmNo(CR_ATTACK_FW_LOW_END); }
			break;
		case CR_ATTACK_FW_LOW_END:
			if (isAnmEnd){ changeAnmNo(CR_COMPLETE); }
			break;

		case VJ_ATTACK_NT_LOW_ATTACK_START:
			if (isAnmEnd){ changeAnmNo(VJ_ATTACK_NT_LOW_ATTACK_ON_1); }
			if (isLand){ operatePartLandStart(); }
			break;
		case VJ_ATTACK_NT_LOW_ATTACK_ON_1:
			if (isAnmEnd){ changeAnmNo(VJ_ATTACK_NT_LOW_ATTACK_OFF_1); }
			if (isLand){ operatePartLandStart(); }
			break;
		case VJ_ATTACK_NT_LOW_ATTACK_OFF_1:
			if (isAnmEnd){ changeAnmNo(VJ_ATTACK_NT_LOW_ATTACK_END); }
			if (isLand){ operatePartLandStart(); }
			break;
		case VJ_ATTACK_NT_LOW_ATTACK_END:
			if (isLand){ operatePartLandStart(); }
			break;

		case FJ_ATTACK_NT_LOW_ATTACK_START:
			if (isAnmEnd){ changeAnmNo(FJ_ATTACK_NT_LOW_ATTACK_ON_1); }
			if (isLand){ operatePartLandStart(); }
			break;
		case FJ_ATTACK_NT_LOW_ATTACK_ON_1:
			if (isAnmEnd){ changeAnmNo(FJ_ATTACK_NT_LOW_ATTACK_OFF_1); }
			if (isLand){ operatePartLandStart(); }
			break;
		case FJ_ATTACK_NT_LOW_ATTACK_OFF_1:
			if (isAnmEnd){ changeAnmNo(FJ_ATTACK_NT_LOW_ATTACK_END); }
			if (isLand){ operatePartLandStart(); }
			break;
		case FJ_ATTACK_NT_LOW_ATTACK_END:
			if (isLand){ operatePartLandStart(); }
			break;


		case ST_ATTACK_NT_HIGH_START:
			setCurSpeed(0, 0);
			if (isAnmEnd){ changeAnmNo(ST_ATTACK_NT_HIGH_ATTACK_ON_1); }
			break;
		case ST_ATTACK_NT_HIGH_ATTACK_ON_1:
			setCurSpeed(1, 0);
			if (isAnmEnd){ changeAnmNo(ST_ATTACK_NT_HIGH_ATTACK_OFF_1); }
			break;
		case ST_ATTACK_NT_HIGH_ATTACK_OFF_1:
			setCurSpeed(0, 0);
			if (isAnmEnd){ changeAnmNo(ST_ATTACK_NT_HIGH_ATTACK_END); }
			break;
		case ST_ATTACK_NT_HIGH_ATTACK_END:
			if (isAnmEnd){ changeAnmNo(ST_COMPLETE); }
			break;

		case ST_ATTACK_FW_HIGH_START:
			setCurSpeed(0.2f, 0);
			if (isAnmEnd){ changeAnmNo(ST_ATTACK_FW_HIGH_ATTACK_ON_1); }
			break;
		case ST_ATTACK_FW_HIGH_ATTACK_ON_1:
			setCurSpeed(2, 0);
			if (isAnmEnd){ changeAnmNo(ST_ATTACK_FW_HIGH_ATTACK_OFF_1); }
			break;
		case ST_ATTACK_FW_HIGH_ATTACK_OFF_1:
			setCurSpeed(0, 0);
			if (isAnmEnd){ changeAnmNo(ST_ATTACK_FW_HIGH_ATTACK_END); }
			break;
		case ST_ATTACK_FW_HIGH_ATTACK_END:
			if (isAnmEnd){ changeAnmNo(ST_COMPLETE); }
			break;

		case CR_ATTACK_NT_HIGH_START:
			setCurSpeed(0, 0);
			if (isAnmEnd){ changeAnmNo(CR_ATTACK_NT_HIGH_ATTACK_ON_1); }
			break;
		case CR_ATTACK_NT_HIGH_ATTACK_ON_1:
			if (isAnmEnd){ changeAnmNo(CR_ATTACK_NT_HIGH_ATTACK_OFF_1); }
			break;
		case CR_ATTACK_NT_HIGH_ATTACK_OFF_1:
			if (isAnmEnd){ changeAnmNo(CR_ATTACK_NT_HIGH_ATTACK_END); }
			break;
		case CR_ATTACK_NT_HIGH_ATTACK_END:
			if (isAnmEnd){ changeAnmNo(CR_COMPLETE); }
			break;

		case CR_ATTACK_FW_HIGH_START:
			setCurSpeed(0, 0);
			if (isAnmEnd){ changeAnmNo(CR_ATTACK_FW_HIGH_ATTACK_ON_1); }
			break;
		case CR_ATTACK_FW_HIGH_ATTACK_ON_1:
			if (isAnmEnd){ changeAnmNo(CR_ATTACK_FW_HIGH_ATTACK_OFF_1); }
			break;
		case CR_ATTACK_FW_HIGH_ATTACK_OFF_1:
			if (isAnmEnd){ changeAnmNo(CR_ATTACK_FW_HIGH_ATTACK_END); }
			break;
		case CR_ATTACK_FW_HIGH_ATTACK_END:
			if (isAnmEnd){ changeAnmNo(CR_COMPLETE); }
			break;

		case VJ_ATTACK_NT_HIGH_ATTACK_START:
			if (isAnmEnd){ changeAnmNo(VJ_ATTACK_NT_HIGH_ATTACK_ON_1); }
			if (isLand){ operatePartLandStart(); }
			break;
		case VJ_ATTACK_NT_HIGH_ATTACK_ON_1:
			if (isAnmEnd){ changeAnmNo(VJ_ATTACK_NT_HIGH_ATTACK_OFF_1); }
			if (isLand){ operatePartLandStart(); }
			break;
		case VJ_ATTACK_NT_HIGH_ATTACK_OFF_1:
			if (isAnmEnd){ changeAnmNo(VJ_ATTACK_NT_HIGH_ATTACK_END); }
			if (isLand){ operatePartLandStart(); }
			break;
		case VJ_ATTACK_NT_HIGH_ATTACK_END:
			if (isLand){ operatePartLandStart(); }
			break;

		case FJ_ATTACK_NT_HIGH_ATTACK_START:
			if (isAnmEnd){ changeAnmNo(FJ_ATTACK_NT_HIGH_ATTACK_ON_1); }
			if (isLand){ operatePartLandStart(); }
			break;
		case FJ_ATTACK_NT_HIGH_ATTACK_ON_1:
			if (isAnmEnd){ changeAnmNo(FJ_ATTACK_NT_HIGH_ATTACK_OFF_1); }
			if (isLand){ operatePartLandStart(); }
			break;
		case FJ_ATTACK_NT_HIGH_ATTACK_OFF_1:
			if (isAnmEnd){ changeAnmNo(FJ_ATTACK_NT_HIGH_ATTACK_END); }
			if (isLand){ operatePartLandStart(); }
			break;
		case FJ_ATTACK_NT_HIGH_ATTACK_END:
			if (isLand){ operatePartLandStart(); }
			break;

		default:
			break;
	}
}

//----------------------------------------------------------
// 行動内の操作関数
void CharDots::operatePartAttackStart(){
	// 攻撃時の初期化
	operatePartAttackStartInit();

	// 攻撃開始コマンド
	operatePartStandAttackStart();
	operatePartCrouchAttackStart();
	operatePartAirAttackStart();
}

void CharDots::operatePartStandAttackStart(){
	if (!isLand)
		return;

	// ボタン１　= 弱攻撃
	if (battleInput.getBattleInputState(BattleInputManager::TRIGGER, BATTLE_BUTTON_1)){
		changeAnmNo(ST_ATTACK_NT_LOW_START);
		if (getCurDirState() == RIGHT){
			if (battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_RIGHT))
				changeAnmNo(ST_ATTACK_FW_LOW_START);
		}
		else if (getCurDirState() == LEFT){
			if (battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_LEFT))
				changeAnmNo(ST_ATTACK_FW_LOW_START);
		}
	}
	// ボタン2　= 強攻撃
	if (battleInput.getBattleInputState(BattleInputManager::TRIGGER, BATTLE_BUTTON_2)){
		changeAnmNo(ST_ATTACK_NT_HIGH_START);
		if (getCurDirState() == RIGHT){
			if (battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_RIGHT))
				changeAnmNo(ST_ATTACK_FW_HIGH_START);
		}
		else if (getCurDirState() == LEFT){
			if (battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_LEFT))
				changeAnmNo(ST_ATTACK_FW_HIGH_START);
		}
	}
	// Aボタン挑発(仮)
	if (battleInput.getBattleInputState(BattleInputManager::TRIGGER, BATTLE_BUTTON_A)){
		changeAnmNo(POSE_PROVOKE_1);
	}
}

void CharDots::operatePartCrouchAttackStart(){
	if (!isLand)
		return;
	// しゃがんでいない場合
	if (!battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_DOWN))
		return;


	// ボタン１　= 弱攻撃
	if (battleInput.getBattleInputState(BattleInputManager::TRIGGER, BATTLE_BUTTON_1)){
		changeAnmNo(CR_ATTACK_FW_LOW_START);
		if (getCurDirState() == RIGHT){
			if (battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_RIGHT))
				changeAnmNo(CR_ATTACK_FW_LOW_START);
		} else if (getCurDirState() == LEFT){
			if (battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_LEFT))
				changeAnmNo(CR_ATTACK_FW_LOW_START);
		}
	}

	// ボタン2　= 強攻撃
	if (battleInput.getBattleInputState(BattleInputManager::TRIGGER, BATTLE_BUTTON_2)){
		changeAnmNo(CR_ATTACK_NT_HIGH_START);
		if (getCurDirState() == RIGHT){
			if (battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_RIGHT))
				changeAnmNo(CR_ATTACK_FW_HIGH_START);
		} else if (getCurDirState() == LEFT){
			if (battleInput.getBattleInputState(BattleInputManager::HOLD, BATTLE_BUTTON_LEFT))
				changeAnmNo(CR_ATTACK_FW_HIGH_START);
		}
	}

	// Aボタン挑発(仮)
	if (battleInput.getBattleInputState(BattleInputManager::TRIGGER, BATTLE_BUTTON_A)){
		changeAnmNo(POSE_PROVOKE_1);
	}
}

void CharDots::operatePartAirAttackStart(){
	if (isLand)
		return;
	// 空中にいるとき

	int tmpCurActionState = getCurActionState();

	// ボタン１　= 弱攻撃
	if (battleInput.getBattleInputState(BattleInputManager::TRIGGER, BATTLE_BUTTON_1)){
		if (tmpCurActionState == JUMP_BACK)
			changeAnmNo(VJ_ATTACK_NT_LOW_ATTACK_START);
		else if (tmpCurActionState == JUMP_VIRTICAL)
			changeAnmNo(VJ_ATTACK_NT_LOW_ATTACK_START);
		else if (tmpCurActionState == JUMP_FORWARD)
			changeAnmNo(FJ_ATTACK_NT_LOW_ATTACK_START);
	}
	// ボタン2　= 強攻撃
	if (battleInput.getBattleInputState(BattleInputManager::TRIGGER, BATTLE_BUTTON_2)){
		if (tmpCurActionState == JUMP_BACK)
			changeAnmNo(VJ_ATTACK_NT_HIGH_ATTACK_START);
		else if (tmpCurActionState == JUMP_VIRTICAL)
			changeAnmNo(VJ_ATTACK_NT_HIGH_ATTACK_START);
		else if (tmpCurActionState == JUMP_FORWARD)
			changeAnmNo(FJ_ATTACK_NT_HIGH_ATTACK_START);
	}
}

