//==========================================================
// ポリゴンクラスの処理を記述する

//==========================================================
// インクルード
#define		_CRT_SECURE_NO_WARNINGS

#include	"Polygon.h"
#include	"ShareBaseFunc.h"
#include	"MyTexture.h"
#include	"MyAnimation.h"
#include	"Debug.h"
#include	"TextureLoader.h"
#include	"AnimationLoader.h"
#include	"Constants.h"

#include	<stdio.h>	// FILE

//==========================================================
// TextureUV
//----------------------------------------------------------
// コンストラクタ
TextureUV::TextureUV(){
	pTexPal = NULL;
	hLen = 1.0f;
	vLen = 1.0f;
	hDiv = 1;
	vDiv = 1;
	hSize = 1;
	vSize = 1;
	hDivSize = 1;
	vDivSize = 1;
	anmData = NULL;
	anmNum = 0;
}

//----------------------------------------------------------
// デストラクタ
TextureUV::~TextureUV(){
	releaseAnmData();
	releaseTexture();
}

//----------------------------------------------------------
// テクスチャの読み込み
// テクスチャ情報クラスからの読み取り
void TextureUV::loadTexture(textureNS::eTextureLabel _label){
	loadTexture(
		textureNS::TEXTURE_INFO[_label].getFilePath(),
		textureNS::TEXTURE_INFO[_label].getHDiv(),
		textureNS::TEXTURE_INFO[_label].getVDiv(),
		textureNS::TEXTURE_INFO[_label].getHSize(),
		textureNS::TEXTURE_INFO[_label].getVSize()
		);
}

// テクスチャの一部を読み込む場合
void TextureUV::loadTexture(const char *path, int _hDiv, int _vDiv, unsigned short _hSize, unsigned short _vSize){
	if (!loadTextureFromFile(path, &tex, &pTexPal)){
		char buf[256];
		stringCopy(buf, path);
		stringAdd(buf, "ファイルの読込にしました。(TextureUV::loadTexture関数内)");
		outErrorMessage(buf);
	}

	hDiv = _hDiv;					// 分割数を設定
	vDiv = _vDiv;
	hLen = (float)(1.0f / hDiv);	// 一辺あたりの長さを設定
	vLen = (float)(1.0f / vDiv);

	hSize = _hSize;					// 画像のサイズを設定
	vSize = _vSize;
	hDivSize = (float)(_hSize / (float)_hDiv);		// テクスチャ１つあたりのサイズを設定(分割後のサイズ)
	vDivSize = (float)(_vSize / (float)_vDiv);

	//anmData = NULL;
}

//----------------------------------------------------------
// テクスチャ開放
void TextureUV::releaseTexture(){
	releaseTextureFromMemory(&tex, &pTexPal);
	
	hDiv = 1;
	vDiv = 1;
	hLen = 1.0f;
	vLen = 1.0f;
	hSize = 0;
	vSize = 0;
}

//----------------------------------------------------------
// アニメーションの読込
void TextureUV::loadAnmData(animationNS::eAnimationLabel _label){
	PatternLoader	*patTop;
	AnimationLoader *anmTop;
	MY_FILE			file;
	char			lineHeader[256];	// 行の先頭列のヘッダ情報の読込先

	//==========================================================
	// アニメーションファイルのオープン
	if(!fileOpen(&file, animationNS::ANIMATION_INFO[_label].getFilePath(), FILE_READ)){ 
		outErrorMessage("TextureUV::loadAnmData関数内で、アニメーションファイルのオープンに失敗しました。");
	}

	//==========================================================
	// リストの先頭にダミーデータを確保
	patTop = new PatternLoader;		// 読込先のリストの先頭にダミーデータを確保
	anmTop = new AnimationLoader;
	anmTop->anmPat = NULL;			// エラー防止

	//==========================================================
	// データ数の読込
	// 先頭列は説明なので、無視
	fileRead(&file, "%[^,]", lineHeader);	// 先頭列の読み飛ばし
	fileRead(&file, "%d", &this->anmNum);			// データ数の読込(読み込み処理を簡略化するため)
	fileLineSkip(&file);

	//==========================================================
	// 実際のパターンの読込
	PatternLoader	*patCur = patTop;
	AnimationLoader *anmCur = anmTop;

	for (int i = 0; i < this->anmNum; ++i){
		fileRead(&file, "%[^,]", lineHeader);		// 先頭列の読み飛ばし

		// パターンの終端子まで繰り返す
		do{
			patCur->next = new PatternLoader;	// 次のメモリを確保
			patCur = patCur->next;				// 確保したメモリに移動
			patCur->next = NULL;				// 次ポインタにNULLを代入し、リストの終端とする
			fileRead(&file, "%d", &patCur->anmNo);	// データの読込
		} while (patCur->anmNo != ANM_END && patCur->anmNo != ANM_LOOP && patCur->anmNo != ANM_NULL);

		fileLineSkip(&file);

		anmCur->next = new AnimationLoader;		// 次メモリを確保
		anmCur = anmCur->next;					// 確保したメモリに移動
		anmCur->next = NULL;

		anmCur->anmPat = patTop;				// 読み込んできたパターンを代入

		// 余分にメモリを確保しないため
		if (i == this->anmNum - 1)
			break;

		patTop = new PatternLoader;				// 別の先頭を確保
		patCur = patTop;						// その先頭を現在の参照先に変更
	}
	fileClose(&file);			// ファイルをクローズ

	//==========================================================
	// 最終的に必要なクラスに読み込む、準備をする(メモリの確保)
	int patLen;	// パターンの長さ(anmNoの数)
	int anmNumFnc;	// アニメーションの数(patternの数)

	//----------------------------------------------------------
	// アニメーションの数をカウント
	// 先に二次元配列を確保するため
	anmNumFnc = 0;
	for (anmCur = anmTop->next; anmCur != NULL; anmCur = anmCur->next)
		anmNumFnc++;

	anmData = new int*[anmNumFnc];	// ポインタ配列を確保(配列を確保するための先頭アドレス)

	//----------------------------------------------------------
	// パターンを入れられる配列を確保
	int i, j;
	i = 0;
	for (anmCur = anmTop->next; anmCur != NULL; anmCur = anmCur->next){
		patLen = 0;
		for (patCur = anmCur->anmPat->next; patCur != NULL; patCur = patCur->next){
			if (patCur->anmNo == ANM_NULL){// 空の場合は加算しない(0にしておく)
				patLen = 0;
				break;
			}

			patLen++;
		}
		if (patLen != 0){
			anmData[i] = new int[patLen];	// 配列を確保
		} else{
			anmData[i] = NULL;	// 空にする
		}
		i++;
	}

	//==========================================================
	// 確保した配列に、実際にデータを入れる。
	i = 0;
	for (anmCur = anmTop->next; anmCur != NULL; anmCur = anmCur->next){
		j = 0;
		for (patCur = anmCur->anmPat->next; patCur != NULL; patCur = patCur->next){
			if (patCur->anmNo != ANM_NULL){
				anmData[i][j] = patCur->anmNo;
				j++;
			}
		}
		i++;
	}

	//----------------------------------------------------------
	// パターンの開放
	for (anmCur = anmTop; anmCur != NULL; anmCur = anmCur->next){
		patCur = anmCur->anmPat;
		PatternLoader *pPrev = patCur;
		while (patCur != NULL){
			pPrev = patCur;
			patCur = patCur->next;

			delete pPrev;
			pPrev = NULL;
		}
	}

	//----------------------------------------------------------
	// 一時読込用リストの開放
	AnimationLoader *aPrev;
	anmCur = anmTop;
	while (anmCur != NULL){
		aPrev = anmCur;
		anmCur = anmCur->next;

		delete aPrev;
		aPrev = NULL;
	}

}

//----------------------------------------------------------
// アニメーションの開放
void TextureUV::releaseAnmData(){
	if (anmData == NULL)
		return;
	
	for (int i = 0; i < anmNum; ++i){
		delete[] anmData[i];
		anmData[i] = NULL;
	}

	delete[] anmData;
	anmData = NULL;
}

//----------------------------------------------------------
// 現在のテクスチャを取得
int TextureUV::getAnmNo(int anmNo, int anmCnt){
	if (anmData != NULL)
		return (anmData[anmNo][anmCnt]);
	return 0;
}

//----------------------------------------------------------
// テクスチャの分割数の縦横比率を取得
// 横
float TextureUV::getHDivSizeRatio(){
	return (hDivSize / vDivSize);
}

// 縦
float TextureUV::getVDivSizeRatio(){
	return (vDivSize / hDivSize);
}

//==========================================================
// Polygon2D
//==========================================================
// 関数定義
Polygon2D::Polygon2D(){
	// 座標情報初期化
	coord.x = 0;
	coord.y = 0;
	coord.h = 0;
	coord.w = 0;

	pTex = NULL;

	// 頂点カラー
	setMyGXColor(&col, 255, 255, 255);

	// 使用するテクスチャへのポインタ
	pTex = NULL;

	// 選択中のテクスチャを初期化
	anmCnt = 0;
	anmNo = 0;

	isReverseX = false;
	isReverseY = false;

	// アニメーションの更新フラグ
	isAnmEnd = true;	// テクスチャは初期状態ではないため。
}

//==========================================================
// デストラクタ
Polygon2D::~Polygon2D(){
#ifdef DIRECTX

#endif
#ifdef WII

#endif
}

//==========================================================
// 座標、高さ、幅の情報を設定
void Polygon2D::setData(float _x, float _y, float _w, float _h){
	coord.x = _x;
	coord.y = _y;
	coord.w = _w;
	coord.h = _h;
}

//==========================================================
// ポリゴンの頂点カラーを変更
void Polygon2D::changeVertexColor(unsigned char r, unsigned char g, unsigned char b){
	setMyGXColor(&col, r, g, b);
}

//==========================================================
// 使用するテクスチャへのポインタを設定
void Polygon2D::setTexturePointer(TextureUV *src){
	this->pTex = src;
}

//==========================================================
// 現在のテクスチャの取得
int Polygon2D::getAnmNo(){
	if (pTex->anmData != NULL)
		return (pTex->getAnmNo(anmNo, anmCnt));
	return 0;
}

//==========================================================
// アニメーションを変更する
// reset をfalse にすると
void Polygon2D::changeAnmNo(int _anmNo){
	anmNo = _anmNo;
	anmCnt = 0;
	isAnmEnd = false;	// 初期状態に戻るから
}

//==========================================================
// isReverseXの状態をセットする
void Polygon2D::setIsReverseX(bool state){
	isReverseX = state;
}

//==========================================================
// isReverseXの状態をセットする
void Polygon2D::setIsReverseY(bool state){
	isReverseY = state;
}


//==========================================================
// ポリゴンを描画
void Polygon2D::drawPolygon(){
#ifdef DIRECTX
	setVertexFormat();																// 頂点フォーマットを指定
#endif
	// if(テクスチャパターンが存在しているとき)
	if (pTex != NULL)
		setTexture(pTex->tex);			// 描画するにテクスチャをセット

	drawQuadPrimitiveWithAnimation(this);
}

//==========================================================
// ポリゴンの更新
void Polygon2D::updatePolygon(){
	// テクスチャのアニメーションがない場合は更新しない
	if (pTex == NULL || 
		pTex->anmData == NULL)
		return;

	isAnmEnd = false;		// 更新状態にする
	anmCnt++;
	if (getAnmNo() == ANM_LOOP){
		anmCnt = 0;			// 最初に戻す
	}
	else if (getAnmNo() == ANM_END){
		anmCnt--;			// 一つ前(一番最後)の状態に戻す
		isAnmEnd = true;	// 終了状態にする。
	}
}