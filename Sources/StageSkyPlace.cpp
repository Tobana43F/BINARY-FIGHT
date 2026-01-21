#include	"StageSkyPlace.h"
#include	"TextureLoader.h"	// テクスチャを使用するため
#include	"MyTexture.h"		// 使用するテクスチャを参照するため

//==========================================================
// StageTrainingクラス
//----------------------------------------------------------
// コンストラクタ
StageSkyPlace::StageSkyPlace(){
	// テクスチャ
	pTextureLoader = new TextureLoader(MAX_TEX);
	pTextureLoader->setUseTextureLabel(BACK_GROUND, textureNS::STAGE_BACKGROUND_SKY_PLACE);
	// ロードは、上位クラスの関数から呼び出せばOK

	soundLabelBGM = soundNS::STAGE_SKYPLACE;
}

//----------------------------------------------------------
// デストラクタ
StageSkyPlace::~StageSkyPlace(){

}

//----------------------------------------------------------
// ステージの初期化
void StageSkyPlace::initStage(){
	// テクスチャの設定
	polyBackGround.setTexturePointer(pTextureLoader->getTexturePointer(BACK_GROUND));
}