//==========================================================
// 攻撃情報の詳細をもつクラス
#ifndef _ATTACK_INFO_H
#define _ATTACK_INFO_H

//==========================================================
// インクルード
#include	"Polygon.h"	// FCoord2Dを使うため

//==========================================================
// 定数定義
namespace attackInfoNS{
	enum eStrength{
		NOTHING_STRENGTH,		// 初期化用
		LOW_STRENGTH,			// 弱攻撃(弱ヒットバック)
		HIGH_STRENGTH,			// 強攻撃
	};

	enum ePosition{
		NOTHING_POS,			// 初期化用
		LOW_POS,				// 下段(しゃがみガードのみ防げる)
		HIGH_POS,				// 上段(しゃがみorたちガードで防げる)
		OVERHEAD_POS,			// 中段(たちガードのみで防げる)
		UNBLOCKABLE_POS,		// ガード不可能
	};

	enum eType{
		NOTHING,				// 初期化用
		NORMAL,					// 通常攻撃
		SPECIAL,				// 必殺技
	};

	// 使い方に注意
	enum eAttribute{			// ビット演算で使用する
		NO_ATTRIBUTE = 0,		// 通常通りの攻撃
		LAND_KNOCK_DOWN,		// 相手をダウンさせる
		AIR_KNOCK_DOWN,			// 空中の相手をダウンさせる（自由落下状態)(追撃は不可能)
		AIR_KNOCK_DOWN_JUGGLE,	// 空中の相手をダウンさせ、追撃可能状態にする
	};
}

using namespace attackInfoNS;
//==========================================================
// クラス定義
class AttackInfo{
public:
	//----------------------------------------------------------
	// 判定後に判定情報を入れる変数
	FCoord2D collideBox;	// 衝突が起こったボックスを求める場所

	//----------------------------------------------------------
	// 攻撃情報として持つ値(攻撃側)
	// ダメージの強さ
	eStrength strength;

	// ダメージの位置(○段とか)
	ePosition pos;

	// 攻撃種別
	eType type;

	eAttribute attribute;

	// ダメージ量
	int attackDamage;		// ヒット時のダメージ量
	int blockDamage;		// ガード時のダメージ量

	// ヒットストップ時間
	int attackHitStopCnt;	// ヒット時のヒットストップ時間(フレーム数)		(画面が更新されない時間)
	int blockHitStopCnt;	// ブロック時のヒットストップ時間(フレーム数)

	// 硬直時間
	int attackHitStunCnt;		// ヒット硬直時間(相手が動けない時間(この時間内に他の攻撃を当てると、コンボになる))
	int blockHitStunCnt;		// ガード硬直時間(連続ガードになる時間)

	// ヒットバック(のけぞり距離)
	float attackHitBack;	// ヒット時の被攻撃側のノックバック量(右向きじを基準に作成)
	float blockHitBack;		// ガード時の〃

	//----------------------------------------------------------
	// 関数
	void initData(){
		collideBox.x = 0;	// 適当に大きな値
		collideBox.y = 0;
		collideBox.w = 0;		// 点として初期化
		collideBox.h = 0;

		strength = NOTHING_STRENGTH;
		pos = NOTHING_POS;
		type = NOTHING;
		attribute = NO_ATTRIBUTE;
		attackDamage = 0;
		blockDamage = 0;
		attackHitStopCnt = 0;
		blockHitStopCnt = 0;
		attackHitStunCnt = 0;
		blockHitStunCnt = 0;
		attackHitBack = 0;
		blockHitBack = 0;
	}
};

#endif // !_ATTACK_INFO_H