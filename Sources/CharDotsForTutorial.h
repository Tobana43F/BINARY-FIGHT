//==========================================================
// チュートリアル用相手キャラ
#ifndef _CHARACTER_DOTS_FOR_TUTORIAL_H
#define _CHARACTER_DOTS_FOR_TUTORIAL_H

#include	"CharacterDots.h"
#include	"ModeTutorial.h"	// ポインタを持つため

class CharDotsForTutorial : public CharDots{
protected:
	ModeTutorial	*pModeTutorial;	// 参照要
	const int INPUT_RESET_CNT;

	// 制御用
	int cnt;
	int cntStart;

	// クラス内メソッド
	void updateCPU();

	void updateGuardHighPosStep();
	void updateGuardLowPosStep();
	void updateGuardMidPosStep();
	void updateBattle();


public:
	CharDotsForTutorial(ModeTutorial *_pModeTutorial);
	virtual ~CharDotsForTutorial();

	void operateActor();

};

#endif