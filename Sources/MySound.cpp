//==========================================================
// サウンド関係のグローバル変数宣言など

#include	"MyDefinitions.h"
#include	"Constants.h"
#include	"MySound.h"
#include	"ShareBaseFunc.h"

using namespace soundNS;

namespace soundNS{
	//==========================================================
	// ファイル名を記述(クラスなのでコンストラクタで行う)
	MySoundInfo FN_SOUNDS[SOUND_LABEL_MAX] = {
		MySoundInfo("Stage_SkyPlace", true),
		MySoundInfo("Stage_Training", true),
		MySoundInfo("testHitSE", false),
		MySoundInfo("testBlockSE", false),

		MySoundInfo("TitleBGM", true),
		MySoundInfo("TitleEnterSE", false),

		MySoundInfo("ModeSelectBGM", true),

		MySoundInfo("CharacterSelectBGM", true),
		MySoundInfo("CharacterSelectDecideSE", false),
		MySoundInfo("CharacterSelectCancelSE", false),

		MySoundInfo("GameReadyStartSE", false),
		MySoundInfo("GameReadyEndSE", false),

		MySoundInfo("CursorSelectSE", false),
		MySoundInfo("Decide_GeneralSE", false),
		MySoundInfo("ManualSelectSE", false),
	};


	//==========================================================
	// クラスメンバ関数の定義
	//----------------------------------------------------------
	// 初期化子用コンストラクタ
	MySoundInfo::MySoundInfo(char *fileName, bool _flgLoop){
		stringCopy(filePath, FILE_PATH);
		stringAdd(filePath, FILE_SOUND);
		stringAdd(filePath, fileName);
		stringAdd(filePath, SOUND_FILE_EXTENSION);		// wavファイルとして読み込む
		flgLoop = _flgLoop;
		handle = 0;
	}

	//----------------------------------------------------------
	// デストラクタ
	MySoundInfo::~MySoundInfo(){
		
	}

	//----------------------------------------------------------
	// ファイル名を取得する
	char* MySoundInfo::getFilePath(){
		return filePath;
	}

	//----------------------------------------------------------
	// ループフラグを取得する
	bool MySoundInfo::getLoopFlag(){
		return flgLoop;
	}
}