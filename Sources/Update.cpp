//==========================================================
// 描画の更新処理

#include	"Common.h"				// グローバル変数を扱うため

#include	"ShareBaseFunc.h"		// updateのプロトタイプ宣言
#include	"Constants.h"			// ゲームシーンの定数
#include	"MySound.h"

#include	"Battle.h"				// バトルクラスの確保
#include	"BattleConfigInfo.h"	// new

// テスト
#include	<stdio.h>

//==========================================================
// 更新処理を行う関数
void update(){
	switch (gGameScene){
		case TITLE_SCENE:{		// ↓---- 画面がある遷移
			pTitle->update();
			break;
		}			
		case MODE_SELECT:{
			pModeSelect->update();
			if (pModeSelect->getIsSceneEnd())
				safeDelete(pModeSelect);
			break;
		}
		case GS_ARCADE_MODE:
			pModeArcade->update();
			if (pModeArcade->getIsSceneEnd())
				gGameScene = RETURN_TO_MODE_SELECT;
			break;

		case GS_VERSUS_MODE:
			pModeVersus->update();
			if (pModeVersus->getIsSceneEnd())
				gGameScene = RETURN_TO_MODE_SELECT;
			break;

		case GS_TUTORIAL_MODE:
			pModeTutorial->update();
			if (pModeTutorial->getIsSceneEnd())
				gGameScene = RETURN_TO_MODE_SELECT;
			break;

		case GS_MANUAL_MODE:
			pModeManual->update();
			if (pModeManual->getIsSceneEnd())
				gGameScene = RETURN_TO_MODE_SELECT;
			break;

		case RETURN_TO_TITLE:{	// ↓---- 仲介用遷移
			gGameScene = TITLE_SCENE;
			pTitle = new Title;

			// 全てのポインタを開放しておく
			safeDelete(pModeSelect);
			safeDelete(pModeArcade);
			safeDelete(pModeTutorial);
			safeDelete(pModeVersus);
			safeDelete(pModeManual);

			break;
		}
		case RETURN_TO_MODE_SELECT:{
			pModeSelect = new ModeSelect;
			gGameScene = MODE_SELECT;
			safeDelete(pTitle);
			safeDelete(pModeArcade);
			safeDelete(pModeVersus);
			safeDelete(pModeTutorial);
			safeDelete(pModeManual);

			break;
		}
	}
}
