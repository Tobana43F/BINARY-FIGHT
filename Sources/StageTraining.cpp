#include	"StageTraining.h"
#include	"TextureLoader.h"	// テクスチャを使用するため
#include	"MyTexture.h"		// 使用するテクスチャを参照するため

//==========================================================
// StageTrainingクラス
//----------------------------------------------------------
// コンストラクタ
StageTraining::StageTraining(){
	// テクスチャ
	pTextureLoader = new TextureLoader(MAX_TEX);
	pTextureLoader->setUseTextureLabel(BACK_GROUND, textureNS::STAGE_BACKGROUND_TEST);
	// ロードは、上位クラスの関数から呼び出せばOK

	soundLabelBGM = soundNS::STAGE_TRAINING;
}

//----------------------------------------------------------
// デストラクタ
StageTraining::~StageTraining(){
	
}

//----------------------------------------------------------
// ステージの初期化
void StageTraining::initStage(){
	// テクスチャの設定
	polyBackGround.setTexturePointer(pTextureLoader->getTexturePointer(BACK_GROUND));
}