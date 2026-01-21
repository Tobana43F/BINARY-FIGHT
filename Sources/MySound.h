//==========================================================
// 音声関係の定数の定義などを行う
#ifndef _MY_SOUND_H
#define _MY_SOUND_H

//==========================================================
// サウンド用のネームスペースの定義
namespace soundNS{
	//==========================================================
	// 定数定義
	//----------------------------------------------------------
	// サウンドのインデックスの定義(再生時にこれを使用する)
	enum SOUND_LABEL
	{
		STAGE_SKYPLACE = 0,		// サンプルBGM
		STAGE_TRAINING,

		TEST_HIT_SE,
		TEST_BLOCK_SE,
		
		TITLE_BGM,
		TITLE_ENTER_SE,

		MODE_SELECT_BGM,

		CHARACTER_SELECT_BGM,
		CHARACTER_SELECT_DECIDE_SE,
		CHARACTER_SELECT_CANCEL_SE,

		GAMEREADY_START_SE,
		GAMEREADY_END_SE,

		CURSOR_SELECT_SE,
		DECIDE_GENERAL_SE,
		MANUAL_SELECT_SE,

		SOUND_LABEL_MAX,			// 使用する音源の最大数
	};
	
	//==========================================================
	// 構造体･クラスの定義
	//----------------------------------------------------------
	// 音声の再生に使用するクラス
	class MySoundInfo{
	private:
		char			filePath[256];		// ファイルへのパス名(共通)(DirectX:const char*(LPCSTR))
		bool			flgLoop;			// ループフラグ(DirectXのみ)
		
	public:
		unsigned int	handle;				// そのサウンドへのハンドラー
	
		MySoundInfo();
		MySoundInfo(char *fileName, bool _flgLoop);	// 初期化用
		virtual ~MySoundInfo();

		char* getFilePath();
		bool  getLoopFlag();
	};


	//----------------------------------------------------------
	// グローバル変数のエクスターン
	extern MySoundInfo FN_SOUNDS[];
}

#endif