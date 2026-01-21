#include	"MyHitBox.h"
#include	"Constants.h"
#include	"ShareBaseFunc.h"

using namespace hitBoxNS;

namespace hitBoxNS{
	MyHitBoxInfo HITBOX_INFO[] = {
		MyHitBoxInfo("DotsHurtBox"),
		MyHitBoxInfo("DotsBodyBox"),
		MyHitBoxInfo("DotsAttackBox"),
		MyHitBoxInfo("DotsGuardBox"),
		MyHitBoxInfo("DotsWallBox"),

	};



	//==========================================================
	// メソッド定義
	//----------------------------------------------------------
	// コンストラクタ
	MyHitBoxInfo::MyHitBoxInfo(char* _fileName){
		stringCopy(filePath, FILE_PATH);
		stringAdd(filePath, FILE_HITBOX);
		stringAdd(filePath, _fileName);
		stringAdd(filePath, CSV_FILE_EXTENSION);
	}

	//----------------------------------------------------------
	// デストラクタ
	MyHitBoxInfo::~MyHitBoxInfo(){

	}
}