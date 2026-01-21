//==========================================================
// 天空ステージ

#include	"AbstractStage.h"	// 継承するため


//==========================================================
// トレーニングステージ用クラス
class StageSkyPlace : public AbstractStage{
protected:
	// テクスチャ
	enum eTexList{
		BACK_GROUND,
		MAX_TEX,
	};

public:
	StageSkyPlace();
	virtual ~StageSkyPlace();

	void initStage();

	//==========================================================
	// 基本的な関数
	// 継承したものを使う

};