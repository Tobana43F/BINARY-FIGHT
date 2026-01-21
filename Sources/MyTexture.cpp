#include	"MyTexture.h"
#include	"ShareBaseFunc.h"
#include	<stdarg.h>					// 可変引数を利用した関数を作るため


using namespace textureNS;

// 見やすくするためだけに定数にしただけ
#define TEX_SIZE_1		(1)
#define TEX_SIZE_2		(2)
#define TEX_SIZE_4		(4)
#define TEX_SIZE_8		(8)
#define TEX_SIZE_16		(16)
#define TEX_SIZE_32		(32)
#define TEX_SIZE_64		(64)
#define TEX_SIZE_128	(128)
#define TEX_SIZE_256	(256)
#define TEX_SIZE_512	(512)
#define TEX_SIZE_1024	(1024)

namespace textureNS{
	//==========================================================
	// テクスチャ情報を持つグローバル変数
	MyTextureInfo TEXTURE_INFO[] = {
		MyTextureInfo("_noTexture", 1, 1, TEX_SIZE_2, TEX_SIZE_2),
		MyTextureInfo("_alphaTexture", 1, 1, TEX_SIZE_2, TEX_SIZE_2),

		MyTextureInfo("General_NowLoadingString", 1, 1, TEX_SIZE_256, TEX_SIZE_32),

		MyTextureInfo("Font_System", 16, 16, TEX_SIZE_256, TEX_SIZE_256),
		MyTextureInfo("Font_DotAndShadow", 16, 8, TEX_SIZE_512, TEX_SIZE_256),

		MyTextureInfo("Title_Back", 1, 1, 640, 480),
		MyTextureInfo("Title_Push+ToStart", 1, 1, TEX_SIZE_512, TEX_SIZE_64),


		MyTextureInfo("Manual_Page1", 1, 1, 640, 480),
		MyTextureInfo("Manual_Page2", 1, 1, 640, 480),
		MyTextureInfo("Manual_Page3", 1, 1, 640, 480),
		MyTextureInfo("Manual_Page4", 1, 1, 640, 480),

		MyTextureInfo("Tutorial_Clear", 1, 1, TEX_SIZE_256, TEX_SIZE_64),
		MyTextureInfo("Tutorial_Bar", 1, 1, TEX_SIZE_32, TEX_SIZE_64),
		MyTextureInfo("Tutorial_BarCap", 1, 1, TEX_SIZE_32, TEX_SIZE_64),

		MyTextureInfo("Tutorial_Step_Move_Title", 1, 1, 640, 100),
		MyTextureInfo("Tutorial_Step_Move_Description", 1, 1, 640, TEX_SIZE_64),
		MyTextureInfo("Tutorial_Step_Move_Instract_1", 1, 1, 640, 40),
		MyTextureInfo("Tutorial_Step_Move_Instract_2", 1, 1, 640, 40),

		MyTextureInfo("Tutorial_Step_Jump_Title", 1, 1, 640, 100),
		MyTextureInfo("Tutorial_Step_Jump_Description", 1, 1, 640, TEX_SIZE_64),
		MyTextureInfo("Tutorial_Step_Jump_Instract_1", 1, 1, 640, 40),
		MyTextureInfo("Tutorial_Step_Jump_Instract_2", 1, 1, 640, 40),
		MyTextureInfo("Tutorial_Step_Jump_Instract_3", 1, 1, 640, 40),

		MyTextureInfo("Tutorial_Step_Attack_Title", 1, 1, 640, 100),
		MyTextureInfo("Tutorial_Step_Attack_Description", 1, 1, 640, TEX_SIZE_64),
		MyTextureInfo("Tutorial_Step_Attack_Instract_1", 1, 1, 640, 40),

		MyTextureInfo("Tutorial_Step_GuardHighPos_Title", 1, 1, 640, 100),
		MyTextureInfo("Tutorial_Step_GuardHighPos_Description", 1, 1, 640, TEX_SIZE_64),
		MyTextureInfo("Tutorial_Step_GuardHighPos_Instract_1", 1, 1, 640, 40),

		MyTextureInfo("Tutorial_Step_GuardLowPos_Title", 1, 1, 640, 100),
		MyTextureInfo("Tutorial_Step_GuardLowPos_Description", 1, 1, 640, TEX_SIZE_64),
		MyTextureInfo("Tutorial_Step_GuardLowPos_Instract_1", 1, 1, 640, 40),

		MyTextureInfo("Tutorial_Step_GuardMidPos_Title", 1, 1, 640, 100),
		MyTextureInfo("Tutorial_Step_GuardMidPos_Description", 1, 1, 640, TEX_SIZE_64),
		MyTextureInfo("Tutorial_Step_GuardMidPos_Instract_1", 1, 1, 640, 40),

		MyTextureInfo("Tutorial_Step_Battle_Title", 1, 1, 640, 100),
		MyTextureInfo("Tutorial_Step_Battle_Description", 1, 1, 640, TEX_SIZE_64),
		MyTextureInfo("Tutorial_Step_Battle_Instract_1", 1, 1, 640, 40),

		MyTextureInfo("ModeSelect_Cursor", 1, 1, TEX_SIZE_256, TEX_SIZE_128),
		MyTextureInfo("ModeSelect_Back", 1, 1, 640, 480),
		MyTextureInfo("ModeSelect_Description_Arcade", 1, 1, 640, TEX_SIZE_64),
		MyTextureInfo("ModeSelect_Description_Versus", 1, 1, 640, TEX_SIZE_64),
		MyTextureInfo("ModeSelect_Description_Tutorial", 1, 1, 640, TEX_SIZE_64),
		MyTextureInfo("ModeSelect_Description_Manual", 1, 1, 640, TEX_SIZE_64),

		MyTextureInfo("Char_Dots_Normal", 8, 8, TEX_SIZE_256, TEX_SIZE_256),
		MyTextureInfo("Char_Dots_Damage", 8, 16, TEX_SIZE_256, TEX_SIZE_512),
		MyTextureInfo("Char_Dots_AttackNormal", 8, 16, TEX_SIZE_512, TEX_SIZE_512),
		MyTextureInfo("Char_Dots_Pose", 8, 8, TEX_SIZE_256, TEX_SIZE_256),

		MyTextureInfo("StageBackGround_Training", 1, 1, TEX_SIZE_1024, TEX_SIZE_512),
		MyTextureInfo("StageBackGround_SkyPlace", 1, 1, TEX_SIZE_1024, TEX_SIZE_512),

		MyTextureInfo("CharacterSelect_Back", 1, 1, 640, 480),
		MyTextureInfo("CharacterSelect_Frame_Player1", 2, 2, TEX_SIZE_256, TEX_SIZE_256),
		MyTextureInfo("CharacterSelect_Frame_Player2", 2, 2, TEX_SIZE_256, TEX_SIZE_256),
		MyTextureInfo("CharacterSelect_Dots", 1, 1, TEX_SIZE_128, TEX_SIZE_128),
		MyTextureInfo("CharacterSelect_Reserve", 1, 1, TEX_SIZE_128, TEX_SIZE_128),
		MyTextureInfo("CharacterSelect_Random", 1, 1, TEX_SIZE_128, TEX_SIZE_128),

		MyTextureInfo("GameReady_Font_V", 1, 1, TEX_SIZE_64, TEX_SIZE_64),
		MyTextureInfo("GameReady_Font_S", 1, 1, TEX_SIZE_64, TEX_SIZE_64),
		MyTextureInfo("GameReady_Back", 1, 1, 640, 480),

		MyTextureInfo("GameReady_Dots", 1, 1, TEX_SIZE_64, TEX_SIZE_64),

		MyTextureInfo("HpBar_Frame", 2, 2, TEX_SIZE_512, TEX_SIZE_64),
		MyTextureInfo("HpBar_Normal", 2, 2, TEX_SIZE_512, TEX_SIZE_64),
		MyTextureInfo("HpBar_Max", 2, 2, TEX_SIZE_512, TEX_SIZE_64),
		MyTextureInfo("AllRoundIcon", 8, 1, TEX_SIZE_512, TEX_SIZE_64),

		MyTextureInfo("BattleEffect_HighAttack", 4, 4, TEX_SIZE_512, TEX_SIZE_512),
		MyTextureInfo("BattleEffect_HighGuard", 4, 4, TEX_SIZE_512, TEX_SIZE_512),
		

		MyTextureInfo("RoundEffect_RoundString", 1, 1, TEX_SIZE_256, TEX_SIZE_64),
		MyTextureInfo("RoundEffect_Number", 4, 4, TEX_SIZE_128, TEX_SIZE_256),
		MyTextureInfo("RoundEffect_FightString", 1, 1, TEX_SIZE_256, TEX_SIZE_64),

		MyTextureInfo("RoundEffect_KO", 1, 1, TEX_SIZE_128, TEX_SIZE_64),
		MyTextureInfo("RoundEffect_Draw", 1, 1, TEX_SIZE_256, TEX_SIZE_64),
		MyTextureInfo("RoundEffect_TimeOver", 1, 1, TEX_SIZE_512, TEX_SIZE_64),
		MyTextureInfo("RoundEffect_Perfect", 1, 1, TEX_SIZE_256, TEX_SIZE_64),

		MyTextureInfo("RoundEffect_Player1Win", 1, 1, TEX_SIZE_512, TEX_SIZE_64),
		MyTextureInfo("RoundEffect_Player2Win", 1, 1, TEX_SIZE_512, TEX_SIZE_64),
		MyTextureInfo("RoundEffect_DrawGame", 1, 1, TEX_SIZE_512, TEX_SIZE_64),

		MyTextureInfo("CharcterShadow", 1, 1, TEX_SIZE_64, TEX_SIZE_64),
	};

	//==========================================================
	// メソッド定義
	MyTextureInfo::MyTextureInfo(char* _fileName, unsigned char _hDiv, unsigned char _vDiv, unsigned short _hSize, unsigned short _vSize){
		stringAdd(fileName, _fileName);
		hDiv = _hDiv;
		vDiv = _vDiv;
		hSize = _hSize;
		vSize = _vSize;
	}

	//----------------------------------------------------------
	// デストラクタ
	MyTextureInfo::~MyTextureInfo(){

	}
}

