//==========================================================
// 共通関数のプロトタイプ
#ifndef _SHARE_BASE_FUNC
#define	_SHARE_BASE_FUNC

#include	"MyDefinitions.h"		// WII,DirectX用に再定義した型名を使用するため
#include	"Constants.h"			// inputNS:: の定数をプロトタイプの引数で使用するため
#include	"Polygon.h"				// Polygon2Dを扱うプロトタイプを書くため
#include	"MySound.h"				// サウンドラベルを引数にするため

//==========================================================
// 定数定義
//----------------------------------------------------------
// getMyGXColorで使用する
enum eColor{
	RED = 0,
	GREEN,
	BLUE,
	ALPHA,
};

// ファイルの開き方の指定
enum eFileOpenType{
	FILE_READ,
	FILE_WRITE,
};


//==========================================================
// プリプロセッサ
#define safeDelete(p)	if(p != NULL){delete p; p = NULL;}


//==========================================================
// プロトタイプ宣言
//----------------------------------------------------------
// 共通の関数のプロトタイプ宣言
// 基本的な処理の共通化を行う関数
void initBeforeMainLoop(void);	// 共通の初期化処理
void destroy(void);				// 共通の解放処理
void run(void);					// それぞれのメインループ内で呼び出す
void draw(void);				// 描画関数の呼び出し。
void render(void);				// GXBegin()～GXEnd　等、ここの描画関数内で呼び出す。
void update(void);				// run内で実行

// 入力状態の取得を共通に行う
bool getInputState(inputNS::eType _type, int _button, int _no = 0);

// 型の違う変数への代入などの共通化をする関数
void setMyGXColor(GXColor *dest, unsigned char r, unsigned char g, unsigned char b);			// GXColorへの代入の共通関数
unsigned char getMyGXColor(GXColor *src, eColor col);
bool loadTextureFromFile(const char *path, GXTexObj *tex, TPLPalettePtr *pal);				// テクスチャの読み込み
void releaseTextureFromMemory(GXTexObj *tex, TPLPalettePtr *pal);		// テクスチャ開放

// 使えない基本機能の代役をする関数
void stringCopy(char *dest,const  char *src);
void stringAdd(char *dest, const char *src);
int stringLen(const char *src);
bool stringCompare(const char *src1, const char *src2);

//----------------------------------------------------------
// ファイルアクセス
bool fileOpen(MY_FILE *_pDest, const char *_filePath, eFileOpenType _type);
bool fileClose(MY_FILE *_pFile);

void fileRead(MY_FILE *_pSrc, const char *format, char *_dest);				// ファイル読み取り
void fileRead(MY_FILE *_pSrc, const char *format, int *_dest);				// オーバーロード
void fileRead(MY_FILE *_pSrc, const char *format, float *_dest);			// オーバーロード

//void fileScanf(MY_FILE *_pSrc, const char *format, ...);	// 現状、fileRead()以外からの呼び出しは禁止。
void fileScanf(MY_FILE *_pSrc, const char *format, ...);

void cntReadCharNum(MY_FILE *_pSrc);						// 〃
	
char fileGetChar(MY_FILE *_pSrc);
void fileLineSkip(MY_FILE *_pSrc);	// 行の読み飛ばし。

//----------------------------------------------------------
// ポリゴン描画の一連の処理
void setVertexFormat();
void setTexture(GXTexObj tex);
void drawQuadPrimitiveWithAnimation(Polygon2D *poly);
void drawQuadPrimitive(Polygon2D *poly, int texNo);
void drawQuadPrimitive(float x, float y, float w, float h, unsigned char colR, unsigned char colG, unsigned char colB, TextureUV *pTex, int texNo);
void drawQuadPrimitive(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, unsigned char colR, unsigned char colG, unsigned char colB, TextureUV *pTex, int texNo);
void drawQuadPrimitive(Polygon2D *poly, float tx0, float tx1, float tx2, float tx3, float ty0, float ty1, float ty2, float ty3);
void drawQuadPrimitive(float x, float y, float w, float h, unsigned char colR, unsigned char colG, unsigned char colB, TextureUV *pTex, int texNo, float tx0, float tx1, float tx2, float tx3, float ty0, float ty1, float ty2, float ty3);
void setVertex2DFromPolygon2D(VERTEX2D dest[4], Polygon2D *src);
void printfText(float x, float y, float w, float h, unsigned char colR, unsigned char colG, unsigned char colB, TextureUV *pTex, const char *format, ...);

// 音声関係の処理
void playSound(soundNS::SOUND_LABEL index);
void stopSound(soundNS::SOUND_LABEL index);
void pauseSound(soundNS::SOUND_LABEL index);

#endif