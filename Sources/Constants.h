//==========================================================
// 定数を定義するヘッダ
#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#include	"MyDefinitions.h"

//==========================================================
// インクルード
#ifdef DIRECTX
#include	"input.h"		// DirectX用の入力定数を定義するため
#endif
#ifdef WII	
#endif

//==========================================================
// 定数定義
//==========================================================
// システム上の定数を定義
// DirectXのウィンドウのタイトル名など
#define	NAME			"win32A"
#define	TITLE			"BINARY FIGHT"

// ファイルへのパス、ファイルの拡張子
#ifdef DIRECTX
// 画像や音声などのリソースがあるファイルまでのパス
#define	FILE_PATH					"_Resource/"

#define	TEXTURE_FILE_EXTENSION		".tga"	// 画像の拡張子
#define SOUND_FILE_EXTENSION		".wav"	// 音声の拡張子
#define ANIMATION_FILE_EXTENSION	".anm"	// アニメーションパターンの拡張子(独自)
#define CSV_FILE_EXTENSION			".csv"	// CSVファイルの拡張子


#endif
#ifdef WII
#endif

// フォルダへのパス
#define FILE_TEXTURE	"Texture/"	
#define FILE_SOUND		"Sound/"
#define FILE_ANIMATION	"Animation/"
#define FILE_HITBOX		"HitBox/"


//----------------------------------------------------------
// ウィンドウ関係
// 基準となるウィンドウサイズ(こっちは変更しない)
#define DEF_WINDOW_WIDTH	(640)
#define DEF_WINDOW_HEIGHT	(480)

// ウィンド(スクリーンの幅と高さ)(こっちが実際のサイズ)
#define	WINDOW_WIDTH	(640)
#define	WINDOW_HEIGHT	(480)

// 基準のウィンドウサイズとの比率
#define WINDOW_WIDTH_RATIO	(WINDOW_WIDTH / DEF_WINDOW_WIDTH)
#define WINDOW_HEIGHT_RATIO	(WINDOW_HEIGHT / DEF_WINDOW_HEIGHT)

// フレームレート
#define DEF_FRAME_RATE	(60.0f)

// デフォルト背景カラー
#define	DEF_BACK_COLOR_R (127)
#define	DEF_BACK_COLOR_G (127)
#define	DEF_BACK_COLOR_B (127)

// ゲームシーンの定義
enum eGameScene{
	// 画面がある遷移
	TITLE_SCENE,
	MODE_SELECT,			// メインメニューに当たる　…モード選択画面、オプションなど

	GS_ARCADE_MODE,
	GS_VERSUS_MODE,
	GS_TUTORIAL_MODE,
	GS_MANUAL_MODE,

	CHARACTER_SELECT,		// キャラ選択
	STAGE_SELECT,			// ステージ選択
	GAME_READY_SCENE,		// キャラクター、ステージのテクスチャのロードなどはここで行う。
	GAMEPLAY_SCENE,			// 戦闘
	RESULT_SCENE,			// アーケードモードの終了

	// 中継用遷移
	RETURN_TO_TITLE,
	RETURN_TO_MODE_SELECT,
};

// 純粋仮想関数をわかりやすくするため
#define PURE_FUNC	=0

//==========================================================
// 数学関係
// 円周率
#define	PI				(3.1415926535897931f)
// 重力加速度
#define	GRAVITY			(0.98f)

//==========================================================
// 入力関係の定数
// 入力状態の種類
namespace inputNS{
	enum eType{
		HOLD = 0,
		TRIGGER,
		RELEASE,
	};

	// コントローラー番号
	enum eControllerNumber{
		NO1 = 0,
		NO2,
		NO3,
		NO4,
		PROGRAM,	// プログラムで制御するときに使用する。
		NO_MAX,
	};

	// ボタンの割り当て
#ifdef DIRECTX

#define BUTTON_UP		(0)
#define BUTTON_DOWN		(1)
#define	BUTTON_LEFT		(2)
#define	BUTTON_RIGHT	(3)
#define	BUTTON_1		(4)
#define	BUTTON_2		(5)
#define	BUTTON_A		(6)
#define	BUTTON_B		(7)
#define	BUTTON_PLUS		(8)
#define	BUTTON_MINUS	(9)

#ifdef MAIN_MODULE
	const unsigned char	BUTTON_UP_ARRAY[NO_MAX] = { DIK_W, DIK_U, 0, 0 };
	const unsigned char	BUTTON_DOWN_ARRAY[NO_MAX] = { DIK_S, DIK_J, 0, 0 };
	const unsigned char	BUTTON_LEFT_ARRAY[NO_MAX] = { DIK_A, DIK_H, 0, 0 };
	const unsigned char	BUTTON_RIGHT_ARRAY[NO_MAX] = { DIK_D, DIK_K, 0, 0 };
	const unsigned char	BUTTON_1_ARRAY[NO_MAX] = { DIK_F, DIK_L, 0, 0 };
	const unsigned char	BUTTON_2_ARRAY[NO_MAX] = { DIK_G, DIK_SEMICOLON, 0, 0 };
	const unsigned char	BUTTON_A_ARRAY[NO_MAX] = { DIK_R, DIK_O, 0, 0 };
	const unsigned char	BUTTON_B_ARRAY[NO_MAX] = { DIK_T, DIK_P, 0, 0 };
	const unsigned char	BUTTON_PLUS_ARRAY[NO_MAX] = { DIK_E, DIK_I, 0, 0 };
	const unsigned char	BUTTON_MINUS_ARRAY[NO_MAX] = { DIK_Q, DIK_Y, 0, 0 };
	const unsigned char	*BUTTON_ARRAY[] = {
		BUTTON_UP_ARRAY,
		BUTTON_DOWN_ARRAY,
		BUTTON_LEFT_ARRAY,
		BUTTON_RIGHT_ARRAY,
		BUTTON_1_ARRAY,
		BUTTON_2_ARRAY,
		BUTTON_A_ARRAY,
		BUTTON_B_ARRAY,
		BUTTON_PLUS_ARRAY,
		BUTTON_MINUS_ARRAY,
	};
#else
	// externの指定
	extern const unsigned char *BUTTON_ARRAY[];

#endif	// !MAIN_MODULE
#endif	// !DIRECTX
#ifdef WII
#endif	// !WII
}


//==========================================================
// ゲーム内定数
//----------------------------------------------------------
// 当り判定ボックスの色 
#define HURTBOX_COLOR_R	(127)
#define HURTBOX_COLOR_G	(255)
#define HURTBOX_COLOR_B (127)

#define BODYBOX_COLOR_R (255)
#define BODYBOX_COLOR_G (255)
#define BODYBOX_COLOR_B	(127)

#define ATTACKBOX_COLOR_R (255)
#define ATTACKBOX_COLOR_G (127)
#define ATTACKBOX_COLOR_B (127)

//#define DEBUG_SHOW_HITBOX			// 当り判定を表示するかどうか
//#define DEBUG_SHOW_PLAYER_INFO		// プレイヤーのデバッグ情報を表示するかどうか

#endif	// !_CONSTANTS_H