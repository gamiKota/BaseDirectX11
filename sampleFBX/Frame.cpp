//********************************************************************************
// �C���N���[�h��
//********************************************************************************
#include "Frame.h"		// ���̃N���X�̐錾��
#include "debugproc.h"	// �f�o�b�O�\���p


//********************************************************************************
// �}�N���E�萔��`
//********************************************************************************
static const int VAL_FRAME = 60;


void Frame::Init() {
	// �t���[���J�E���g������
	timeBeginPeriod(1);				// ����\��ݒ�
	m_startTime = m_nowTime = m_oldTime = (DWORD)timeGetTime();
	m_Frame = 0;
}


void Frame::Uninit() {
	// �^�C�}�ݒ�����ɖ߂�
	timeEndPeriod(1);
}


void Frame::Update() {
	m_nowTime = timeGetTime();	// ���݂̎��Ԏ擾(�~���b)
}


bool Frame::isUpdateGame() {
	if ((m_nowTime - m_oldTime) >= 1000 / VAL_FRAME) {
		m_Frame = (1000 / (m_nowTime - m_oldTime));
		m_deltaTime = m_nowTime - m_oldTime;
		m_oldTime = m_nowTime;
		return true;
	}
	return false;
}


DWORD Frame::GetFrame() {
	return m_Frame;
}


DWORD Frame::GetPassageTime() {
	return m_nowTime - m_startTime;
}

static float time;

void Frame::DrawFPS() {
	PrintDebugProc("FPS:%d\n\n", m_Frame);
	PrintDebugProc("DeltaTime:%f\n\n", m_deltaTime / 1000.f);
	time += m_deltaTime / 1000.f;
	PrintDebugProc("time:%f\n\n", time);
}


// EOF