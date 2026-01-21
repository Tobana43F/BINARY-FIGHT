//==========================================================
// アーケードモード用のキャラクター選択画面
//  ・1pのみがキャラクターを選択するように変更
#ifndef _CHARACTER_SELECT_FOR_ARCADE_MODE
#define _CHARACTER_SELECT_FOR_ARCADE_MODE

//==========================================================
// インクルード
#include	"CharacterSelect.h"		// このクラスを継承するため

//==========================================================
// クラス定義
class CharacterSelectForArcadeMode : public CharacterSelect{
protected:


	void updateGameScene();

public:
	CharacterSelectForArcadeMode(BattleConfigInfo *_pBattleConfigInfo);
	virtual ~CharacterSelectForArcadeMode();

	void draw();
	void update();

};


#endif