//==========================================================
// 全キャラクターに連番を振る
#ifndef _CONST_CHARACTER_LIST
#define _CONST_CHARACTER_LIST

//----------------------------------------------------------
// クラス外から、キャラクターを指定するときに使用する定数(CharacterSelectクラス内の、テクスチャリストと合わせておく必要がある)
enum eCharacterList{
	CHARACTER_LIST_DOTS,
	CHARACTER_LIST_RANDOM,	// 必要ないかも(ランダムの選択肢、ランダムなキャラクターを選択する)
	CHARACTER_LIST_DOTS_FOR_TUTORIAL,	// チュートリアル用キャラ
	MAX_CHARACTER_LIST,
};


#endif