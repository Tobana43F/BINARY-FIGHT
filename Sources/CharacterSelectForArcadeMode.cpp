#include	"CharacterSelectForArcadeMode.h"

//==========================================================
// CharacterSelectForArcadeMode
//----------------------------------------------------------
// コンストラクタ
CharacterSelectForArcadeMode::CharacterSelectForArcadeMode(BattleConfigInfo *_pBattleConfigInfo):
CharacterSelect(_pBattleConfigInfo)	// 引数付きコンストラクタのスーパークラスの初期化
{

}

//----------------------------------------------------------
// デストラクタ
CharacterSelectForArcadeMode::~CharacterSelectForArcadeMode(){

}

//----------------------------------------------------------
// 描画
void CharacterSelectForArcadeMode::draw(){
	polyBack.drawPolygon();
	drawCharacterBox();
	polyPlayer1Cursor.drawPolygon();	// プレイヤー１のみ描画
}

//----------------------------------------------------------
// 更新
void CharacterSelectForArcadeMode::update(){
	updateGameScene();
	if (isSceneEnd)
		return;

	updatePlayer1Cursor();
}

//----------------------------------------------------------
// 画面遷移
void CharacterSelectForArcadeMode::updateGameScene(){
	if (isPlayer1Selected == true){// 両プレイヤー、キャラクターを選択し終わっていたら
		pBattleConfigInfo->setSelectCharacter(
			(eCharacterList)curPlayer1Cursor, 
			(eCharacterList)CHARACTER_LIST_RANDOM);	// プレイヤー２は適当に初期化
		pBattleConfigInfo->setPlayerID(1, PLAYERID_PLAYER1);	// プレイヤー１ VS CPU
		pBattleConfigInfo->setPlayerID(2, PLAYERID_CPU);

		isSceneEnd = true;
	}
}
