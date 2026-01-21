//==========================================================
// デバッグじ専用関数を定義

#include	"MyDefinitions.h"	// DirectX,Wiiを切り替えるため

#ifdef DIRECTX
#include	<Windows.h>			// MessageBoxを使用するため
#include	<stdlib.h>			// exit関数を使用するため(注意:Win32でコンソール系の関数を使用するのはあまりよくない)
#endif
#ifdef WII
#endif

#include	"Debug.h"			// このCppで使用する関数のプロトタイプを宣言するため

#define ERROR_WINDOW_TITLE		"error"

//==========================================================
// エラーメッセージの表示と強制終了
void outErrorMessage(char *errMsg){
#ifdef DIRECTX
	MessageBox(NULL, errMsg, ERROR_WINDOW_TITLE, MB_OK);
	exit(1);
#endif // !DIRECTX
#ifdef WII
#endif // !WII
}
