//==========================================================
// チュートリアルの達成項目クラス(ステップ内容)
#ifndef _TUTORIAL_STEP_H
#define _TUTORIAL_STEP_H

#include	"Battle.h"
#include	"Constants.h"		// PURE_FUNC
#include	"TextureLoader.h"

//==========================================================
// 達成項目の抽象クラス
class AbstractTutorialStep {
public:
	bool	isStepCleard;	// ステップクリアフラグ

	Battle	*pBattle;		// 参照用(全ての情報が詰まっているので、これから参照していろいろな情報を得る)

	TextureLoader	*pTextureLoader;

	// ポリゴン
	const float STEP_TITLE_WIDTH;
	const float STEP_TITLE_HEIGHT;
	const float STEP_TITLE_X;
	const float STEP_TITLE_Y;

	const float STEP_DESCRIPTION_WIDTH;
	const float STEP_DESCRIPTION_HEIGHT;
	const float STEP_DESCRIPTION_X;
	const float STEP_DESCRIPTION_Y;

	const float STEP_INSTRACTION_WIDTH;
	const float STEP_INSTRACTION_HEIGHT;
	const float STEP_INSTRACTION_X;
	const float STEP_INSTRACTION_Y;

	Polygon2D	polyStepTitle;
	Polygon2D	polyStepDescription;
	Polygon2D	*pPolyInstractString;	// 配列確保先ポインタ(達成項目数分だけ確保する)

	// 達成項目数(下位クラスからもらう)
	int instructionNum;	

	// 文字色
	const int STRING_COL_NOT_CLEARD_R;	// クリアしていないときの色
	const int STRING_COL_NOT_CLEARD_G;
	const int STRING_COL_NOT_CLEARD_B;

	const int STRING_COL_CLEARD_R;	// クリア後の色
	const int STRING_COL_CLEARD_G;
	const int STRING_COL_CLEARD_B;

	//==========================================================
	// メソッド
	AbstractTutorialStep();
	virtual ~AbstractTutorialStep();

	// ゲッター
	bool getIsStepCleard(){ return isStepCleard; }

	// セッター
	void setBattlePointer(Battle *_pBattle);	// 参照するバトルへのポインタを設定
	void load();	// 共通の処理
	void draw();

	// 純粋仮想関数
	virtual void init() PURE_FUNC;
	
	virtual void update() PURE_FUNC;

	virtual void checkClear() PURE_FUNC;	// ステップクリア条件の検査
};

//==========================================================
// 移動の練習用
class TutorialStepMove : public AbstractTutorialStep {
public:
	// 制御用
	bool isWalkForwardOK;	// 前歩きをした
	bool isWalkBackOK;		// 後ろ歩きをした

	// 達成項目定数
	enum eInstraction{
		WALK_FORWARD,
		WALK_BACK,
		MAX_INSTRACTION
	};

	// テクスチャ
	enum eTexList{
		STEP_TITLE,
		STEP_DESCRIPTION,
		INSTRACT_WALK_FORWARD,
		INSTRACT_WALK_BACK,
		MAX_TEX,
	};


	//==========================================================
	// メソッド
	TutorialStepMove();
	virtual ~TutorialStepMove();

	void init();

	void update();

	void checkClear();
};

//==========================================================
// ジャンプの練習
class TutorialStepJump : public AbstractTutorialStep {
public:
	// 制御用
	bool isJumpVirtical;	// 垂直ジャンプをした
	bool isJumpBack;		// 後ろジャンプをした
	bool isJumpForward;		// 前ジャンプをした

	// 達成項目定数
	enum eInstraction{
		JUMP_VIRTICAL,
		JUMP_BACK,
		JUMP_FORWARD,
		MAX_INSTRACTION
	};

	// テクスチャ
	enum eTexList{
		STEP_TITLE,
		STEP_DESCRIPTION,
		INSTRACT_JUMP_VIRTICAL,
		INSTRACT_JUMP_BACK,
		INSTRACT_JUMP_FORWARD,
		MAX_TEX,
	};


	//==========================================================
	// メソッド
	TutorialStepJump();
	virtual ~TutorialStepJump();

	void init();

	void update();

	void checkClear();
};

//==========================================================
// 攻撃の練習
class TutorialStepAttack : public AbstractTutorialStep {
public:
	// 制御用
	bool isAttackSomeTime;		// 垂直ジャンプをした

	// 達成項目定数
	enum eInstraction{
		ATTACK_SOME_TIME,
		MAX_INSTRACTION
	};

	// テクスチャ
	enum eTexList{
		STEP_TITLE,
		STEP_DESCRIPTION,
		INSTRACT_ATTACK,
		MAX_TEX,
	};

	bool prevIsAttackHit;

	const int CLEAR_ATTACK_QUOTA;
	int cntAttack;

	//==========================================================
	// メソッド
	TutorialStepAttack();
	virtual ~TutorialStepAttack();

	void init();

	void update();

	void checkClear();
};

//==========================================================
// 上段ガードの練習
class TutorialStepGuardHighPos : public AbstractTutorialStep {
public:
	// 制御用
	bool isGuardSomeTime;		// 垂直ジャンプをした

	// 達成項目定数
	enum eInstraction{
		GUARD_SOMETIME,
		MAX_INSTRACTION
	};

	// テクスチャ
	enum eTexList{
		STEP_TITLE,
		STEP_DESCRIPTION,
		INSTRACT_GUARD,
		MAX_TEX,
	};

	const int CLEAR_GUARD_QUOTA;
	int cntGuard;
	bool prevIsBlock;

	//==========================================================
	// メソッド
	TutorialStepGuardHighPos();
	virtual ~TutorialStepGuardHighPos();

	void init();

	void update();

	void checkClear();
};

//==========================================================
// 下段ガードの練習
class TutorialStepGuardLowPos : public AbstractTutorialStep {
public:
	// 制御用
	bool isGuardSomeTime;		// 垂直ジャンプをした

	// 達成項目定数
	enum eInstraction{
		GUARD_SOMETIME,
		MAX_INSTRACTION
	};

	// テクスチャ
	enum eTexList{
		STEP_TITLE,
		STEP_DESCRIPTION,
		INSTRACT_GUARD,
		MAX_TEX,
	};

	const int CLEAR_GUARD_QUOTA;
	int cntGuard;
	bool prevIsBlock;

	//==========================================================
	// メソッド
	TutorialStepGuardLowPos();
	virtual ~TutorialStepGuardLowPos();

	void init();

	void update();

	void checkClear();
};

//==========================================================
// 下段ガードの練習
class TutorialStepGuardMidPos : public AbstractTutorialStep {
public:
	// 制御用
	bool isGuardSomeTime;		// 垂直ジャンプをした

	// 達成項目定数
	enum eInstraction{
		GUARD_SOMETIME,
		MAX_INSTRACTION
	};

	// テクスチャ
	enum eTexList{
		STEP_TITLE,
		STEP_DESCRIPTION,
		INSTRACT_GUARD,
		MAX_TEX,
	};

	const int CLEAR_GUARD_QUOTA;
	int cntGuard;
	bool prevIsBlock;

	//==========================================================
	// メソッド
	TutorialStepGuardMidPos();
	virtual ~TutorialStepGuardMidPos();

	void init();

	void update();

	void checkClear();
};

//==========================================================
// 敵をKOしよう！
class TutorialStepBattle : public AbstractTutorialStep {
public:
	// 制御用
	bool isOpponentKO;		// 垂直ジャンプをした

	// 達成項目定数
	enum eInstraction{
		KO,
		MAX_INSTRACTION
	};

	// テクスチャ
	enum eTexList{
		STEP_TITLE,
		STEP_DESCRIPTION,
		INSTRACT_KO,
		MAX_TEX,
	};

	//==========================================================
	// メソッド
	TutorialStepBattle();
	virtual ~TutorialStepBattle();

	void init();

	void update();

	void checkClear();
};

#endif