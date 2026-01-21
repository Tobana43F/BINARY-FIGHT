//==========================================================
// 戦闘時の処理
#ifndef _BATTLE_H
#define	_BATTLE_H

//==========================================================
// インクルード
#include	"Actor.h"					// Actorクラスをメンバに持つため
#include	"CollisionBoundingBox.h"	// 当り判定用クラスをメンバに持つため
#include	"BattleUI.h"				// BattleUIクラスをメンバに持つため
#include	"AbstractStage.h"			// ステージを持つため
#include	"BattleEffect.h"			// エフェクト
#include	"CharacterShadow.h"			// キャラの影
#include	"RoundEffect.h"				// ラウンドコール

#include	"BattleConfigInfo.h"		// BattleConfigInfo
#include	"GameReady.h"				// この情報を元に、初期化する


//==========================================================
// クラス定義
class Battle{
private:
	//==========================================================
	// 定数
	//----------------------------------------------------------
	// バトル内での遷移
	enum eBattleScene{
		INIT_ONCE,					// 最初の一度のみ
		INIT,						// バトルの初期化
		ROUND_START,				// ラウンドの開始(ラウンド開始コール時)
		BATTLE_PLAY,				// バトル中
		PLAYER_KO,					// プレイヤーがKO(KOされて、着地する前。(着地が完了すると、RoundEndへ))B
		ROUND_END,					// ラウンド終了後(KO着地後・TimeUPは直接ここにくる。)
		BATTLE_END,					// 全ラウンド、バトルが終了
		END,
		MAX_BATTLE_SCENE,
	} battleScene;
	
	// 制御用
	bool	isSceneEnd;
	int		cnt;			// バトル開始からのカウンタ(コンストラクタで初期化)

	// 画面遷移の更新(プログラム全体の)
	bool	updateGameScene();	

	// 描画関数
	void drawGameScreen();				// ゲーム画面全体を表示
	void drawHitBox();

	// 更新処理
	void	updateBattlePlay();			// 戦闘処理を更新(基本)
	void	updateBattleAfterKO();		// KO後の更新処理(操作可能+体力からの減算処理がない)
	void	updateRoundEnd();			// ラウンド終了時の更新(キャラの勝利演出？)
	void	updateGameScreen();			// 戦闘状態でないときの更新(操作不可)

	// デバッグ用
	void	drawDebugInfo();			// デバッグ時の情報を表示

public:
	//----------------------------------------------------------
	// バトル情報
	BattleConfigInfo	*pBattleConfigInfo;

	//----------------------------------------------------------
	// プレイヤー関係
	static const char PLAYER_NUM = 2;
	static const char PLAYER_CHANGER = 1;	// これとXOR(~)することで、交代できる
	Actor	*player[PLAYER_NUM];	// プレイヤーのポインタ配列

	CollisionBoundingBox collisionDetector;	// 当り判定を判定するクラス
	int		hitStopCnt;		// 攻撃が成立したときにヒットストップとして代入する。

	//----------------------------------------------------------
	// ステージ関係
	AbstractStage	*pStage;	// ステージのポインタ

	//----------------------------------------------------------
	// 影
	CharacterShadow	*pCharacterShadow;

	//----------------------------------------------------------
	// UI関係
	TimerUI			*pTimerUI;			// タイマー	
	HitPointBarUI	*pHitPointBarUI;	// HPバー(HP情報は、Actorクラスへのポインタを持たせる)
	RoundIconUI		*pRoundIconUI;		// ラウンドアイコン
	ComboUI			*pComboUI;			// コンボ

	BattleEffect	*pBattleEffect;		// エフェクト
	
	RoundStartEffect	*pRoundStartEffect;

	RoundKOEffect	*pRoundKOEffect;

	RoundEndEffect	*pRoundEndEffect;

	BattleEndEffect		*pBattleEndEffect;

	//==========================================================
	// 関数
	Battle(GameReady *_pGameReady);
	virtual ~Battle();

	// セッター系
	void initBattlePlayer(Actor *_player1, Actor *_player2);	// 対戦する二人のポインタを渡す

	// ゲッター
	bool getIsSceneEnd(){ return isSceneEnd; }

	// 判定系
	void checkCollisionBodyBox(Actor *_player1, Actor *_player2);	// 体判定のチェック(めり込み処理もする。)
	void checkCollisionAttackBox(Actor *defense, Actor *offense);	// 攻撃判定のチェック(チェック以外のことも行っているので、分けたほうが良いかもしれない)
	void checkCollisionGuardBox(Actor *defense, Actor *offense);

	void checkCollisionScreenLimit(Actor *_player);	// プレイヤーと表示画面の端判定。

	void checkPlayerPosition();					// 2プレイヤー間の位置関係を判定

	// 基本的な関数
	void init();	// 初期化
	void draw();	// 描画用関数(render内で呼び出す)
	void update();	// 行進用関数(update内で呼び出す)
};

#endif