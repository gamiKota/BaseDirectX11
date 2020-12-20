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

	// ����
	// �ŏ��ɒ��_����@����ݒ肵�Ă�̂�
	// �@���̍X�V�̐S�z�͂���Ȃ�
	// ���C�g�̌����Ɩ@���̌������ꏏ���Ə�ɖ��邢
	// �t���Ə�ɈÂ�(�e)
	// ����3D�I�u�W�F�N�g�̓����蔻��Ƀ��b�V�����g���Ă�
	// �ʂƐ����̓����蔻��ł�����

	// ����
	// �x�N�g���ƃx�N�g���̓��ς����̒l�Ȃ獇���}�͉s�p
	// ���̒l�Ȃ�݊p
	// �@���x�N�g���̃��C�g�̃x�N�g��������ς��Ƃ��ĉe������Ă�

	// ����
	// ��Ԗڂ̈����̖ʂ���Ƃ��āA
	// ��Ԗڂ̈����̖ʂ�4�̒��_��������
	// �ʂƐ����̓����蔻��ɕϊ�����
	// (�ʂ̖@���x�N�g���Ɛ����e���_�̃x�N�g���̓��ς��Ƃ��Ċ|�����킹���l�����̒l�Ȃ�Փ�)
	// (����͖@���ł͂Ȃ��A������x�N�g�����g��)
	// 4�{�����ł͂Ȃ��A�Ίp����2�{�ł������� �������܂���A������4�{�����Ŕ��肵�Ă�������

public:
	// ���͎�荇�����������ʂƂ̓����蔻��ɂȂ��Ă�
	static bool isMesh2Mesh(Transform* mesh1, Transform* mesh2);

	/**
	 * @brief ���b�V���̒��_���W�擾
	 */
	static void GetVertex(Transform* mesh, float3 vertex[4]);
};


// EOF