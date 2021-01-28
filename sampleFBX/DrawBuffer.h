#ifndef __DRAW_BUFFER_H__
#define __DRAW_BUFFER_H__

#include <Windows.h>
#include <d3d11.h>

class DrawBuffer
{
public:
	DrawBuffer();
	~DrawBuffer();

	// DirectX��Buffer�ɗ��߂�
	// ���_�������Ƃɕ`���
	// �s���B���_�̏���
	// ���₵���茸�炵���肷��
	// ���Ƃ��ł���B���̂��߁A
	// �����Œ��_�ׂ̍�������
	// ������K�v������B
	HRESULT CreateVertexBuffer(
		// �`��ɕK�v�Ȓ��_�̃f�[�^
		const void* pVtx,
		// �������̒��_��
		// �f�[�^�T�C�Y
		UINT size,
		// �`�悷�钸�_�̐�
		UINT count,

		bool isWrite = false
	);

	// �C���f�b�N�X�o�b�t�@�̍쐬
	// ���ۂ̃��f���͓������_�����L����
	// �O�p�`��`�悷�邱�Ƃ������B
	// �����ŎO�p�`��`�悷��ۂɁA����
	// ���_���w�肷��̂ł͂Ȃ��A
	// �C���f�b�N�X(�Q�Ɣԍ�)��p����
	// �`�悷��B
	// ���_�o�b�t�@�݂̂ŕ`�悷�����
	// �f�[�^�ʂ��y���Ȃ�
	HRESULT CreateIndexBuffer(
		const void* pIdx,
		UINT size,
		UINT count
	);

	// �o�b�t�@�̕`��
	void Draw(
		// �|���S���̕`����@
		D3D11_PRIMITIVE_TOPOLOGY primitive);

	// ��������
	HRESULT Write(void* pVtx);

private:
	// ���_�o�b�t�@
	ID3D11Buffer*
		m_pVertexBuffer;
	// ���_�̃f�[�^�T�C�Y
	UINT m_vtxSize;
	// ���_�̐�
	UINT m_vtxCount;

	// �C���f�b�N�X�o�b�t�@
	ID3D11Buffer* m_pIndexBuffer;
	// �C���f�b�N�X�T�C�Y
	UINT m_idxSize;
	// �C���f�b�N�X�̐�
	UINT m_idxCount;
};



#endif // __DRAW_BUFFER_H__