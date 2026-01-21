#include	"MyAnimation.h"
#include	"ShareBaseFunc.h"	// 文字列関係の処理を行うため

using namespace animationNS;

namespace animationNS{
	MyAnimationInfo ANIMATION_INFO[] = {
		MyAnimationInfo("_Nothing"),

		MyAnimationInfo("Char_Dots_Normal"),
		MyAnimationInfo("Char_Dots_Damage"),
		MyAnimationInfo("Char_Dots_ATttack_Normal"),
		MyAnimationInfo("Char_Dots_Pose"),

		MyAnimationInfo("BattleEffect_HighAttack"),
		MyAnimationInfo("BattleEffect_HighGuard"),
		
		MyAnimationInfo("CharacterSelect_Frame"),
	};

	//==========================================================
	// メソッド定義
	//----------------------------------------------------------
	// コンストラクタ
	MyAnimationInfo::MyAnimationInfo(char* _fileName){
		stringCopy(filePath, FILE_PATH);
		stringAdd(filePath, FILE_ANIMATION);
		stringAdd(filePath, _fileName);
		stringAdd(filePath, ANIMATION_FILE_EXTENSION);
	}

	//----------------------------------------------------------
	// デストラクタ
	MyAnimationInfo::~MyAnimationInfo(){

	}
}