//==========================================================
// 共通関数を定義

#include	"MyDefinitions.h"
#include	"Common.h"					// グローバル変数を扱うため
#include	"ShareBaseFunc.h"			// このCppで使用する関数のプロトタイプを宣言するため
#include	"Constants.h"				// 定数を扱うため
#include	"MySound.h"					// 音声関係の定数を使うため

#ifdef DIRECTX
#include	"input.h"
#include	"XAudio2.h"
#endif
#ifdef WII

#endif

#include	<stdio.h>					// sprintf , vsprintfなどを使うため	
#include	<stdarg.h>					// 可変引数を利用した関数を作るため

//==========================================================
// 基本的な、メイン処理の実行などにかかわる関数。
//==========================================================
// 描画関数を呼び出す関数、主に描画前処理を行う
void draw(){
#ifdef DIRECTX
	DrawD3D();
#endif
#ifdef WII

#endif
}

//==========================================================
// GXColorへの代入を共通に使えるようにする関数
void setMyGXColor(GXColor *dest, unsigned char r, unsigned char g, unsigned char b){
#ifdef DIRECTX
	*dest = D3DCOLOR_XRGB(r, g, b);
#endif
#ifdef WII

#endif
}

//----------------------------------------------------------
// GXColorから色情報を取得する関数
unsigned char getMyGXColor(GXColor *src, eColor col){
#ifdef DIRECTX
	switch (col){
		case RED:
			return (unsigned char)((*src & 0x00FF0000) >> 16);
		case GREEN:
			return (unsigned char)((*src & 0x0000FF00) >> 8);
		case BLUE:
			return (unsigned char)((*src & 0x000000FF));
		case ALPHA:
			return (unsigned char)((*src & 0xFF000000) >> 24);
	}

	return 0;	// 警告防止

#endif
#ifdef WII

#endif
}

//==========================================================
// 入力状態の取得
bool getInputState(inputNS::eType _type, int _button, int _no){
#ifdef DIRECTX
	switch (_type){
		case inputNS::HOLD:
			//return GetKeyboardPress(_button);
			return GetKeyboardPress(inputNS::BUTTON_ARRAY[_button][_no]);
			break;
		case inputNS::TRIGGER:
			//return GetKeyboardTrigger(_button);
			return GetKeyboardTrigger(inputNS::BUTTON_ARRAY[_button][_no]);
			break;
		case inputNS::RELEASE:
			//return GetKeyboardRelease(_button);
			return GetKeyboardRelease(inputNS::BUTTON_ARRAY[_button][_no]);
			break;
		default:
			return 0;
	}
#endif
#ifdef WII

#endif
}

//==========================================================
// テクスチャの読み込み(pathには拡張子を書かない)
// DirectXは.tga形式のみで、_DirectXフォルダに格納
bool loadTextureFromFile(const char *path, GXTexObj *tex, TPLPalettePtr *pal){
	char truePath[256];	// codeWarriorのdvddata内を参照するパスに合わせる
	if (path != NULL){
		stringCopy(truePath, FILE_PATH);
		stringAdd(truePath, FILE_TEXTURE);
		stringAdd(truePath, path);
		stringAdd(truePath,	TEXTURE_FILE_EXTENSION);
	}
	
#ifdef DIRECTX
	pal = NULL;	// 警告防止(関数内で使用されていない引数)

	// エラー判定
	if (FAILED(D3DXCreateTextureFromFile(g_pd3dDevice, truePath, tex)))
		return false;
	return true;
#endif
#ifdef WII

#endif // !WII
}

//==========================================================
// テクスチャ開放
void releaseTextureFromMemory(GXTexObj *tex, TPLPalettePtr *pal){
#ifdef DIRECTX
	pal = NULL;	// 警告防止
	IDirect3DTexture9_Release(*tex);	// 正しい？
	tex = NULL;
#endif
#ifdef WII

#endif
}


//==========================================================
// 使用できないスタンダードライブラリの代役
//----------------------------------------------------------
// strcpyの代役
void stringCopy(char *dest, const char *src){
	int i = 0;
	while (src[i] != '\0'){
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}

// strcatの代役
void stringAdd(char *dest, const char *src){
	int i = 0;
	int j = 0;
	while (dest[i] != '\0'){ ++i; }	// 文字列の最後まですすめる
	while (src[j] != '\0'){
		dest[i] = src[j];
		i++;
		j++;
	}
	dest[i] = '\0';
}

// strlenの代役(\0は含まない)
int stringLen(const char *src){
	int cnt;
	for (cnt = 0; src[cnt] != '\0'; ++cnt){}
	return cnt;
}

// strcmpの代役(アレンジ)
// 0:異なる　1:等しい
bool stringCompare(const char *src1, const char *src2){
	// 長さが異なればまず違う。
	if (stringLen(src1) != stringLen(src2))
		return false;

	// 文字列の比較
	for (int i = 0; i <= stringLen(src1); ++i){
		if (src1[i] != src2[i])
			return false;
	}

	return true;
}

//==========================================================
// ファイルアクセス
// _pDest　読み込み先ファイルポインタ
bool fileOpen(MY_FILE *_pDest, const char *_filePath, eFileOpenType _type){
#ifdef DIRECTX
	char buf[5];
	switch (_type){
	case FILE_READ:
		stringCopy(buf, "r");
		break;
	case FILE_WRITE:
		stringCopy(buf, "w");
		break;
	}

	// ファイルオープン
	_pDest->fp = fopen(_filePath, buf);	// ファイルのオープン
	if (_pDest->fp == NULL)				// エラー処理
		return false;

	// ファイルサイズの取得
	unsigned int fileSize;
	fseek(_pDest->fp, 0, SEEK_END);		// ファイル終端へ移動
	fileSize = ftell(_pDest->fp);			// カーソル位置(ファイルサイズ)の取得
	fseek(_pDest->fp, 0, SEEK_SET);		// ファイルの戦闘に戻す(後でそのまま使えるように)

	// ファイル内容格納バッファの確保
	_pDest->buf = new char[fileSize];

	// ファイルをバッファーにコピー
	for (int i = 0; (_pDest->buf[i] = fgetc(_pDest->fp)) != EOF; ++i){}

	fclose(_pDest->fp);	// ファイル処理の終了

	// バッファのカーソル初期化
	_pDest->cur = 0;

	fseek(_pDest->fp, 0, SEEK_SET);		// ファイルの戦闘に戻す(後でそのまま使えるように)

	return true;
#endif
#ifdef WII
#endif
}

// クローズ
bool fileClose(MY_FILE *_pFile){
#ifdef DIRECTX
	//fclose(_pFile->fp);	// ファイルクローズ
	delete[] _pFile->buf;	// バッファーの開放
	_pFile->buf = NULL;
	_pFile->cur = 0;		// カーソルの初期化
#endif
#ifdef WII
	
#endif
	return true;		// 判定を行わない
}


//----------------------------------------------------------
// 読込の一連の関数
// 読込に使用する関数
// 文字列用
void fileRead(MY_FILE *_pSrc, const char *format, char *_dest){
	fileScanf(_pSrc, format, _dest);		// 文字列用
	cntReadCharNum(_pSrc);
}

// int型用
void fileRead(MY_FILE *_pSrc, const char *format, int *_dest){
	fileScanf(_pSrc, format, _dest);
	cntReadCharNum(_pSrc);
}

// float型用
void fileRead(MY_FILE *_pSrc, const char *format, float *_dest){
	fileScanf(_pSrc, format, _dest);
	cntReadCharNum(_pSrc);
}


// 読み込み
// fileScanf()
void fileScanf(MY_FILE *_pSrc, const char *format, ...){
	va_list arg;
	va_start(arg, format);
	vsscanf(&_pSrc->buf[_pSrc->cur], format, arg);
	va_end(arg);
}


// 読み込んだ文字数を数える
void cntReadCharNum(MY_FILE *_pSrc){
	char c;
	while (true){
		c = fileGetChar(_pSrc);
		if (c == ','){			// , の場合は、そのまま読み進める。
			break;
		}
		else if (c == '\n'){	// 行の最終列のデータを読み込んだ後に、行の読み飛ばしを行うと、次の行が読み飛ばされるため、その防止策としてこれ。
			_pSrc->cur--;
			break;
		}
	}
}

//----------------------------------------------------------
// fgetc()の役割
char fileGetChar(MY_FILE *_pSrc){
	char tmp = _pSrc->buf[_pSrc->cur];	// 文字を取得
	if (tmp == EOF || tmp == '\0')
		return '\n';					// 判定処理の簡略化のため

	_pSrc->cur++;						// カーソルを進める
	return tmp;
}

void fileLineSkip(MY_FILE *_pSrc){
	while (fileGetChar(_pSrc) != '\n'){}
}

////////////////////////////////////////////////////////////
//==========================================================
// ポリゴン描画一連の処理
//==========================================================
// 頂点フォーマットの設定
void setVertexFormat(){
#ifdef DIRECTX
	g_pd3dDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);				// 頂点フォーマットを指定
#endif
#ifdef WII
	
#endif
}

//==========================================================
// テクスチャをセットする関数(描画直前に呼び出す)
void setTexture(GXTexObj tex){
#ifdef DIRECTX
	if (tex != NULL)
		g_pd3dDevice->SetTexture(0, tex);
	else 
		g_pd3dDevice->SetTexture(0, NULL);
#endif
#ifdef WII
#endif
}

//==========================================================
// 4頂点のポリゴンの描画
//----------------------------------------------------------
// ポリゴンデータから直接描画する関数
// アニメーションが登録されているテクスチャを設定
void drawQuadPrimitiveWithAnimation(Polygon2D *poly){
	int tx = 0, ty = 0;
	float texHLen = 1, texVLen = 1;

	int offsetTuLeft, offsetTuRight;	// tx+1の 1の部分(反転時の処理を簡潔にするため)
	int offsetTvTop, offsetTvBottom;

	//----------------------------------------------------------
	// ポリゴンのテクスチャのUVを反転
	if (poly->isReverseX == true){		// 左向
		offsetTuLeft = 1;
		offsetTuRight = 0;
	}
	else {								// 右向き
		offsetTuLeft = 0;
		offsetTuRight = 1;
	}
	if (poly->isReverseY == true){		// 下
		offsetTvTop = 1;
		offsetTvBottom = 0;
	}
	else {								// 上
		offsetTvTop = 0;
		offsetTvBottom = 1;
	}

	//----------------------------------------------------------
	// テクスチャ情報があるときのみアニメーション
	if (poly->pTex != NULL &&
		poly->pTex->anmData != NULL){
		tx = poly->pTex->anmData[poly->anmNo][poly->anmCnt] % poly->pTex->hDiv;		// 横の数での剰余
		ty = poly->pTex->anmData[poly->anmNo][poly->anmCnt] / poly->pTex->hDiv;		// 横の数の商
	}

	//----------------------------------------------------------
	// テクスチャがある場合はＵＶ座標を設定
	if (poly->pTex != NULL){
		texHLen = poly->pTex->hLen;
		texVLen = poly->pTex->vLen;
	}


	//----------------------------------------------------------
	// 4頂点の情報を格納
	float tmpX[4] = {
		poly->coord.x,
		poly->coord.x + poly->coord.w,
		poly->coord.x + poly->coord.w,
		poly->coord.x };
	float tmpY[4] = {
		poly->coord.y,
		poly->coord.y,
		poly->coord.y + poly->coord.h,
		poly->coord.y + poly->coord.h };
	float tmpTu[4] = {
		texHLen * (tx + offsetTuLeft),
		texHLen * (tx + offsetTuRight),
		texHLen * (tx + offsetTuRight),
		texHLen * (tx + offsetTuLeft)};
	float tmpTv[4] = {
		texVLen * (ty + offsetTvTop),
		texVLen * (ty + offsetTvTop),
		texVLen * (ty + offsetTvBottom),
		texVLen * (ty + offsetTvBottom),
	};
#ifdef DIRECTX
	//----------------------------------------------------------
	// DirectX専用の方に代入
	VERTEX2D tmpVX[4];

	for (int i = 0; i < 4; ++i){
		tmpVX[i].z = 0;
		tmpVX[i].rhw = 1;
		tmpVX[i].diffuse = poly->col;
		tmpVX[i].tx = tmpTu[i];
		tmpVX[i].x = tmpX[i];
		tmpVX[i].y = tmpY[i];
		tmpVX[i].ty = tmpTv[i];
	}
	g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, tmpVX, sizeof(VERTEX2D));		// ポリゴンを描画
#endif
#ifdef WII
#endif
}

//----------------------------------------------------------
// テクスチャアニメーションを使用しない場合の描画方法
// texNoで使用するテクスチャを指定する。
void drawQuadPrimitive(Polygon2D *poly, int texNo){
	drawQuadPrimitive(
		poly->coord.x, poly->coord.y,
		poly->coord.w, poly->coord.h,
		255, 255, 255,	// 後でcol情報からの指定に変更する
		poly->pTex,
		texNo
		);
}

//----------------------------------------------------------
// 値などを直接指定して、ポリゴンを描画する関数
void drawQuadPrimitive(float x, float y, float w, float h, unsigned char colR, unsigned char colG, unsigned char colB, TextureUV *pTex, int texNo){
	int tx = texNo % pTex->hDiv;		// 横の数での剰余
	int ty = texNo / pTex->hDiv;		// 横の数の商

	float tmpX[4] = {
		x,
		x + w,
		x + w,
		x };
	float tmpY[4] = {
		y,
		y,
		y + h,
		y + h };
	float tmpTu[4] = {
		pTex->hLen * tx,
		pTex->hLen * (tx + 1),
		pTex->hLen * (tx + 1),
		pTex->hLen * tx };
	float tmpTv[4] = {
		pTex->vLen * ty,
		pTex->vLen * ty,
		pTex->vLen * (ty + 1),
		pTex->vLen * (ty + 1),
	};
#ifdef DIRECTX
	VERTEX2D tmpVX[4];
	for (int i = 0; i < 4; ++i){
		tmpVX[i].x = tmpX[i];
		tmpVX[i].y = tmpY[i];
		tmpVX[i].z = 0;
		tmpVX[i].rhw = 1;
		setMyGXColor(&tmpVX[i].diffuse, colR, colG, colB);
		tmpVX[i].tx = tmpTu[i];
		tmpVX[i].ty = tmpTv[i];
	}
	setVertexFormat();
	setTexture(pTex->tex);
	g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, tmpVX, sizeof(VERTEX2D));		// ポリゴンを描画
#endif
#ifdef WII
#endif
}

//----------------------------------------------------------
// テクスチャのUV値の補正値を指定して描画を行う関数
// テクスチャの補正は左上の座標を基準に行う。
// 注意：補正値は、0.0~1.0の値で行う。
//		 アニメーションがあり、テクスチャを分割していた場合でも
//       一枚と想定した補正値を渡すこと。(例)半分にする場合はテクスチャの分割にかかわらず 0.5f)
void drawQuadPrimitive(Polygon2D *poly, float tx0, float tx1, float tx2, float tx3, float ty0, float ty1, float ty2, float ty3){
	int tx = 0, ty = 0;
	float texHLen = 1, texVLen = 1;

	// 指定された引数の値を、分割された状態の値に補正する
	tx0 /= poly->pTex->hDiv;
	tx1 /= poly->pTex->hDiv;
	tx2 /= poly->pTex->hDiv;
	tx3 /= poly->pTex->hDiv;

	ty0 /= poly->pTex->vDiv;
	ty1 /= poly->pTex->vDiv;
	ty2 /= poly->pTex->vDiv;
	ty3 /= poly->pTex->vDiv;

	//----------------------------------------------------------
	// テクスチャ情報があるときのみアニメーション
	if (poly->pTex != NULL &&
		poly->pTex->anmData != NULL){
		tx = poly->pTex->anmData[poly->anmNo][poly->anmCnt] % poly->pTex->hDiv;		// 横の数での剰余
		ty = poly->pTex->anmData[poly->anmNo][poly->anmCnt] / poly->pTex->hDiv;		// 横の数の商
	}

	//----------------------------------------------------------
	// テクスチャがある場合はＵＶ座標を設定
	if (poly->pTex != NULL){
		texHLen = poly->pTex->hLen;
		texVLen = poly->pTex->vLen;
	}

	//----------------------------------------------------------
	// 4頂点の情報を格納
	float tmpX[4] = {
		poly->coord.x,
		poly->coord.x + poly->coord.w,
		poly->coord.x + poly->coord.w,
		poly->coord.x };
	float tmpY[4] = {
		poly->coord.y,
		poly->coord.y,
		poly->coord.y + poly->coord.h,
		poly->coord.y + poly->coord.h };
	float tmpTu[4] = {
		(texHLen * (tx)) + (tx0),
		(texHLen * (tx)) + (tx1),
		(texHLen * (tx)) + (tx2),
		(texHLen * (tx)) + (tx3) };
	float tmpTv[4] = {
		(texVLen * (ty)) + (ty0),
		(texVLen * (ty)) + (ty1),
		(texVLen * (ty)) + (ty2),
		(texVLen * (ty)) + (ty3), };

#ifdef DIRECTX
	//----------------------------------------------------------
	// DirectX専用の方に代入
	VERTEX2D tmpVX[4];

	for (int i = 0; i < 4; ++i){
		tmpVX[i].z = 0;
		tmpVX[i].rhw = 1;
		tmpVX[i].diffuse = poly->col;
		tmpVX[i].tx = tmpTu[i];
		tmpVX[i].x = tmpX[i];
		tmpVX[i].y = tmpY[i];
		tmpVX[i].ty = tmpTv[i];
	}
	setVertexFormat();
	setTexture(poly->pTex->tex);
	g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, tmpVX, sizeof(VERTEX2D));		// ポリゴンを描画
#endif
#ifdef WII
#endif
}

//----------------------------------------------------------
// 全ての座標を指定して、ポリゴンを描画する関数(矩形以外も描画可能)
void drawQuadPrimitive(
	float x0, float y0,
	float x1, float y1,
	float x2, float y2,
	float x3, float y3,
	unsigned char colR, unsigned char colG, unsigned char colB,
	TextureUV *pTex, int texNo)
{
	int tx = texNo % pTex->hDiv;		// 横の数での剰余
	int ty = texNo / pTex->hDiv;		// 横の数の商

	float tmpX[4] = {x0,x1,x2,x3 };
	float tmpY[4] = {y0,y1,y2,y3 };
	float tmpTu[4] = {
		pTex->hLen * tx,
		pTex->hLen * (tx + 1),
		pTex->hLen * (tx + 1),
		pTex->hLen * tx };
	float tmpTv[4] = {
		pTex->vLen * ty,
		pTex->vLen * ty,
		pTex->vLen * (ty + 1),
		pTex->vLen * (ty + 1),
	};
#ifdef DIRECTX
	VERTEX2D tmpVX[4];
	for (int i = 0; i < 4; ++i){
		tmpVX[i].x = tmpX[i];
		tmpVX[i].y = tmpY[i];
		tmpVX[i].z = 0;
		tmpVX[i].rhw = 1;
		setMyGXColor(&tmpVX[i].diffuse, colR, colG, colB);
		tmpVX[i].tx = tmpTu[i];
		tmpVX[i].ty = tmpTv[i];
	}
	setVertexFormat();
	setTexture(pTex->tex);
	g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, tmpVX, sizeof(VERTEX2D));		// ポリゴンを描画
#endif
#ifdef WII
#endif
}

//==========================================================
// ビットマップフォントで文字列を表示する関数
//----------------------------------------------------------
//  255バイトまでの文字をポリゴンで表示できる。
void printfText(float x, float y, float w, float h, unsigned char colR, unsigned char colG, unsigned char colB, TextureUV *pTex, const char *format, ...){
	va_list arg;
	char buf[256] = { "\0" };			// 動的確保だと、フォーマットと置き換える前のサイズでとってしまうので、置き換えたときにバッファが足りなくなる。

	va_start(arg, format);
	vsprintf(buf, format, arg);	
	va_end(arg);

	for (int i = 0; buf[i] != '\0'; ++i){
		drawQuadPrimitive(
			x + (i*w), y, w, h,
			colR, colG, colB,
			pTex, buf[i] - ' ');
	}
}

//==========================================================
// サウンド関係
// サウンドの再生を行う
void playSound(soundNS::SOUND_LABEL index){
#ifdef DIRECTX
	PlaySound(index);
#endif
#ifdef WII
#endif
}

// サウンドの停止を行う
void stopSound(soundNS::SOUND_LABEL index){
#ifdef DIRECTX
	StopSound(index);
#endif
#ifdef WII
#endif
}

void pauseSound(soundNS::SOUND_LABEL index){
#ifdef DIRECTX
	PauseSound(index);
#endif
#ifdef WII
#endif
}