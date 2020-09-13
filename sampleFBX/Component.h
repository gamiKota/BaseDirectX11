/**
 * @file Object.h
 */


/**
 * @include guard
 */
#ifndef ___HEADER_COMPONENT_H___
#define ___HEADER_COMPONENT_H___


/**
 * @include
 */
#include "Object.h"
#include "Data.h"


/**
 * @forward declaration
 */
class GameObject;
class Transform;


/**
 * @class Component : inheritance Object
 */
class Component : public Object {
public:
	GameObject*				m_gameObject;	//!< 親オブジェクト
	Transform*				m_transform;	//!< トランスフォーム

public:
	Component();
	virtual ~Component();

	/*メモ
	 AwekeはAddComponentのタイミング
	 StartはUpdateやDrawと一緒で一括処理
	 */
	
	virtual void Awake();
	virtual void Start();
	virtual void Uninit();
	virtual void Update();
	virtual void LastUpdate();


	// もうここでええや
	// わかんにゃい
	virtual void OnCollision(GameObject* obj) {}
};


/*

XMMATRIX情報の理解

(world._11, world._22, world._33)は各X軸Y軸Z軸の引き延ばしの基準値
X軸で回転させたいなら、X軸意外が動く式(sin()cos()を使う奴)
(world._14, world._24, world._34)は基本弄らない値(アフィン変換やらなんか)
(world._41, world._42, world._43)は座標、(world._44は他全部の値の基準(1→2にすると全体が小さくなる感じ))
行列の値の順番は、拡縮→回転→位置(だから破魔矢サンプルではvPosと格納用変数が用意されていた)

*/


#endif // !___HEADER_COMPONENT_H___
// EOF