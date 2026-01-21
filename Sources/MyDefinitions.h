//==========================================================
// 内容:
//----------------------------------------------------------
// 変数の互換性を取るために、型を再定義し、共通の名前にする。
// 一方のみで使用する構造体などを定義
// DirectXの型をWii側にあわせる。
// 注意：全ソース、ヘッダに内容を知らせる必要がある。
// 全体で使用する定数などの定義
#ifndef _MY_DEFINITIONS_H
#define	_MY_DEFINITIONS_H

//==========================================================
// DirectX,Wii 切り替えマクロ
#define DIRECTX			// この行をコメントアウトすると、Wii用プロジェクト
#ifndef DIRECTX
#define	WII
#endif

//==========================================================
// インクルード
#ifdef DIRECTX
#include	<Windows.h>					// 型を再定義するため
#include	<d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include	<stdio.h>					// FILE構造体を再定義するため。

//#if _DEBUG
//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>
//#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#endif

#endif

#ifdef WII
#endif

//==========================================================
// 定義
//----------------------------------------------------------
// 再定義
#ifdef DIRECTX
typedef D3DCOLOR				GXColor;	// 色情報を格納する型
typedef LPDIRECT3DTEXTURE9		GXTexObj;	// テクスチャを格納しておく型(使い方が若干異なる(構造体ポインタ))

typedef struct
{
	// 空の構造体にしておく
} TPLPalette, *TPLPalettePtr;

#endif
#ifdef WII
#endif

//----------------------------------------------------------
// 構造体

// ファイルの読み込みを行うための構造体
struct MY_FILE{
#ifdef DIRECTX
	FILE			*fp;	// ファイルポインタ(DIRECTX)
	char			*buf;	// 配列確保先
#endif
#ifdef WII
#endif
	unsigned int	cur;	// 読込先カーソル(bufの添え字(ファイルの位置指定子の役割))
};

#endif