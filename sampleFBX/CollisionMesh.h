/**
 * @file CollisionMesh.h
 */


/**
 * @include guard
 */
#pragma once


/**
 * @include
 */
#include "Component.h"


/**
 * @class CollisionMesh
 */
class CollisionMesh : public Component {

	// メモ
	// 最初に頂点情報や法線を設定してるので
	// 法線の更新の心配はいらない
	// ライトの向きと法線の向きが一緒だと常に明るい
	// 逆だと常に暗い(影)
	// もし3Dオブジェクトの当たり判定にメッシュを使っても
	// 面と線分の当たり判定でいける

	// メモ
	// ベクトルとベクトルの内積が正の値なら合成図は鋭角
	// 負の値なら鈍角
	// 法線ベクトルのライトのベクトルから内積をとって影を作ってる

	// メモ
	// 一番目の引数の面を基準として、
	// 二番目の引数の面の4つの頂点情報を元に
	// 面と線分の当たり判定に変換する
	// (面の法線ベクトルと線分各頂点のベクトルの内積をとって掛け合わせた値が負の値なら衝突)
	// (今回は法線ではなく、上方向ベクトルを使う)
	// 4本線分ではなく、対角線の2本でいけそう ※いけません、ちゃんと4本線分で判定してください

public:
	// 今は取り合えず無限平面との当たり判定になってる
	static bool isMesh2Mesh(Transform* mesh1, Transform* mesh2);

	/**
	 * @brief メッシュの頂点座標取得
	 */
	static void GetVertex(Transform* mesh, float3 vertex[4]);
};


// EOF