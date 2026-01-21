#include	"BattleEffect.h"	// クラスのメソッドを定義するため

//==========================================================
// EffectListクラス
//----------------------------------------------------------
// コンストラクタ
// 表示する中心座標を渡す
EffectList::EffectList(){

}

//----------------------------------------------------------
// デストラクタ
EffectList::~EffectList(){

}

//----------------------------------------------------------
// 座標の設定
void EffectList::setData(FCoord2D *_pSrc, TextureUV *_src){
	// ポリゴンに反映
	poly.setData(
		_pSrc->x, _pSrc->y,
		_pSrc->w, _pSrc->h);

	poly.setTexturePointer(_src);
}

//----------------------------------------------------------
// アニメーションナンバーの設定
void EffectList::changeAnmNo(int _anmNo){
	poly.changeAnmNo(_anmNo);
}

//----------------------------------------------------------
// 描画
void EffectList::draw(){
	poly.drawPolygon();
}

//----------------------------------------------------------
// 更新
void EffectList::update(){
	poly.updatePolygon();
}


//==========================================================
// BattleEffectクラス
//----------------------------------------------------------
// コンストラクタ
BattleEffect::BattleEffect():
LOW_DAMAGE_SIZE(100.0f),
MID_DAMAGE_SIZE(150.0f),
HIGH_DAMAGE_SIZE(200.0f)
{
	// テクスチャ
	pTextureLoader = new TextureLoader(MAX_TEX);
	pTextureLoader->setUseTextureLabel(HIGH_ATTACK, textureNS::EFFECT_HIGH_ATTACK);
	pTextureLoader->setUseTextureLabel(HIGH_GUARD, textureNS::EFFECT_HIGH_GUARD);

	pTextureLoader->setUseAnimationLabel(HIGH_ATTACK, animationNS::EFFECT_HIGH_ATTACK);
	pTextureLoader->setUseAnimationLabel(HIGH_GUARD, animationNS::EFFECT_HIGH_GUARD);



	// 先頭確保(ダミーデータ)
	pEffectListTop = new EffectList();
	pEffectListTop->next = NULL;

}

//----------------------------------------------------------
// デストラクタ
BattleEffect::~BattleEffect(){
	freeAllEffectList();
	delete pTextureLoader;
	pTextureLoader = NULL;
}

//----------------------------------------------------------
// 初期化
void BattleEffect::init(){
	freeAllEffectList();
}

//----------------------------------------------------------
// ロード
void BattleEffect::load(){
	pTextureLoader->loadUseTexture();
	pTextureLoader->loadUseAnimation();
}

//----------------------------------------------------------
// 攻撃情報から、座標情報を取得
void BattleEffect::getDataFromAttackInfo(FCoord2D *_pCoordDest, eTexList *_pTexListDest, AttackInfo *_pAttackInfo){
	switch (_pAttackInfo->strength){
		case NOTHING_STRENGTH:	// ありえない
			break;

		case LOW_STRENGTH:
			_pCoordDest->w = LOW_DAMAGE_SIZE;
			_pCoordDest->h = LOW_DAMAGE_SIZE;
			*_pTexListDest = HIGH_ATTACK;
			break;

		case HIGH_STRENGTH:
			_pCoordDest->w = HIGH_DAMAGE_SIZE;
			_pCoordDest->h = HIGH_DAMAGE_SIZE;
			*_pTexListDest = HIGH_ATTACK;
			break;
	}

}



//----------------------------------------------------------
// リストの先頭データ(ダミーデータを飛ばす)を取得
EffectList* BattleEffect::getEffectListDataTop(){
	return pEffectListTop->next;
}

//----------------------------------------------------------
// リストの先頭(ダミーデータ)を取得
EffectList* BattleEffect::getEffectListTop(){
	return pEffectListTop;
}

//----------------------------------------------------------
// 全リスト開放
void BattleEffect::freeAllEffectList(){
	EffectList	*cur, *work;

	cur = getEffectListTop();	// 先頭を設定
	for (int i = 0; cur->next != NULL; ++i){
		work = cur->next;
		delete cur;
		cur = work;
	}
	cur = NULL;
	pEffectListTop->next = NULL;
}

//----------------------------------------------------------
// EffectListの追加
void BattleEffect::addEffectList(float _centerX, float _centerY, AttackInfo *_attackInfo){
	EffectList *cur;
	FCoord2D   coord;	// 座標
	eTexList   srcNo;	// 使用するテクスチャ番号

	cur = getEffectListTop();	// データの先頭を設定
	while (cur->next != NULL){
		cur = cur->next;
	}

	cur->next = new EffectList;	// 終端にリスト確保
	cur = cur->next;			// 参照先を確保したリストへ

	// 使用する座標データを作成
	getDataFromAttackInfo(&coord, &srcNo, _attackInfo);	// 使用するデータを取得
	coord.x = _centerX - coord.w / 2.0f;				// 中心座標からポリゴンで使用する左上の座標へ変換
	coord.y = _centerY - coord.h / 2.0f;

	cur->setData(&coord, pTextureLoader->getTexturePointer(srcNo));	// データの設定
	cur->changeAnmNo(NORMAL);
	cur->next = NULL;			// 終端子を代入
}


//----------------------------------------------------------
// EffectListの追加
void BattleEffect::addEffectList(Actor *_pActor, AttackInfo *_attackInfo){
	EffectList *cur;
	FCoord2D   coord;	// 座標
	eTexList   srcNo;	// 使用するテクスチャ番号

	cur = getEffectListTop();	// データの先頭を設定
	while (cur->next != NULL){
		cur = cur->next;
	}

	cur->next = new EffectList;	// 終端にリスト確保
	cur = cur->next;			// 参照先を確保したリストへ

	getDataFromAttackInfo(&coord, &srcNo, _attackInfo);

	float centerX =
		_attackInfo->collideBox.x +
		_attackInfo->collideBox.w / 2.0f;

	float centerY =
		_attackInfo->collideBox.y +
		_attackInfo->collideBox.h / 2.0f;

	coord.x = centerX - coord.w / 2.0f;				// 中心座標からポリゴンで使用する左上の座標へ変換
	coord.y = centerY - coord.h / 2.0f;

	if (_pActor->isBlock)
		srcNo = HIGH_GUARD;
	else
		srcNo = HIGH_ATTACK;

	cur->setData(&coord, pTextureLoader->getTexturePointer(srcNo));	// データの設定
	cur->changeAnmNo(NORMAL);
	cur->next = NULL;			// 終端子を代入
}

//----------------------------------------------------------
// EffectListの開放
//  _target :　削除対象
void BattleEffect::freeEffectList(EffectList *_target){
	EffectList *cur, *prev;

	prev = getEffectListTop();		// 一応ダミーの先頭
	cur = getEffectListDataTop();	// データの先頭
	while (cur != _target){			// 削除対象まで進める
		prev = cur;					// 前のリストを格納
		cur = cur->next;			// 次へ
	}

	prev->next = cur->next;			// リスト連結
	delete cur;
	cur = NULL;
}

//----------------------------------------------------------
// 描画
void BattleEffect::draw(){
	EffectList *cur;

	cur = getEffectListDataTop();
	while (cur != NULL){
		cur->draw();		// 個々のエフェクトを表示

		cur = cur->next;	// 次へ
	}
}

//----------------------------------------------------------
// 更新
void BattleEffect::update(){
	EffectList *cur, *freeTarget;

	cur = getEffectListDataTop();
	while (cur != NULL){
		cur->update();		// 個々のエフェクトを表示

		if (cur->poly.isAnmEnd == false){
			cur = cur->next;			// 次へ
			continue;
		}

		// エフェクトのアニメーションが終わった場合
		freeTarget = cur;			// 前リストを格納
		cur = cur->next;			// 次へ

		freeEffectList(freeTarget);	// リストを削除
	}
}