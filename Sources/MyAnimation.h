//==========================================================
// 使用するアニメーションの管理
#ifndef _MY_ANIMATION_H
#define _MY_ANIMATION_H

namespace animationNS{
	// アニメーションを指定する定数
	enum eAnimationLabel{
		NO_ANM = 0,
		
		CHAR_DOTS_NORMAL,
		CHAR_DOTS_DAMAGE,
		CHAR_DOTS_ATTACK_NORMAL,
		CHAR_DOTS_POSE,

		EFFECT_HIGH_ATTACK,
		EFFECT_HIGH_GUARD,

		CHARACTER_SELECT_FRAME,

		ANM_MAX,
	};

	//==========================================================
	// アニメーションファイルを読み込むのに必要なクラス
	class MyAnimationInfo{
	private:
		char	filePath[256];
		
	public:
		MyAnimationInfo(char* _fileName);
		virtual ~MyAnimationInfo();

		char* getFilePath(){ return filePath; }
	};

	extern MyAnimationInfo ANIMATION_INFO[];
}

#endif