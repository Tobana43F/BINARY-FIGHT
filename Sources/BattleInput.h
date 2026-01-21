//==========================================================
// バトル中に使用する入力クラス。
// リプレイ機能をつける際に使える
// 今は実装しない
#ifndef _BATTLE_INPUT_H
#define _BATTLE_INPUT_H

#include	"ShareBaseFunc.h"	// 入力関数
#include	"Constants.h"

//==========================================================
// バトル用入力クラスのボタン指定定数
enum eBattleButton{
	BATTLE_BUTTON_UP,
	BATTLE_BUTTON_DOWN,
	BATTLE_BUTTON_LEFT,
	BATTLE_BUTTON_RIGHT,
	BATTLE_BUTTON_1,
	BATTLE_BUTTON_2,
	BATTLE_BUTTON_A,
	BATTLE_BUTTON_B,
	BATTLE_BUTTON_PLUS,
	BATTLE_BUTTON_MINUS,
	MAX_BATTLE_BUTTON,
};

//==========================================================
// BattleInputData クラス
// リスト構造
class BattleInputData{
public:
	//----------------------------------------------------------
	// 実際の入力状態
	// シフト用
	enum eBitShift{
		SHIFT_UP,
		SHIFT_DOWN,
		SHIFT_LEFT,
		SHIFT_RIGHT,
		SHIFT_1,
		SHIFT_2,
		SHIFT_A,
		SHIFT_B,
		SHIFT_PLUS,
		SHIFT_MINUS,
	};
	// 連番ではない
	enum eRawInputMaskBit{
		MASK_UP = 0x0001,
		MASK_DOWN = 0x0002,
		MASK_LEFT = 0x0004,
		MASK_RIGHT = 0x0008,
		MASK_1 = 0x0010,
		MASK_2 = 0x0020,
		MASK_A = 0x0040,
		MASK_B = 0x0080,
		MASK_PLUS = 0x0100,
		MASK_MINUS = 0x0200,
	};
	unsigned int rawInputState;

	//----------------------------------------------------------
	// 方向キーの入力(右向きを参照)をテンキーに対応させたもの
	enum eArrowDirection{
		DIR_1 = 1,	// 数字を合わせる
		DIR_2,
		DIR_3,
		DIR_4,
		DIR_5,
		DIR_6,
		DIR_7,
		DIR_8,
		DIR_9,
	};
	int  directionKey;	// プレイヤー操作用
	
	bool *button[MAX_BATTLE_BUTTON];
	bool buttonUp;		// 方向キーのトリガーなどをとりたい場合
	bool buttonDown;
	bool buttonLeft;
	bool buttonRight;

	bool button1;
	bool button2;
	bool buttonA;
	bool buttonB;
	bool buttonPlus;
	bool buttonMinus;

	int	 cntInputUpdated;	// 入力が更新されたフレームを記憶する変数

	BattleInputData	*next;	// リストで管理

public:
	BattleInputData();
	virtual ~BattleInputData();
	
	// 初期化
	void init();
};

//==========================================================
// クラス定義
// Actorごとに持たせる
class BattleInputManager{
public:
	//----------------------------------------------------------
	// 使用するコントローラー
	inputNS::eControllerNumber	useControllerNo;

	enum eBattleInputType{
		TRIGGER,
		HOLD,
		RELEASE, 
		MAX_TYPE,
	};

	// 参照用
	int				*pBattleCnt;	// バトルのカウンタを参照する

	BattleInputData	curData;		// 現在の入力
	BattleInputData *pPrevData;		// 入力履歴の先頭データ(スタックとしてリストを作成する) image: pPrevData->next ... rear->NULL;
	bool isInputUpdated;

	// それぞれの入力状態用の変数
	BattleInputData *useData[MAX_TYPE];
	BattleInputData trigger;
	BattleInputData hold;
	BattleInputData release;

public:
	BattleInputManager();
	virtual ~BattleInputManager();

	//==========================================================
	// 必須の初期化
	void setUseControllerNo(inputNS::eControllerNumber _no){ useControllerNo = _no; }
	void setBattleCntPointer(int *_pBattleCnt){ pBattleCnt = _pBattleCnt; }

	//==========================================================
	// セッター
	// コントローラーの入力状態から、データを設定
	void updateRawInputState();			// 入力状態の更新
	void updateBattleInputState();		// 実際に使用する状態の更新

	void updateTrigger();				// 方向キーデータは使用しない
	void updateHold();
	void updateRelease();

	void checkInputUpdate();			// 入力状態の更新状態を取得

	//==========================================================
	// ゲッター
	int		getRawInputData(){ return curData.rawInputState; }
	int		getDirectionKeyState(){ return curData.directionKey; }
	bool	getBattleInputState(eBattleInputType _type, int _buttonNo);	// 今まで通り使える関数（const.hのボタンＮｏを使用する
	
	//==========================================================
	// リスト操作
	void freeAllBattleInputDataList();
	void addBattleInputDataList();

	//==========================================================
	// プログラムでのキー入力用関数
	void setButtonData(int _buttonNo, bool _val);		// ボタンデータを直接操作する
	void setDirectionKey(BattleInputData::eArrowDirection _val);

	//==========================================================
	// 基本的な関数
	void update();
};


#endif