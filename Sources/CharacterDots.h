//==========================================================
// キャラ　Dots(オリジナル)
// キャラクタ作成、第一号　標準
#ifndef _CHARACTER_DOTS_H
#define _CHARACTER_DOTS_H

//==========================================================
// インクルード
#include	"Actor.h"

//==========================================================
// クラス定義
class CharDots : public Actor{
public:
	//==========================================================
	// 定数
	const int	MAX_HP;
	const float SPEED_WALK_FORWARD;
	const float SPEED_WALK_BACK;

	const float SPEED_JUMP_Y;
	const float SPEED_JUMP_X;	// ジャンプ時の横方向の速度

	//==========================================================
	// 行動状態
	// 攻撃や、キャラクター特有の動きは、個別に定義する
	enum eUniqueActionState{
		ST_ATTACK_NT_LOW_START = MAX_COMMON_ACTION_STATE,
		ST_ATTACK_NT_LOW_ATTACK_ON_1,		// 名前がおかしい(ATTACK ATTACK)
		ST_ATTACK_NT_LOW_ATTACK_OFF_1,
		ST_ATTACK_NT_LOW_ATTACK_END,

		ST_ATTACK_FW_LOW_START,
		ST_ATTACK_FW_LOW_ATTACK_ON_1,
		ST_ATTACK_FW_LOW_ATTACK_OFF_1,
		ST_ATTACK_FW_LOW_ATTACK_END,

		CR_ATTACK_NT_LOW_START,
		CR_ATTACK_NT_LOW_ATTACK_ON_1,
		CR_ATTACK_NT_LOW_ATTACK_OFF_1,
		CR_ATTACK_NT_LOW_ATTACK_END,

		// CR_ATTACK_FW_LOW
		CR_ATTACK_FW_LOW_START,
		CR_ATTACK_FW_LOW_ON_1,
		CR_ATTACK_FW_LOW_OFF_1,
		CR_ATTACK_FW_LOW_END,

		VJ_ATTACK_NT_LOW_ATTACK_START,
		VJ_ATTACK_NT_LOW_ATTACK_ON_1,
		VJ_ATTACK_NT_LOW_ATTACK_OFF_1,
		VJ_ATTACK_NT_LOW_ATTACK_END,

		FJ_ATTACK_NT_LOW_ATTACK_START,
		FJ_ATTACK_NT_LOW_ATTACK_ON_1,
		FJ_ATTACK_NT_LOW_ATTACK_OFF_1,
		FJ_ATTACK_NT_LOW_ATTACK_END,

		ST_ATTACK_NT_HIGH_START,
		ST_ATTACK_NT_HIGH_ATTACK_ON_1,
		ST_ATTACK_NT_HIGH_ATTACK_OFF_1,
		ST_ATTACK_NT_HIGH_ATTACK_END,

		ST_ATTACK_FW_HIGH_START,
		ST_ATTACK_FW_HIGH_ATTACK_ON_1,
		ST_ATTACK_FW_HIGH_ATTACK_OFF_1,
		ST_ATTACK_FW_HIGH_ATTACK_END,

		CR_ATTACK_NT_HIGH_START,
		CR_ATTACK_NT_HIGH_ATTACK_ON_1,
		CR_ATTACK_NT_HIGH_ATTACK_OFF_1,
		CR_ATTACK_NT_HIGH_ATTACK_END,

		// CR_ATTACK_FW_
		CR_ATTACK_FW_HIGH_START,
		CR_ATTACK_FW_HIGH_ATTACK_ON_1,
		CR_ATTACK_FW_HIGH_ATTACK_OFF_1,
		CR_ATTACK_FW_HIGH_ATTACK_END,

		VJ_ATTACK_NT_HIGH_ATTACK_START,
		VJ_ATTACK_NT_HIGH_ATTACK_ON_1,
		VJ_ATTACK_NT_HIGH_ATTACK_OFF_1,
		VJ_ATTACK_NT_HIGH_ATTACK_END,

		FJ_ATTACK_NT_HIGH_ATTACK_START,
		FJ_ATTACK_NT_HIGH_ATTACK_ON_1,
		FJ_ATTACK_NT_HIGH_ATTACK_OFF_1,
		FJ_ATTACK_NT_HIGH_ATTACK_END,

		MAX_STATE,
	};

	//==========================================================
	// テクスチャ関係
	enum eTexList{
		NORMAL_TEX,
		DAMAGE_TEX,
		ATTACK_NORMAL_TEX,
		POSE_TEX,
		MAX_TEX,
	};

	//==========================================================
	// 関数
	CharDots();
	virtual ~CharDots();

	//==========================================================
	// Actorの関数をオーバーライドした関数
	// 初期化
	void initCharacter();
	void load();

	//----------------------------------------------------------
	// セッター系
	void	changeAnmNo(int _anmNo);						// オーバーライド(ActionStateが別テクスチャの場合一意に特定でいないので、その調整も行う)
	void	changeIsAttackAction();

	// その他
	// 操作系
	virtual void	operateActor();							// (オーバーライド)キャラクターの状態ごとの操作を記述(CPU用クラスでオーバーライド)

	//----------------------------------------------------------
	// 行動ごとの関数(必須のものが書かれている)


	//----------------------------------------------------------
	// 行動内の操作関数
	// 攻撃系
	void operatePartAttackStart();	// (オーバーライド) 全ての攻撃開始操作関数を包含する

	void operatePartStandAttackStart();	// 攻撃の開始操作をそれぞれ記述
	void operatePartCrouchAttackStart();
	void operatePartAirAttackStart();

};


#endif