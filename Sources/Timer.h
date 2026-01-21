//==========================================================
// フレームレートの制御を行うクラス

#ifndef _TIMER_H
#define	_TIMER_H

//==========================================================
// インクルード
#include	<Windows.h>										// LARGE_INTEGER型を使用するため
#include	<mmsystem.h>									// timeBeginPeriod()等を使用するため
#pragma		comment(lib, "winmm.lib")						// 〃

#include	"Constants.h"	// FRAME_RATEの定数を使用するため

//==========================================================
// 定数定義

namespace timerNS{
	const int	MILLISEC_PER_SEC = 1000;					// 秒間毎のミリ秒(そのままの意味)
	const float	FRAME_RATE = DEF_FRAME_RATE;							// フレームレート
	const float	MIN_FRAME_RATE = FRAME_RATE * 0.8f;				// 最小のフレームレート
	const float	MIN_FRAME_TIME = 1.0f / FRAME_RATE;			// 1フレーム辺りの時間
	const float	MAX_FRAME_TIME = 1.0f / MIN_FRAME_RATE;		// 1フレーム辺りの最大の時間
}

//==========================================================
// クラス
class Timer{
private:
	LARGE_INTEGER	start;			// 計測開始時間
	LARGE_INTEGER	end;			// 計測終了時間(現在時間)
	LARGE_INTEGER	freq;			// CPUの周波数(Hz)
	float			frameTime;		// 1フレーム毎の経過時間
	float			fps;			// frame per second
	DWORD			sleepTime;		// CPUを開放する時間を格納する

public:
	Timer();
	virtual ~Timer();

	bool frameControl();
	float getFPS();
};

#endif // !_TIMER_H

