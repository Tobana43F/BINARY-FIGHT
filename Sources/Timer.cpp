//==========================================================
// フレームレートの制御で使用するクラスの定義
#include	"MyDefinitions.h"

#ifdef DIRECTX
#include	"Timer.h"	// Timerクラスの使用、Timerクラスで使用するヘッダのインクルードのため

using namespace timerNS;

//==========================================================
// コンストラクタ
Timer::Timer(){
	ZeroMemory(this, sizeof(Timer));						// 初期化

	if (!QueryPerformanceFrequency(&freq)){
		// エラーの処理(高分解能タイマーを使用できない)
		exit(1);
	}
}

//==========================================================
// デストラクタ
Timer::~Timer(){
	// 処理
}

//==========================================================
// フレームレートの制御
// フレームレートをあわせるため、処理をスリープ(CPUを開放)させる
// 戻り値 : true 更新が必要
//        : false 更新が不要
bool Timer::frameControl(){
	QueryPerformanceCounter(&end);								// 現在時間を取得

	// 前フレームからの経過時間を取得し、frameTime に格納
	// 周波数で出るので、(経過周波数)÷(1秒間の周波数) で経過時間(ms)を求めている
	frameTime = (float)(end.QuadPart - start.QuadPart) / (float)freq.QuadPart;

	// 節電コード、winnmm.libが必要
	// 経過時間が、1フレームに必要な最小の時間に満たない場合
	if (frameTime < MIN_FRAME_TIME){
		sleepTime = (DWORD)((MIN_FRAME_TIME - frameTime) * MILLISEC_PER_SEC);
		timeBeginPeriod(1);										// CPUの分解能を1msに設定
		Sleep(sleepTime);										// sleepTime分 CPUを開放(1フレームに必要な残り秒数を経過させる)
		timeEndPeriod(1);										// timeBeginPeriod(1)の後処理
		
		return false;
	}
	
	if (frameTime > MAX_FRAME_TIME)								// 1フレームの最大時間を超えていた場合
		frameTime = MAX_FRAME_TIME;								// 最大時間に制限する

	if (frameTime > 0.0f)
		fps = (fps * 0.99f) + (0.01f / frameTime);				// 平均fps



	start = end;												// 時間を更新

	return true;
}

//==========================================================
// FPSを取得
float Timer::getFPS(){
	return (fps);
}

#endif