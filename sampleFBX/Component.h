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
	GameObject*				m_gameObject;	//!< �e�I�u�W�F�N�g
	Transform*				m_transform;	//!< �g�����X�t�H�[��

public:
	Component();
	virtual ~Component();

	/*����
	 Aweke��AddComponent�̃^�C�~���O
	 Start��Update��Draw�ƈꏏ�ňꊇ����
	 */
	
	virtual void Awake();
	virtual void Start();
	virtual void Uninit();
	virtual void Update();
	virtual void LastUpdate();


	// ���������ł�����
	// �킩��ɂႢ
	virtual void OnCollision(GameObject* obj) {}
};


/*

XMMATRIX���̗���

(world._11, world._22, world._33)�͊eX��Y��Z���̈������΂��̊�l
X���ŉ�]���������Ȃ�AX���ӊO��������(sin()cos()���g���z)
(world._14, world._24, world._34)�͊�{�M��Ȃ��l(�A�t�B���ϊ����Ȃ�)
(world._41, world._42, world._43)�͍��W�A(world._44�͑��S���̒l�̊(1��2�ɂ���ƑS�̂��������Ȃ銴��))
�s��̒l�̏��Ԃ́A�g�k����]���ʒu(������j����T���v���ł�vPos�Ɗi�[�p�ϐ����p�ӂ���Ă���)

*/


#endif // !___HEADER_COMPONENT_H___
// EOF