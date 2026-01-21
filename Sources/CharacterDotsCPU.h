//==========================================================
// キャラクターのCPU用クラス
#ifndef _CHARACTER_DOTS_CPU_H
#define _CHARACTER_DOTS_CPU_H

//==========================================================
// インクルード
#include	"CharacterDots.h"	// 継承するため

//==========================================================
// クラス定義
class CharDotsCPU : public CharDots{
protected:
	// 制御用
	int  cnt;
	int  updateInterval;		// 更新スピード

	// 歩きの更新のゆれ
	int cntWalkNoise;
	bool isNear;	// flase: far

	// 後退のゆれ
	int cntBackNoise;

	// 攻撃の確率
	int perAttackFarNoise;

	// 近距離でのしゃがみ確率
	int perCrouchNear;
	int cntCrouchNearNoise;

	// ジャンプ確率
	int perVJump;
	int cntVJumpNoise;

	int cntFJumpNoise;

	// ガード確率
	int perGuard;
	
	// ガード時間
	int cntGuardLength;
	int cntGuard;
	bool isKeepBlock;

	void updateCPU();	// CPUの行動の更新

public:
	CharDotsCPU();
	virtual ~CharDotsCPU();

	void operateActor();	// オーバーライド
};

#endif