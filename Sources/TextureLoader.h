#ifndef _TEXTURE_LOADER_H
#define _TEXTURE_LOADER_H

#include	"MyTexture.h"
#include	"MyAnimation.h"
#include	"Polygon.h"

//==========================================================
// テクスチャの読み込みに使用するクラス
// 後々、アニメーションのロードなどもここで行う。
// ・テクスチャを使用するクラスは、このクラスを内包する必要がある
// ・それに加え、列挙対でテクスチャリスト(テクスチャを指定できる添え字になるもの)を持ち、最大数を引数に渡す
class TextureLoader{
private:
	int								texNum;			// テクスチャ数
	textureNS::eTextureLabel		*useTexLabel;	// 配列を確保する
	animationNS::eAnimationLabel	*useAnmLabel;	// 配列を確保する
	TextureUV						*useTex;		// 配列を確保する

public:

	TextureLoader(int _maxTex);
	virtual ~TextureLoader();

	//==========================================================
	// テクスチャのロードに必要な関数
	//void setUseTextureLabel(int _texNum, int _label, ...);	// 可変引数の処理を簡単にするために、列挙型の引数にしない
															// 第2引数以降しか、可変引数を利用できない？ので最初に他の意味の引数を持たせる
	void setUseTextureLabel(int _label, textureNS::eTextureLabel _texLabel);

	void loadUseTexture();
	void releaseUseTexture();

	//==========================================================
	// アニメーションのロードに必要な関数
	//void setUseAnimationLabel(int _texNum, int _label, ...);	// テクスチャの枚数に従うため、_texNumを引数に持
	void setUseAnimationLabel(int _label, animationNS::eAnimationLabel _anmLabel);

	void loadUseAnimation();
	void releaseUseAnimation();

	//==========================================================
	// 使用できるテクスチャのポインタを取得する
	TextureUV* getTexturePointer(int _texListNo);
};

#endif