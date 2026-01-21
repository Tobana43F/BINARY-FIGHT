//==========================================================
// 当たり判定をロードする際に使用する、定数、クラスを定義

namespace hitBoxNS{
	//==========================================================
	// 当たり判定をロードする識別子
	enum eHitBoxLabel{
		DOTS_HURT_BOX,
		DOTS_BODY_BOX,
		DOTS_ATTACK_BOX,
		DOTS_GUARD_BOX,
		DOTS_WALL_BOX,

		MAX_HIT_BOX,
	};

	//==========================================================
	// ヒットボックスファイルを読み込むのに必要なクラス
	class MyHitBoxInfo{
	private:
		char	filePath[256];

	public:
		MyHitBoxInfo(char* _fileName);
		virtual ~MyHitBoxInfo();

		char* getFilePath(){ return filePath; }
	};

	extern MyHitBoxInfo HITBOX_INFO[];

}