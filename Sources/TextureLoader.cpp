#include	"TextureLoader.h"	// クラスのメソッドを定義するため
#include	"Debug.h"			// エラー処理

//==========================================================
// TextureLoaderクラス
//----------------------------------------------------------
// コンストラクタ
TextureLoader::TextureLoader(int _maxTex){
	texNum = _maxTex;
	useTexLabel = new textureNS::eTextureLabel[texNum];
	useAnmLabel = new animationNS::eAnimationLabel[texNum];
	useTex = new TextureUV[texNum];

	// 初期化
	for (int i = 0; i < texNum; ++i){
		useTexLabel[i] = textureNS::TEX_MAX;	// 無効な値を入れておく(エラー処理で使用する)
		useAnmLabel[i] = animationNS::ANM_MAX;
	}
}

//----------------------------------------------------------
// デストラクタ
TextureLoader::~TextureLoader(){
	delete[] useTexLabel;
	useTexLabel = NULL;
	delete[] useAnmLabel;
	useAnmLabel = NULL;
	delete[] useTex;
	useTex = NULL;
}

//----------------------------------------------------------
// テクスチャラベルの設定
// _label : クラス内で、テクスチャを指定するラベル。
// _texLabel : テクスチャを指定するラベル。
void TextureLoader::setUseTextureLabel(int _label, textureNS::eTextureLabel _texLabel){
	if (_label > texNum)
		outErrorMessage("setUseTextureLabel()関数で、無効な値が指定されました。");

	useTexLabel[_label] = _texLabel;
}

//----------------------------------------------------------
// テクスチャのロード
void TextureLoader::loadUseTexture(){
	for (int i = 0; i < texNum; ++i){
		if (useTexLabel[i] == textureNS::TEX_MAX)
			outErrorMessage("テクスチャの読み込みに失敗しました。\n 内容:テクスチャのラベルが設定されていません。");

		useTex[i].loadTexture(useTexLabel[i]);
	}
}

//----------------------------------------------------------
// テクスチャの解放
// 開放処理は、基本的にこのクラスを解放することで行う。
// この関数をよび、任意のタイミングで開放することも可能
void TextureLoader::releaseUseTexture(){
	if (useTex == NULL)
		return;

	for (int i = 0; i < texNum; ++i){
		useTex[i].releaseTexture();
	}
}

//----------------------------------------------------------
// アニメーションラベルの設定
// _label : クラス内で、テクスチャを指定するラベル。
// _texLabel : テクスチャを指定するラベル。
void TextureLoader::setUseAnimationLabel(int _label, animationNS::eAnimationLabel _anmLabel){
	if (_label > texNum)
		outErrorMessage("setUseAnimationLabel()関数で、無効な値が指定されました。");

	useAnmLabel[_label] = _anmLabel;
}


//----------------------------------------------------------
// アニメーションのロード
void TextureLoader::loadUseAnimation(){
	for (int i = 0; i < texNum; ++i){
		if (useAnmLabel[i] == animationNS::ANM_MAX)
			outErrorMessage("アニメーションの読み込みに失敗しました。\n 内容:アニメーションのラベルが設定されていません。");

		useTex[i].loadAnmData(useAnmLabel[i]);
	}
}

//----------------------------------------------------------
// アニメーションの開放
void TextureLoader::releaseUseAnimation(){
	if (useTex == NULL)
		return;

	for (int i = 0; i < texNum; ++i){
		useTex[i].releaseAnmData();
	}
}


//----------------------------------------------------------
// 使用できるテクスチャのポインタを取得
TextureUV* TextureLoader::getTexturePointer(int _texListNo){
	if (_texListNo >= texNum){
		return NULL;	//　エラー処理
	}

	return &useTex[_texListNo];
}