//==========================================================
// トレーニングステージ

#include	"AbstractStage.h"	// 継承するため

//==========================================================
// トレーニングステージ用クラス
class StageTraining : public AbstractStage{
protected:
	// テクスチャ
	enum eTexList{
		BACK_GROUND,
		MAX_TEX,
	};

public:
	StageTraining();
	virtual ~StageTraining();

	void initStage();
	
	//==========================================================
	// 基本的な関数
	// 継承したものを使う

};