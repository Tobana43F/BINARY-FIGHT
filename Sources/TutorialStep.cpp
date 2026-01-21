#include	"TutorialStep.h"	// メソッド定義
#include	"MyTexture.h"
#include	"CharacterDots.h"

//==========================================================
// AbstractTutorialStep
//----------------------------------------------------------
// コンストラクタ
AbstractTutorialStep::AbstractTutorialStep() :
STRING_COL_NOT_CLEARD_R(0xFF),
STRING_COL_NOT_CLEARD_G(0xFF),
STRING_COL_NOT_CLEARD_B(0xFF),
STRING_COL_CLEARD_R(0xFF),
STRING_COL_CLEARD_G(0x00),
STRING_COL_CLEARD_B(0x00),
STEP_TITLE_WIDTH(DEF_WINDOW_WIDTH),
STEP_TITLE_HEIGHT(100),
STEP_TITLE_X(WINDOW_WIDTH / 2.0f - STEP_TITLE_WIDTH / 2.0f),
STEP_TITLE_Y(0),
STEP_DESCRIPTION_WIDTH(DEF_WINDOW_WIDTH),
STEP_DESCRIPTION_HEIGHT(64),
STEP_DESCRIPTION_X(0),
STEP_DESCRIPTION_Y(WINDOW_HEIGHT - STEP_DESCRIPTION_HEIGHT),
STEP_INSTRACTION_WIDTH(640),
STEP_INSTRACTION_HEIGHT(40),
STEP_INSTRACTION_X(WINDOW_WIDTH / 2.0f - WINDOW_WIDTH * 0.45),
STEP_INSTRACTION_Y(WINDOW_HEIGHT * 0.21f)
{
	isStepCleard = false;
	pBattle = NULL;	// 初期化

	// 共通のポリゴンの初期化
	polyStepTitle.setData(STEP_TITLE_X, STEP_TITLE_Y, STEP_TITLE_WIDTH, STEP_TITLE_HEIGHT);
	polyStepTitle.changeVertexColor(0xFF, 0xFF, 0xFF);
	polyStepDescription.setData(STEP_DESCRIPTION_X, STEP_DESCRIPTION_Y, STEP_DESCRIPTION_WIDTH, STEP_DESCRIPTION_HEIGHT);
	polyStepDescription.changeVertexColor(0xFF, 0xFF, 0xFF);
}

//----------------------------------------------------------
// デストラクタ
AbstractTutorialStep::~AbstractTutorialStep(){
	pBattle = NULL;
	safeDelete(pTextureLoader);
	if (pPolyInstractString != NULL){
		delete[] pPolyInstractString;
		pPolyInstractString = NULL;
	}

}

//----------------------------------------------------------
// 参照先ばとるを設定
void AbstractTutorialStep::setBattlePointer(Battle *_pBattle){
	pBattle = _pBattle;
}

//----------------------------------------------------------
// 読み込み(アニメーションは使用しないようにする)
void AbstractTutorialStep::load(){
	pTextureLoader->loadUseTexture();
}

//----------------------------------------------------------
// 描画　共通の処理にして、追加を楽にする
void AbstractTutorialStep::draw(){
	polyStepTitle.drawPolygon();
	polyStepDescription.drawPolygon();

	for (int i = 0; i < instructionNum; ++i){
		pPolyInstractString[i].drawPolygon();
	}
}


//==========================================================
// ステップ１　移動の練習
TutorialStepMove::TutorialStepMove(){							// 仮)適当なテクスチャをロードしてみる
	pTextureLoader = new TextureLoader(MAX_TEX);
	pTextureLoader->setUseTextureLabel(STEP_TITLE, textureNS::TUTORIAL_STEP_MOVE_TITLE);
	pTextureLoader->setUseTextureLabel(STEP_DESCRIPTION, textureNS::TUTORIAL_STEP_MOVE_DESCRIPTION);
	pTextureLoader->setUseTextureLabel(INSTRACT_WALK_FORWARD, textureNS::TUTORIAL_STEP_MOVE_INSTRACT_1);
	pTextureLoader->setUseTextureLabel(INSTRACT_WALK_BACK, textureNS::TUTORIAL_STEP_MOVE_INSTRACT_2);

	// 達成項目表示用ポリゴン
	instructionNum = MAX_INSTRACTION;	// 上位クラスの変数に、項目数を渡す
	pPolyInstractString = new Polygon2D[MAX_INSTRACTION];
}

TutorialStepMove::~TutorialStepMove(){

}

void TutorialStepMove::init(){
	//----------------------------------------------------------
	// フラグの初期化
	isWalkForwardOK = false;
	isWalkBackOK = false;

	//----------------------------------------------------------
	// ポリゴンにテクスチャを設定
	polyStepTitle.setTexturePointer(pTextureLoader->getTexturePointer(STEP_TITLE));
	polyStepDescription.setTexturePointer(pTextureLoader->getTexturePointer(STEP_DESCRIPTION));

	for (int i = 0; i < MAX_INSTRACTION; ++i){
		pPolyInstractString[i].setData(
			STEP_INSTRACTION_X, STEP_INSTRACTION_Y + STEP_INSTRACTION_HEIGHT * i,
			STEP_INSTRACTION_WIDTH, STEP_INSTRACTION_HEIGHT);

		pPolyInstractString[i].setTexturePointer(pTextureLoader->getTexturePointer(INSTRACT_WALK_FORWARD + i));	// 文字列の開始番号から連番
		pPolyInstractString[i].changeVertexColor(STRING_COL_NOT_CLEARD_R, STRING_COL_NOT_CLEARD_G, STRING_COL_NOT_CLEARD_B);	// 頂点カラーを設定
	}
}

void TutorialStepMove::update(){
	checkClear();
	
	if (isWalkForwardOK && isWalkBackOK){
		isStepCleard = true;
	}
}


void TutorialStepMove::checkClear(){
	if (pBattle->player[0]->getCurActionState() == Actor::WALK_FORWARD){
		isWalkForwardOK = true;
		pPolyInstractString[WALK_FORWARD].changeVertexColor(STRING_COL_CLEARD_R, STRING_COL_CLEARD_G, STRING_COL_CLEARD_B);
	}

	if (pBattle->player[0]->getCurActionState() == Actor::WALK_BACK){
		isWalkBackOK = true;
		pPolyInstractString[WALK_BACK].changeVertexColor(STRING_COL_CLEARD_R, STRING_COL_CLEARD_G, STRING_COL_CLEARD_B);
	}
}

//==========================================================
// ステップ２　ジャンプ
TutorialStepJump::TutorialStepJump(){							// 仮)適当なテクスチャをロードしてみる
	pTextureLoader = new TextureLoader(MAX_TEX);
	pTextureLoader->setUseTextureLabel(STEP_TITLE, textureNS::TUTORIAL_STEP_JUMP_TITLE);
	pTextureLoader->setUseTextureLabel(STEP_DESCRIPTION, textureNS::TUTORIAL_STEP_JUMP_DESCRIPTION);
	pTextureLoader->setUseTextureLabel(INSTRACT_JUMP_VIRTICAL, textureNS::TUTORIAL_STEP_JUMP_INSTRACT_1);
	pTextureLoader->setUseTextureLabel(INSTRACT_JUMP_BACK, textureNS::TUTORIAL_STEP_JUMP_INSTRACT_2);
	pTextureLoader->setUseTextureLabel(INSTRACT_JUMP_FORWARD, textureNS::TUTORIAL_STEP_JUMP_INSTRACT_3);

	// 達成項目表示用ポリゴン
	instructionNum = MAX_INSTRACTION;	// 上位クラスの変数に、項目数を渡す
	pPolyInstractString = new Polygon2D[MAX_INSTRACTION];
}

TutorialStepJump::~TutorialStepJump(){

}


void TutorialStepJump::init(){
	//----------------------------------------------------------
	// フラグの初期化
	isJumpVirtical = false;
	isJumpBack = false;
	isJumpForward = false;

	//----------------------------------------------------------
	// ポリゴンにテクスチャを設定
	polyStepTitle.setTexturePointer(pTextureLoader->getTexturePointer(STEP_TITLE));
	polyStepDescription.setTexturePointer(pTextureLoader->getTexturePointer(STEP_DESCRIPTION));

	for (int i = 0; i < MAX_INSTRACTION; ++i){
		pPolyInstractString[i].setData(
			STEP_INSTRACTION_X, STEP_INSTRACTION_Y + STEP_INSTRACTION_HEIGHT * i,
			STEP_INSTRACTION_WIDTH, STEP_INSTRACTION_HEIGHT);

		pPolyInstractString[i].setTexturePointer(pTextureLoader->getTexturePointer(INSTRACT_JUMP_VIRTICAL + i));	// 文字列の開始番号から連番
		pPolyInstractString[i].changeVertexColor(STRING_COL_NOT_CLEARD_R, STRING_COL_NOT_CLEARD_G, STRING_COL_NOT_CLEARD_B);	// 頂点カラーを設定
	}
}

void TutorialStepJump::update(){
	checkClear();

	if (isJumpVirtical && isJumpBack && isJumpForward){
		isStepCleard = true;
	}
}


void TutorialStepJump::checkClear(){
	if (pBattle->player[0]->getCurActionState() == Actor::JUMP_VIRTICAL){
		isJumpVirtical = true;
		pPolyInstractString[JUMP_VIRTICAL].changeVertexColor(STRING_COL_CLEARD_R, STRING_COL_CLEARD_G, STRING_COL_CLEARD_B);
	}

	if (pBattle->player[0]->getCurActionState() == Actor::JUMP_BACK){
		isJumpBack = true;
		pPolyInstractString[JUMP_BACK].changeVertexColor(STRING_COL_CLEARD_R, STRING_COL_CLEARD_G, STRING_COL_CLEARD_B);
	}

	if (pBattle->player[0]->getCurActionState() == Actor::JUMP_FORWARD){
		isJumpForward = true;
		pPolyInstractString[JUMP_FORWARD].changeVertexColor(STRING_COL_CLEARD_R, STRING_COL_CLEARD_G, STRING_COL_CLEARD_B);
	}
}

//==========================================================
// ステップ3 攻撃
TutorialStepAttack::TutorialStepAttack() :							// 仮)適当なテクスチャをロードしてみる
CLEAR_ATTACK_QUOTA(10)
{
	pTextureLoader = new TextureLoader(MAX_TEX);
	pTextureLoader->setUseTextureLabel(STEP_TITLE, textureNS::TUTORIAL_STEP_ATTACK_TITLE);
	pTextureLoader->setUseTextureLabel(STEP_DESCRIPTION, textureNS::TUTORIAL_STEP_ATTACK_DESCRIPTION);
	pTextureLoader->setUseTextureLabel(INSTRACT_ATTACK, textureNS::TUTORIAL_STEP_ATTACK_INSTRACT_1);

	// 達成項目表示用ポリゴン
	instructionNum = MAX_INSTRACTION;	// 上位クラスの変数に、項目数を渡す
	pPolyInstractString = new Polygon2D[MAX_INSTRACTION];
}

TutorialStepAttack::~TutorialStepAttack(){

}


void TutorialStepAttack::init(){
	//----------------------------------------------------------
	// フラグの初期化
	isAttackSomeTime = false;
	prevIsAttackHit = false;
	cntAttack = 0;

	//----------------------------------------------------------
	// ポリゴンにテクスチャを設定
	polyStepTitle.setTexturePointer(pTextureLoader->getTexturePointer(STEP_TITLE));
	polyStepDescription.setTexturePointer(pTextureLoader->getTexturePointer(STEP_DESCRIPTION));

	for (int i = 0; i < MAX_INSTRACTION; ++i){
		pPolyInstractString[i].setData(
			STEP_INSTRACTION_X, STEP_INSTRACTION_Y + STEP_INSTRACTION_HEIGHT * i,
			STEP_INSTRACTION_WIDTH, STEP_INSTRACTION_HEIGHT);

		pPolyInstractString[i].setTexturePointer(pTextureLoader->getTexturePointer(INSTRACT_ATTACK + i));	// 文字列の開始番号から連番
		pPolyInstractString[i].changeVertexColor(STRING_COL_NOT_CLEARD_R, STRING_COL_NOT_CLEARD_G, STRING_COL_NOT_CLEARD_B);	// 頂点カラーを設定
	}
}

void TutorialStepAttack::update(){
	checkClear();

	if (isAttackSomeTime){
		isStepCleard = true;
	}
}


void TutorialStepAttack::checkClear(){
	if (pBattle->player[0]->isAttackHit != prevIsAttackHit && pBattle->player[0]->isAttackHit == true){
		cntAttack++;
	}

	if (cntAttack >= CLEAR_ATTACK_QUOTA){
		isAttackSomeTime = true;
		pPolyInstractString[ATTACK_SOME_TIME].changeVertexColor(STRING_COL_CLEARD_R, STRING_COL_CLEARD_G, STRING_COL_CLEARD_B);
	}

	prevIsAttackHit = pBattle->player[0]->isAttackHit;
}

//==========================================================
// ステップ3 上段ガード
TutorialStepGuardHighPos::TutorialStepGuardHighPos() :							// 仮)適当なテクスチャをロードしてみる
CLEAR_GUARD_QUOTA(5)
{
	pTextureLoader = new TextureLoader(MAX_TEX);
	pTextureLoader->setUseTextureLabel(STEP_TITLE, textureNS::TUTORIAL_STEP_GUARD_HIGH_POS_TITLE);
	pTextureLoader->setUseTextureLabel(STEP_DESCRIPTION, textureNS::TUTORIAL_STEP_GUARD_HIGH_POS_DESCRIPTION);
	pTextureLoader->setUseTextureLabel(INSTRACT_GUARD, textureNS::TUTORIAL_STEP_GUARD_HIGH_POS_INSTRACT_1);

	// 達成項目表示用ポリゴン
	instructionNum = MAX_INSTRACTION;	// 上位クラスの変数に、項目数を渡す
	pPolyInstractString = new Polygon2D[MAX_INSTRACTION];
}

TutorialStepGuardHighPos::~TutorialStepGuardHighPos(){

}


void TutorialStepGuardHighPos::init(){
	//----------------------------------------------------------
	// フラグの初期化
	isGuardSomeTime = false;
	prevIsBlock = false;
	cntGuard = 0;

	//----------------------------------------------------------
	// ポリゴンにテクスチャを設定
	polyStepTitle.setTexturePointer(pTextureLoader->getTexturePointer(STEP_TITLE));
	polyStepDescription.setTexturePointer(pTextureLoader->getTexturePointer(STEP_DESCRIPTION));

	for (int i = 0; i < MAX_INSTRACTION; ++i){
		pPolyInstractString[i].setData(
			STEP_INSTRACTION_X, STEP_INSTRACTION_Y + STEP_INSTRACTION_HEIGHT * i,
			STEP_INSTRACTION_WIDTH, STEP_INSTRACTION_HEIGHT);

		pPolyInstractString[i].setTexturePointer(pTextureLoader->getTexturePointer(INSTRACT_GUARD + i));	// 文字列の開始番号から連番
		pPolyInstractString[i].changeVertexColor(STRING_COL_NOT_CLEARD_R, STRING_COL_NOT_CLEARD_G, STRING_COL_NOT_CLEARD_B);	// 頂点カラーを設定
	}
}

void TutorialStepGuardHighPos::update(){
	checkClear();

	if (isGuardSomeTime){
		isStepCleard = true;
	}
}


void TutorialStepGuardHighPos::checkClear(){
	if (pBattle->player[0]->isBlock != prevIsBlock && pBattle->player[0]->isBlock == true){
		cntGuard++;
	}

	if (cntGuard >= CLEAR_GUARD_QUOTA){
		isGuardSomeTime = true;
		pPolyInstractString[GUARD_SOMETIME].changeVertexColor(STRING_COL_CLEARD_R, STRING_COL_CLEARD_G, STRING_COL_CLEARD_B);
	}

	prevIsBlock = pBattle->player[0]->isBlock;
}

//==========================================================
// ステップ4 下段ガード
TutorialStepGuardLowPos::TutorialStepGuardLowPos() :							// 仮)適当なテクスチャをロードしてみる
CLEAR_GUARD_QUOTA(5)
{
	pTextureLoader = new TextureLoader(MAX_TEX);
	pTextureLoader->setUseTextureLabel(STEP_TITLE, textureNS::TUTORIAL_STEP_GUARD_LOW_POS_TITLE);
	pTextureLoader->setUseTextureLabel(STEP_DESCRIPTION, textureNS::TUTORIAL_STEP_GUARD_LOW_POS_DESCRIPTION);
	pTextureLoader->setUseTextureLabel(INSTRACT_GUARD, textureNS::TUTORIAL_STEP_GUARD_LOW_POS_INSTRACT_1);

	// 達成項目表示用ポリゴン
	instructionNum = MAX_INSTRACTION;	// 上位クラスの変数に、項目数を渡す
	pPolyInstractString = new Polygon2D[MAX_INSTRACTION];
}

TutorialStepGuardLowPos::~TutorialStepGuardLowPos(){

}


void TutorialStepGuardLowPos::init(){
	//----------------------------------------------------------
	// フラグの初期化
	isGuardSomeTime = false;
	prevIsBlock = false;
	cntGuard = 0;

	//----------------------------------------------------------
	// ポリゴンにテクスチャを設定
	polyStepTitle.setTexturePointer(pTextureLoader->getTexturePointer(STEP_TITLE));
	polyStepDescription.setTexturePointer(pTextureLoader->getTexturePointer(STEP_DESCRIPTION));

	for (int i = 0; i < MAX_INSTRACTION; ++i){
		pPolyInstractString[i].setData(
			STEP_INSTRACTION_X, STEP_INSTRACTION_Y + STEP_INSTRACTION_HEIGHT * i,
			STEP_INSTRACTION_WIDTH, STEP_INSTRACTION_HEIGHT);

		pPolyInstractString[i].setTexturePointer(pTextureLoader->getTexturePointer(INSTRACT_GUARD + i));	// 文字列の開始番号から連番
		pPolyInstractString[i].changeVertexColor(STRING_COL_NOT_CLEARD_R, STRING_COL_NOT_CLEARD_G, STRING_COL_NOT_CLEARD_B);	// 頂点カラーを設定
	}
}

void TutorialStepGuardLowPos::update(){
	checkClear();

	if (isGuardSomeTime){
		isStepCleard = true;
	}
}


void TutorialStepGuardLowPos::checkClear(){
	if (pBattle->player[0]->isBlock != prevIsBlock && pBattle->player[0]->isBlock == true){
		cntGuard++;
	}

	if (cntGuard >= CLEAR_GUARD_QUOTA){
		isGuardSomeTime = true;
		pPolyInstractString[GUARD_SOMETIME].changeVertexColor(STRING_COL_CLEARD_R, STRING_COL_CLEARD_G, STRING_COL_CLEARD_B);
	}

	prevIsBlock = pBattle->player[0]->isBlock;
}


//==========================================================
// ステップ5 中断ガード
TutorialStepGuardMidPos::TutorialStepGuardMidPos() :							// 仮)適当なテクスチャをロードしてみる
CLEAR_GUARD_QUOTA(5)
{
	pTextureLoader = new TextureLoader(MAX_TEX);
	pTextureLoader->setUseTextureLabel(STEP_TITLE, textureNS::TUTORIAL_STEP_GUARD_MID_POS_TITLE);
	pTextureLoader->setUseTextureLabel(STEP_DESCRIPTION, textureNS::TUTORIAL_STEP_GUARD_MID_POS_DESCRIPTION);
	pTextureLoader->setUseTextureLabel(INSTRACT_GUARD, textureNS::TUTORIAL_STEP_GUARD_MID_POS_INSTRACT_1);

	// 達成項目表示用ポリゴン
	instructionNum = MAX_INSTRACTION;	// 上位クラスの変数に、項目数を渡す
	pPolyInstractString = new Polygon2D[MAX_INSTRACTION];
}

TutorialStepGuardMidPos::~TutorialStepGuardMidPos(){

}


void TutorialStepGuardMidPos::init(){
	//----------------------------------------------------------
	// フラグの初期化
	isGuardSomeTime = false;
	prevIsBlock = false;
	cntGuard = 0;

	//----------------------------------------------------------
	// ポリゴンにテクスチャを設定
	polyStepTitle.setTexturePointer(pTextureLoader->getTexturePointer(STEP_TITLE));
	polyStepDescription.setTexturePointer(pTextureLoader->getTexturePointer(STEP_DESCRIPTION));

	for (int i = 0; i < MAX_INSTRACTION; ++i){
		pPolyInstractString[i].setData(
			STEP_INSTRACTION_X, STEP_INSTRACTION_Y + STEP_INSTRACTION_HEIGHT * i,
			STEP_INSTRACTION_WIDTH, STEP_INSTRACTION_HEIGHT);

		pPolyInstractString[i].setTexturePointer(pTextureLoader->getTexturePointer(INSTRACT_GUARD + i));	// 文字列の開始番号から連番
		pPolyInstractString[i].changeVertexColor(STRING_COL_NOT_CLEARD_R, STRING_COL_NOT_CLEARD_G, STRING_COL_NOT_CLEARD_B);	// 頂点カラーを設定
	}
}

void TutorialStepGuardMidPos::update(){
	checkClear();

	if (isGuardSomeTime){
		isStepCleard = true;
	}
}


void TutorialStepGuardMidPos::checkClear(){
	if (pBattle->player[0]->isBlock != prevIsBlock && pBattle->player[0]->isBlock == true){
		cntGuard++;
	}

	if (cntGuard >= CLEAR_GUARD_QUOTA){
		isGuardSomeTime = true;
		pPolyInstractString[GUARD_SOMETIME].changeVertexColor(STRING_COL_CLEARD_R, STRING_COL_CLEARD_G, STRING_COL_CLEARD_B);
	}

	prevIsBlock = pBattle->player[0]->isBlock;
}

//==========================================================
// 最終ステップ　敵をKO
TutorialStepBattle::TutorialStepBattle()
{
	pTextureLoader = new TextureLoader(MAX_TEX);
	pTextureLoader->setUseTextureLabel(STEP_TITLE, textureNS::TUTORIAL_STEP_BATTLE_TITLE);
	pTextureLoader->setUseTextureLabel(STEP_DESCRIPTION, textureNS::TUTORIAL_STEP_BATTLE_DESCRIPTION);
	pTextureLoader->setUseTextureLabel(INSTRACT_KO, textureNS::TUTORIAL_STEP_BATTLE_INSTRACT_1);

	// 達成項目表示用ポリゴン
	instructionNum = MAX_INSTRACTION;	// 上位クラスの変数に、項目数を渡す
	pPolyInstractString = new Polygon2D[MAX_INSTRACTION];
}

TutorialStepBattle::~TutorialStepBattle(){

}


void TutorialStepBattle::init(){
	//----------------------------------------------------------
	// フラグの初期化
	isOpponentKO = false;

	//----------------------------------------------------------
	// ポリゴンにテクスチャを設定
	polyStepTitle.setTexturePointer(pTextureLoader->getTexturePointer(STEP_TITLE));
	polyStepDescription.setTexturePointer(pTextureLoader->getTexturePointer(STEP_DESCRIPTION));

	for (int i = 0; i < MAX_INSTRACTION; ++i){
		pPolyInstractString[i].setData(
			STEP_INSTRACTION_X, STEP_INSTRACTION_Y + STEP_INSTRACTION_HEIGHT * i,
			STEP_INSTRACTION_WIDTH, STEP_INSTRACTION_HEIGHT);

		pPolyInstractString[i].setTexturePointer(pTextureLoader->getTexturePointer(INSTRACT_KO + i));	// 文字列の開始番号から連番
		pPolyInstractString[i].changeVertexColor(STRING_COL_NOT_CLEARD_R, STRING_COL_NOT_CLEARD_G, STRING_COL_NOT_CLEARD_B);	// 頂点カラーを設定
	}
}

void TutorialStepBattle::update(){
	checkClear();

	if (isOpponentKO){
		isStepCleard = true;
	}
}


void TutorialStepBattle::checkClear(){
	if (pBattle->player[1]->isKO == true){
		isOpponentKO = true;
		pPolyInstractString[KO].changeVertexColor(STRING_COL_CLEARD_R, STRING_COL_CLEARD_G, STRING_COL_CLEARD_B);
	}
}

