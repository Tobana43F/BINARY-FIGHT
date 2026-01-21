//==========================================================
// プレイアブルキャラクター
#ifndef		_ACTOR_H
#define		_ACTOR_H

#include	"Constants.h"		// inputNSを使うため
#include	"Polygon.h"			// Polygon2Dを継承するため
#include	"BoundingBox.h"		// BoundingBoxをメンバに持つため
#include	"MyTexture.h"		// テクスチャラベルを使用するため
#include	"TextureLoader.h"	// テクスチャを使用するため

#include	"BattleInput.h"		// 入力管理

#include	"MyHitBox.h"		// ヒットボックスを読み込むため

//==========================================================
// 定数
//----------------------------------------------------------
// 当たり判定読込時の終端子
#define HITBOX_END		"NULL"

//==========================================================
// 構造体
// フロート型の2値を持つ構造体
struct F2Value{
	float x, y;
};

//==========================================================
// クラス(キャラを増やすなら、書くキャラごとに作る)
class Actor : public Polygon2D{
public:
	//==========================================================
	// 定数
	// 仮)何フレーム前の状態まで保存するか
	static const int BUFFER_MAX = 2;	// 行動状態などを記録するバッファーの数
	
	//----------------------------------------------------------
	// プレイヤーデータ
	Actor	*pOpponentActor;	// 敵への参照
	void	setOpponentActorPointer(Actor *_pActor);

	//----------------------------------------------------------
	// キャラクターのサイズ関係
	// 初期値(基準値:300前後 )(定数だが、プレイヤーごとに決めたいため、変数として持つ)
	float initActorWidth;	 
	float initActorHeight;

	// 実際に描画する大きさ
	float actorWidth;	
	float actorHeight;
	// 描画する大きさとテクスチャのサイズの比率(当たり判定の補正値として使う。)
	// テクスチャサイズが無いと初期化できないのでそれぞれのキャラクターのテクスチャ読込後に初期化を行う。
	float actorWidthRatio;
	float actorHeightRatio;

	// 全キャラ共通の行動時の定数
	const char JUMP_FRAME;// = 4;		// 空中判定になるまでのフレーム数

	// 仮)
	FCoord2D prevCoord[BUFFER_MAX];		// 仮)1フレーム前の座標

	//----------------------------------------------------------
	// 全キャラクターに共通する行動状態
	//  共通の状態で定義することで、外部からでも基本的なキャラクター操作が可能になる。
	enum eCommonActionState{
		// 基本行動
		ST_START,
		ST_COMPLETE,
		ST_REVERSE,

		CR_START,
		CR_COMPLETE,
		CR_REVERSE,

		WALK_FORWARD,
		WALK_BACK,

		JUMP_BACK_START,
		JUMP_VIRTICAL_START,
		JUMP_FORWARD_START,

		JUMP_BACK,
		JUMP_VIRTICAL,
		JUMP_FORWARD,

		LAND,

		// ダメージ関係
		ST_GUARD,
		ST_GUARD_END,
		ST_LOW_DAMAGE,
		ST_HIGH_DAMAGE,
		ST_RECOVER,

		CR_GUARD,
		CR_GUARD_END,
		CR_LOW_DAMAGE,
		CR_HIGH_DAMAGE,
		CR_RECOVER,

		AIR_DAMAGE,
		AIR_RECOVER,

		LAND_KNOCK_DOWN,
		AIR_KNOCK_DOWN,
		KNOCK_DOWN_COMPLETE,
		KNOCK_DOWN_RECOVER,

		KO_START,
		KO_COMPLETE,
		KO_END,

		// ポーズや挑発などの行動
		POSE_ROUND_WIN_1,
		POSE_BATTLE_WIN_1,

		POSE_PROVOKE_1,

		// 攻撃関係(攻撃関係は、キャラごとに個別に設定しなければ生けない)
		//ST_ATTACK_NT_LOW_START,
		//ST_ATTACK_FW_LOW_START,
		//CR_ATTACK_NT_LOW_START,
		//VJ_ATTACK_NT_LOW_START,
		//FJ_ATTACK_NT_LOW_START,

		//ST_ATTACK_NT_HIGH_START,
		//ST_ATTACK_FW_HIGH_START,
		//CR_ATTACK_NT_HIGH_START,
		//VJ_ATTACK_NT_HIGH_START,
		//FJ_ATTACK_NT_HIGH_START,

		MAX_COMMON_ACTION_STATE,
	};
	int actionState[BUFFER_MAX];	// Common,Uniqueの行動状態が入る

	//----------------------------------------------------------
	// 変数
	BattleInputManager	battleInput;

	// キャラクターのステータス
	int		maxHitPoint;	// 最大体力
	int		curHitPoint;	// 体力量

	// 移動速度(定数だが、上位クラスで使用したいので、下位クラスで定数を代入してつかう)
	F2Value maxSpeed;		// 廃止予定…セッターも一緒に消すこと)歩きのスピードとして使っている。。。

	float	speedJumpY;
	float	speedJumpX;

	float	speedWalkForward;
	float	speedWalkBack;


	// 実際の移動速度
	F2Value	curSpeed;		// 現在のスピードを格納する(右向きを基準にする)

	// キャラクター左右の状態の定義
	// dirState : 向いている方向の状態
	// posState : プレイヤー同士の相対的な位置の状態
	enum eDirectionState{
		RIGHT = 0,			// 左側(posState = LEFT)にいるときは右側(dirState = RIGHT)を向いている。
		LEFT,
	} 
	dirState[BUFFER_MAX], posState[BUFFER_MAX];

	// ガード状態
	enum eGuardState{
		NO_GUARD = 0,		// ガードなし(何もガードできない、ダメージ時にはこれになる。)
		STAND_GUARD,		// 立ちガード(上段、中段)
		CROUCH_GUARD,		// しゃがみガード(上段、下段)
	}guardState;

	bool				isStageLimitX;								// キャラクターの画面端判定用

	bool				isKO;										// KOしているかどうか(true:KO(先頭不能状態)　false:先頭可能(生きてる))

	bool				isLand;										// 地上にいるか、空中にいるかのフラグ(true:地上　false:空中)
	bool				isLanding;									// 着地モーション中
	bool				isLandCancelable;							// 着地モーションをキャンセルできるかどうか(ジャンプ後…攻撃を出していない、攻撃があったった場合は、キャンセル可能)

	bool				flgTriggerChangePosState;					// 位置関係が変わったときのトリガーフラグ。(とりあえず残しておく)
	bool				flgChangePosState;							// 位置関係が変わったときのホールドフラグ(向きを変えるまで、true状態は残る)
	bool				isReversing;								// 振り向き中の場合


	bool				flgTriggerChangeAnm;						// アニメーション変更トリガーフラグ
	bool				flgTriggerChangeAction;						// アクションを変更した時のトリガーフラグ

	//==========================================================
	// 攻撃時の制御
	bool	isAttackAction;							// 攻撃行動をとった。(あたったかどうかは関係ない)
	bool	isAttackBoxEnable;						// 攻撃判定が有効かどうか(一連の攻撃の攻撃判定を無効化するときに使用する)(trueのときは攻撃を判定しなくなる。)
	bool	isAttackHit;							// 攻撃があたったかどうか(一連の攻撃の場合、どれか一つでもあたればtrue)
	bool	isAttackBlocked;						// 攻撃がガードされたフラグ(falseなら攻撃があたっている)…注意)他の変数と組み合わせて使用すること
	int		cntCombo;								// コンボのカウンタ

	//----------------------------------------------------------
	// 攻撃を喰らったときの情報を格納する(リストのほうがいいかも)
	static const int	RECEIVE_ATTACK_MAX = 10;					// 同時に喰らう攻撃の最大数(forループ用)
	AttackInfo			receivedAttackInfo[RECEIVE_ATTACK_MAX];		// 喰らった攻撃情報を格納 
	int					receivedAttackNum;							// 同時に2個以上の攻撃を喰らった場合に処理できるように(飛び道具と攻撃など)(攻撃を受けたフラグとしても使える)

	bool				isBlock;									// 攻撃をガードしたフラグ
	bool				isJuggle;									// 空中追撃可能状態(true:追撃可能　false:不可能(基本はfalse))(空中のときにしか使用しない)
	bool				isCrossUpReceived;							// めくり攻撃を受けたフラグ(true:めくり　false:正面)
	bool				isCrossUpHitStun;							// めくり攻撃のヒット硬直の間のフラグ(向きを変えるフラグとして使用する)

	// 攻撃を受けたフラグ(トリガー)が必要(効果音を鳴らすとき)
	int					hitDamage;				// 受けたダメージ
	int					attackHitStunCnt;		// ヒット硬直時間
	float				hitBack;				// ヒットバックの距離を代入

	bool				isCollideGuardBox;		// (相手の)ガードボックスと衝突があった場合(true:衝突　false:なし(基本はfalse)) 

	//----------------------------------------------------------
	// 当たり判定を持つポインタ配列(汎用的な当たり判定をHitBoxという)
	//----------------------------------------------------------
	// 当り判定リストの先頭はダミーデータ
	WallBoundingBox			*wallBox;				// 壁との当たり判定(一次元配列で持つ)	
	BodyBoundingBoxList		**bodyBoxList;			// 体判定(体同士がぶつかる判定に使用する)
	HurtBoundingBoxList		**hurtBoxList;			// 喰らい判定
	AttackBoundingBoxList	**attackBoxList;		// 攻撃判定
	GuardBoundingBoxList	**guardBoxList;			// ガード判定
													// 投げ判定
	
	//==========================================================
	// テクスチャ関係
	enum eTexListForGameReady{
		VS_TEX,			// １枚のみで固定
		MAX_VS_TEX,
	};
	TextureLoader	*pTextureLoaderForGameReady;	// ゲームレディーの時に使用するテクスチャのみ読み込む。(その後は開放してもよい)

	TextureLoader	*pTextureLoader;	// キャラクターのテクスチャを読み込む場所
	int				curTexNo;			// 現在使用しているテクスチャポインタ配列の添え字

	void changeUseTexture(int no);		// 使用するテクスチャの変更

	void setCoordFromTextureDivRatio();		// テクスチャサイズに合わせたキャラクターポリゴンの座標(BOX)を設定

	void setWidthRatio();			// サイズの比率を取得
	void setHeightRatio();			// サイズの比率を取得


	//==========================================================
	// 関数
	Actor();
	virtual ~Actor();
	//----------------------------------------------------------
	// 初期化
	void init();			// Actor共通の初期化

	//----------------------------------------------------------
	// セッター系
	void setMaxHitPointData(int maxHp);				// 最大体力を設定
	void setMaxSpeedData(float x, float y);			// 廃止予定)最大スピードを設定

	void setPrevCoord();		// 仮)

	void setCurSpeed(float _speedX, float _speedY);	// 向きに関わらない、速度を設定

	void setCoord(float x, float y);				// 座標を設定する処理
	void addCoordWithPosState(float x, float y);				// 位置関係で判断し、座標に加算するときに使用する。
	void addCoordWithDirState(float x, float y);	// 向いている方向で判断し、座標に加算する
	void moveCoord();								// 移動量を座標を反映(移動)させる処理

	void setDirState(eDirectionState val);			// プレイヤーの向きを設定
	void setPosState(eDirectionState val);			// プレイヤーの位置(左側、右側)を設定

	void checkPosState();							// posStateをチェックし、位置関係のフラグを変更

	void changeDirState();							// posStateによりdirStateを変更する。
	void changePolyReverseXState();					// dirStateによりisReverseXの値を変える

	void changeActionState(int _state);				// actionStateを変更する

	void checkGuardState();							// ガード状態を調べ、値をセットする。

	void checkLandingState();						// 着地中かどうかを、判断し、着地に関する変数を設定する。

	//----------------------------------------------------------
	// ゲッター系
	void calcAbsoluteCoord(FCoord2D *dest, const FCoord2D *srcHitBox);		// 当り判定座標のワールド座標を求める

	virtual int		getCurActionState();		// virtual である必要はないが、変更が楽なため付けておく
	virtual int*	getActionStateArray();	
	eDirectionState getCurDirState();				// 現在の方向を取得
	eDirectionState getCurPosState();				// 現在の位置情報を取得

	eDirectionState getCurSpeedDirection();			// 現在のスピードが向いている方向を取得

	eGuardState getCurGuardState();					// ガード状態の取得
	
	//----------------------------------------------------------
	// 描画関係
	void drawWallBox();
	void drawBodyBox();
	void drawHurtBox();
	void drawAttackBox();
	void drawGuardBox();
	
	//----------------------------------------------------------
	// 当たり判定関係
	// 当たり判定情報を格納するリストの先頭アドレスの配列を確保する関数(MAX_STATE個確保する)
	// 変更あるかも)MAX_STATEを引数として、渡せば、仮想関数にする必要が無くなる。
	void allocHitBoxListArray(int _maxTex, int _maxState);
	void freeHitBoxListArray();

	// 当たり判定情報を設定する関数。
	void setWallBoxData(int _maxTex, hitBoxNS::eHitBoxLabel _label);	// テクスチャの枚数分だけ、HitBoxを持つ
	void setBodyBoxData(int _maxState, hitBoxNS::eHitBoxLabel _label);
	void setHurtBoxData(int _maxState, hitBoxNS::eHitBoxLabel _label);
	void setAttackBoxData(int _maxState, hitBoxNS::eHitBoxLabel _label);
	void setGuardBoxData(int _maxState, hitBoxNS::eHitBoxLabel _label);

	// 当たり判定情報の開放
	// wallBoxは実態を変数として宣言しているので開放は要らない
	void freeBodyBoxList(int _maxState);				// 体判定
	void freeHurtBoxList(int _maxState);
	void freeAttackBoxList(int _maxState);
	void freeGuardBoxList(int _maxState);

	//==========================================================
	// Battleクラスから呼び出す関数
	// 当り判定関係
	void initHurtBoxData();			// 当り判定初期化
	void initAttackBoxData();

	// 被攻撃時応答
	void reactReceiveAttack();		// 攻撃を受けたときの応答
	void reactReceiveGuardBox();	// ガードBoxとしょうとつがあったときの処理

	//==========================================================
	// 純粋仮想関数
	//----------------------------------------------------------
	// 基本的な関数
	virtual void initCharacter() PURE_FUNC;
	virtual void load() PURE_FUNC;		// テクスチャのロード、当たり判定のロードなどを行う

	//----------------------------------------------------------
	// セッター系
	virtual void	changeAnmNo(int _anmNo) PURE_FUNC;				// テクスチャナンバーの変更()
	virtual void	changeIsAttackAction() PURE_FUNC;				// 攻撃行動フラグ(攻撃行動のときにisAttackActionをtrueにする関数)

	//----------------------------------------------------------
	// その他
	// 操作系
	virtual void operateActor();		// キャラクターの状態ごとの操作を記述 , 基本的な操作(キャラで固有の行動をするときは、オーバーライドする)　

protected:
	//----------------------------------------------------------
	// 行動ごとの関数(必須のものが書かれている)
	// 変更しない可能性が高いもの
	virtual void operateActorST_START();
	virtual void operateActorCR_START();

	virtual void operateActorST_REVERSE();
	virtual void operateActorCR_REVERSE();

	// 変更するかも
	virtual void operateActorST_COMPLETE();	
	virtual void operateActorCR_COMPLETE();

	virtual void operateActorWALK_FORWARD();
	virtual void operateActorWALK_BACK();

	virtual void operateActorJUMP_BACK_START();
	virtual void operateActorJUMP_VIRTICAL_START();
	virtual void operateActorJUMP_FORWARD_START();

	virtual void operateActorJUMP_BACK();
	virtual void operateActorJUMP_VIRTICAL();
	virtual void operateActorJUMP_FORWARD();

	virtual void operateActorLAND();

	virtual void operateActorST_GUARD();
	virtual void operateActorST_GUARD_END();

	virtual void operateActorCR_GUARD();
	virtual void operateActorCR_GUARD_END();

	virtual void operateActorST_DAMAGE();	// 強弱共通
	virtual void operateActorCR_DAMAGE();	// 強弱共通
	virtual void operateActorAIR_DAMAGE();

	virtual void operateActorST_RECOVER();
	virtual void operateActorCR_RECOVER();
	virtual void operateActorAIR_RECOVER();

	virtual void operateActorLAND_KNOCK_DOWN();
	virtual void operateActorAIR_KNOCK_DOWN();

	virtual void operateActorKNOCK_DOWN_COMPLETE();
	virtual void operateActorKNOCK_DOWN_RECOVER();

	virtual void operateActorKO_START();
	virtual void operateActorKO_COMPLETE();

	virtual void operatePOSE();	// ポーズ全般

	//----------------------------------------------------------
	// 行動内の操作関数
	virtual void operatePartStandStart();
	virtual void operatePartCrouchStart();

	virtual void operatePartStandReverseStart();
	virtual void operatePartCrouchReverseStart();

	virtual void operatePartStandStop();	// 停止後たち状態に戻る

	virtual void operatePartWalkStart();
	virtual void operatePartWalkForwardStart();	// 前歩きのみ
	virtual void operatePartWalkBackStart();

	virtual void operatePartJumpStart();

	virtual void operatePartLandStart();

	virtual void operatePartAirDamageStart();

	//----------------------------------------------------------
	// 継承クラスで必須の項目を忘れないために
	void operatePartAttackStartInit();					// 攻撃時の初期化処理(共通)(継承後のクラスのAttackStartの最初に呼び出す必要がある)
	virtual void operatePartAttackStart() PURE_FUNC;	// 継承クラスでの固有の動作を記述

public:
	//----------------------------------------------------------
	// バトルから呼び出す行動変更処理
	virtual void operatePartReverseCancel();
	virtual void operatePartLandCancelWidthAttackReverse();	// 着地を攻撃でキャンセルした場合の振り向き処理(強制振り向き)

};

#endif	_ACTOR_H