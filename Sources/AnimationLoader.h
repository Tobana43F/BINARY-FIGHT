//==========================================================
// アニメーションをロードするクラス
#ifndef _ANIMATION_LOADER
#define _ANIMATION_LOADER

#include	"MyAnimation.h"

//==========================================================
// 1つのパターンを読み込むリスト
class PatternLoader{
public:
	int				anmNo;
	PatternLoader	*next;
};

//==========================================================
// 1つのアニメーションを一時的に読み込むリスト
class AnimationLoader{
public:
	PatternLoader	*anmPat;
	AnimationLoader	*next;
};


#endif