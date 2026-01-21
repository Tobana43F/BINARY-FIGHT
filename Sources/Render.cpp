//==========================================================
// ‹¤’Ê‚Ì•`‰æŠÖŒW‚Ìˆ—‚ð‹Lq

#include	"Common.h"			// ƒOƒ[ƒoƒ‹•Ï”‚ðˆµ‚¤‚½‚ß
#include	"ShareBaseFunc.h"

//==========================================================
// •`‰æ‚ðs‚¤ŠÖ”Bƒ|ƒŠƒSƒ“‚Ì•\Ž¦‚È‚Ç
//  ‰æ–Ê‘JˆÚ‚ÍŒãXrun‘¤‚Åˆ—‚·‚é
void render(){
	switch (gGameScene){
		case TITLE_SCENE:{
			pTitle->draw();

			break;
		}
		case MODE_SELECT:{
			pModeSelect->draw();
			break;
		}
		case GS_ARCADE_MODE:	
			pModeArcade->draw();
			break;

		case GS_VERSUS_MODE:{
			pModeVersus->draw();
			break;
		}
		case GS_TUTORIAL_MODE:{
			pModeTutorial->draw();
			break;
		}
		case GS_MANUAL_MODE:{
			pModeManual->draw();
			break;
		}

		case RETURN_TO_TITLE:{
			// ‰½‚à•`‰æ‚µ‚È‚¢
			break;
		}
		case RETURN_TO_MODE_SELECT:{
			// ‰½‚à•`‰æ‚µ‚È‚¢
			break;
		}
	}
}
