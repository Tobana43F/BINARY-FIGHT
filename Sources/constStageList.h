//==========================================================
// 全ステージのリスト
//  ステージを指定する際に使用する定数
#ifndef _CONST_STAGE_LIST_H
#define _CONST_STAGE_LIST_H

enum eStageList{
	STAGE_LIST_SKY_PLACE,
	STAGE_LIST_RANDOM,		// ランダム(これより大きいものは、ランダムで選ばれることは無くなる)
	STAGE_LIST_TRAINING,
	MAX_STAGE_LIST,
};

#endif