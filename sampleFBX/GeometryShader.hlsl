struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 wPos : TEXCOORD1;
};


// �W�I���g���V�F�[�_�[�̓��̓p�����[�^
typedef VS_OUT GS_IN;


struct GS_OUT
{
	float4 pos    : SV_POSITION;
	float2 uv     : TEXCOORD0;
	float3 normal : NORMAL;
	float3 wPos   : TEXCOORD1;
	float4 color  : COLOR0;
};


// �萔�o�b�t�@
// cbuffer�L�[���[�h�Ő錾
// ���W�X�g����b...�o�b�t�@���w��
// �\���̂̂悤�ɏ������A�\���̂ł͂Ȃ��B
// �����o�ϐ����O���[�o���ϐ��̂悤�Ɉ����B
cbuffer WorldBuffer : register(b3) {
	float4x4 world;
}


[maxvertexcount(3)]   // �W�I���g���V�F�[�_�[�ŏo�͂���ő咸�_��
// �W�I���g���V�F�[�_�[
void main(triangle GS_IN In[3],					// �g���C�A���O�� ���X�g���\�����钸�_�z��̓��͏��
	inout TriangleStream<GS_OUT> TriStream		// ���_����ǉ����邽�߂̃X�g���[���I�u�W�F�N�g
)
{
	GS_OUT Out;

	int i;

	for (i = 0; i < 3; i++)
	{
		Out.pos = In[i].pos;
		Out.uv = In[i].uv;
		Out.normal = In[i].normal;
		Out.wPos = In[i].wPos;
		Out.color = float4(1.0f, 1.0f, 1.0f, 1.0f);

		TriStream.Append(Out);
	}
	TriStream.RestartStrip();
}