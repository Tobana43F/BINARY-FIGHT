#include	"BattleInput.h"	// メソッド定義

//==========================================================
// BattleInputDataクラス
//----------------------------------------------------------
// コンストラクタ
BattleInputData::BattleInputData(){
	button[BATTLE_BUTTON_UP] = &buttonUp;
	button[BATTLE_BUTTON_DOWN] = &buttonDown;
	button[BATTLE_BUTTON_LEFT] = &buttonLeft;
	button[BATTLE_BUTTON_RIGHT] = &buttonRight;
	button[BATTLE_BUTTON_1] = &button1;
	button[BATTLE_BUTTON_2] = &button2;
	button[BATTLE_BUTTON_A] = &buttonA;
	button[BATTLE_BUTTON_B] = &buttonB;
	button[BATTLE_BUTTON_PLUS] = &buttonPlus;
	button[BATTLE_BUTTON_MINUS] = &buttonMinus;

	init();
}

//----------------------------------------------------------
// デストラクタ
BattleInputData::~BattleInputData(){
	for (int i = 0; i < MAX_BATTLE_BUTTON; ++i)
		button[i] = NULL;
}

//----------------------------------------------------------
// 初期化
void BattleInputData::init(){
	rawInputState = 0;
	directionKey = DIR_5;	// ニュートラル

	buttonUp = false;
	buttonDown = false;
	buttonLeft = false;
	buttonRight = false;
	button1 = false;
	button2 = false;
	buttonA = false;
	buttonB = false;
	buttonPlus = false;
	buttonMinus = false;
	cntInputUpdated = 0;	// 適当な初期化

	next = NULL;
}


//==========================================================
// BattleInputManagerクラス
//----------------------------------------------------------
// コンストラクタ
BattleInputManager::BattleInputManager(){
	curData.init();
	trigger.init();
	hold.init();
	release.init();
	pPrevData = NULL;
	isInputUpdated = false;

	// 使用しやすいようにまとめとく
	useData[TRIGGER] = &trigger;
	useData[HOLD] = &hold;
	useData[RELEASE] = &release;

	pPrevData = new BattleInputData;	// 最後尾として確保
}

//----------------------------------------------------------
// デストラクタ
BattleInputManager::~BattleInputManager(){
	pBattleCnt = NULL;	// 参照を解除

	freeAllBattleInputDataList();
}

//----------------------------------------------------------
// 更新
void BattleInputManager::update(){
	// 履歴の更新
	checkInputUpdate();

	// コントローラーからの入力
	updateRawInputState();
	updateBattleInputState();

	// 使用できる入力状態の更新
	updateTrigger();
	updateHold();
	updateRelease();
}

//----------------------------------------------------------
// コントローラーの生の入力を更新
void BattleInputManager::updateRawInputState(){
	// プログラム制御の場合は実行しない
	if (useControllerNo == inputNS::PROGRAM)
		return;

	curData.rawInputState = 0;
	curData.rawInputState |= (getInputState(inputNS::HOLD, BUTTON_UP, useControllerNo) << BattleInputData::SHIFT_UP);
	curData.rawInputState |= (getInputState(inputNS::HOLD, BUTTON_DOWN, useControllerNo) << BattleInputData::SHIFT_DOWN);
	curData.rawInputState |= (getInputState(inputNS::HOLD, BUTTON_LEFT, useControllerNo) << BattleInputData::SHIFT_LEFT);
	curData.rawInputState |= (getInputState(inputNS::HOLD, BUTTON_RIGHT, useControllerNo) << BattleInputData::SHIFT_RIGHT);

	curData.rawInputState |= (getInputState(inputNS::HOLD, BUTTON_1, useControllerNo) << BattleInputData::SHIFT_1);
	curData.rawInputState |= (getInputState(inputNS::HOLD, BUTTON_2, useControllerNo) << BattleInputData::SHIFT_2);
	curData.rawInputState |= (getInputState(inputNS::HOLD, BUTTON_A, useControllerNo) << BattleInputData::SHIFT_A);
	curData.rawInputState |= (getInputState(inputNS::HOLD, BUTTON_B, useControllerNo) << BattleInputData::SHIFT_B);

	curData.rawInputState |= (getInputState(inputNS::HOLD, BUTTON_PLUS, useControllerNo) << BattleInputData::SHIFT_PLUS);
	curData.rawInputState |= (getInputState(inputNS::HOLD, BUTTON_MINUS, useControllerNo) << BattleInputData::SHIFT_MINUS);
}

//----------------------------------------------------------
// コントローラーの生の入力状態から、バトルで使う変数を設定。
void BattleInputManager::updateBattleInputState(){
	if (useControllerNo == inputNS::PROGRAM)
		return;

	//----------------------------------------------------------
	// 入力状態の取得
	curData.directionKey = BattleInputData::DIR_5;	// 初期値として
	if ((curData.rawInputState & BattleInputData::MASK_UP) != 0)
		curData.directionKey += 3;	// テンキーのX方向の分割数
	else if ((curData.rawInputState & BattleInputData::MASK_DOWN) != 0)
		curData.directionKey -= 3;
	if ((curData.rawInputState & BattleInputData::MASK_LEFT) != 0)
		curData.directionKey -= 1;
	else if ((curData.rawInputState & BattleInputData::MASK_RIGHT) != 0)
		curData.directionKey += 1;

	curData.buttonUp = (curData.rawInputState & BattleInputData::MASK_UP) != 0;
	curData.buttonDown = (curData.rawInputState & BattleInputData::MASK_DOWN) != 0;
	curData.buttonLeft = (curData.rawInputState & BattleInputData::MASK_LEFT) != 0;
	curData.buttonRight = (curData.rawInputState & BattleInputData::MASK_RIGHT) != 0;

	curData.button1 = (curData.rawInputState & BattleInputData::MASK_1) != 0;
	curData.button2 = (curData.rawInputState & BattleInputData::MASK_2) != 0;
	curData.buttonA = (curData.rawInputState & BattleInputData::MASK_A) != 0;
	curData.buttonB = (curData.rawInputState & BattleInputData::MASK_B) != 0;
	curData.buttonPlus = (curData.rawInputState & BattleInputData::MASK_PLUS) != 0;
	curData.buttonMinus = (curData.rawInputState & BattleInputData::MASK_MINUS) != 0;

	//----------------------------------------------------------
	// 入力されたフレームを代入
	curData.cntInputUpdated = *pBattleCnt;
}

//----------------------------------------------------------
// trigger用
void BattleInputManager::updateTrigger(){
	trigger.init();	// トリガーとして使うため毎回初期化する

	for (int i = 0; i < MAX_BATTLE_BUTTON; ++i){
		if (*curData.button[i] != *pPrevData->button[i] && *curData.button[i] == true)
			*trigger.button[i] = true;
	}
}

// hold
void BattleInputManager::updateHold(){
	hold.init();
	
	for (int i = 0; i < MAX_BATTLE_BUTTON; ++i){
		if (*curData.button[i] == *pPrevData->button[i] && *curData.button[i] == true)
			*hold.button[i] = true;
	}
}

// release
void BattleInputManager::updateRelease(){
	release.init();	// トリガーとして使うため毎回初期化する

	for (int i = 0; i < MAX_BATTLE_BUTTON; ++i){
		if (*curData.button[i] != *pPrevData->button[i] && *curData.button[i] == false)
			*release.button[i] = true;
	}
}

//----------------------------------------------------------
// 更新状態のチェック
void BattleInputManager::checkInputUpdate(){
	isInputUpdated = false;

	// 方向キーの更新
	if (curData.directionKey != pPrevData->directionKey)
		isInputUpdated = true;

	for (int i = 0; i < MAX_BATTLE_BUTTON; ++i){
		if (*curData.button[i] != *pPrevData->button[i]){
			isInputUpdated = true;
			break;
		}
	}

	if (isInputUpdated)
		addBattleInputDataList();
}

//----------------------------------------------------------
// 入力状態の取得
bool BattleInputManager::getBattleInputState(eBattleInputType _type, int _buttonNo){
	return *useData[_type]->button[_buttonNo];
}

//----------------------------------------------------------
// 全リストの開放
void BattleInputManager::freeAllBattleInputDataList(){
	BattleInputData *pCur, *pWork;

	pCur = pPrevData;		// 先頭を代入
	while (pCur != NULL){	// 最後までループ
		pWork = pCur->next;	// 現在の次のポインタを格納
		delete pCur;		// 現在のポインタを開放
		pCur = pWork;		// 次へ
	}
	pPrevData = NULL;

}

//----------------------------------------------------------
// リストへの追加
void BattleInputManager::addBattleInputDataList(){
	BattleInputData *pNewData;
	pNewData = new BattleInputData;
	pNewData->init();
	//*pNewData = curData;			// 現在の入力状態を渡す
	//operater=()
	pNewData->rawInputState = curData.rawInputState;
	pNewData->directionKey = curData.directionKey;
	pNewData->buttonUp = curData.buttonUp;
	pNewData->buttonDown = curData.buttonDown;
	pNewData->buttonLeft = curData.buttonLeft;
	pNewData->buttonRight = curData.buttonRight;
	pNewData->button1 = curData.button1;
	pNewData->button2 = curData.button2;
	pNewData->buttonA = curData.buttonA;
	pNewData->buttonB = curData.buttonB;
	pNewData->buttonPlus = curData.buttonPlus;
	pNewData->buttonMinus = curData.buttonMinus;
	pNewData->cntInputUpdated = curData.cntInputUpdated;
	pNewData->next = NULL;

	pNewData->next = pPrevData;		// リストの先頭につなぐ
	pPrevData = pNewData;			// 前情報を最新のものにする
}

//----------------------------------------------------------
// プログラムでのボタン操作
void BattleInputManager::setButtonData(int _buttonNo, bool _val){
	*curData.button[_buttonNo] = _val;

	//----------------------------------------------------------
	// 方向キーの設定
	curData.directionKey = BattleInputData::DIR_5;	// 初期値として
	if (*curData.button[BATTLE_BUTTON_UP])
		curData.directionKey += 3;	// テンキーのX方向の分割数
	else if (*curData.button[BATTLE_BUTTON_DOWN])
		curData.directionKey -= 3;
	if (*curData.button[BATTLE_BUTTON_LEFT])
		curData.directionKey -= 1;
	else if (*curData.button[BATTLE_BUTTON_RIGHT])
		curData.directionKey += 1;

	curData.cntInputUpdated = *pBattleCnt;

	updateTrigger();
	updateHold();
	updateRelease();
}

void BattleInputManager::setDirectionKey(BattleInputData::eArrowDirection _val){
	curData.directionKey = _val;

	//----------------------------------------------------------
	// 入力状態も合わせて、変更する
	switch (curData.directionKey % 3){
		case 0:	// 右要素
			*curData.button[BATTLE_BUTTON_RIGHT] = true;
			*curData.button[BATTLE_BUTTON_LEFT] = false;
			break;

		case 1:	// 左要素
			*curData.button[BATTLE_BUTTON_RIGHT] = false;
			*curData.button[BATTLE_BUTTON_LEFT] = true;
			break;

		case 2:	// 中要素
			*curData.button[BATTLE_BUTTON_RIGHT] = false;
			*curData.button[BATTLE_BUTTON_LEFT] = false;
			break;
	}

	switch ((int)((curData.directionKey - 1) / 3)){	// 分かり安く　-１
		case 0:	// 下要素
			*curData.button[BATTLE_BUTTON_UP] = false;
			*curData.button[BATTLE_BUTTON_DOWN] = true;
			break;

		case 1:	// 中要素
			*curData.button[BATTLE_BUTTON_UP] = false;
			*curData.button[BATTLE_BUTTON_DOWN] = false;
			break;

		case 2:	// 上要素
			*curData.button[BATTLE_BUTTON_UP] = true;
			*curData.button[BATTLE_BUTTON_DOWN] = false;
			break;
	}

	curData.cntInputUpdated = *pBattleCnt;

	updateTrigger();
	updateHold();
	updateRelease();
}