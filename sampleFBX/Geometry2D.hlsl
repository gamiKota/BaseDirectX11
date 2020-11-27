struct VS_OUT
{
	float4	Position	: SV_POSITION;
	float2	TexCoord	: TEXCOORD0;
	float4	Diffuse		: COLOR0;
};


// �W�I���g���V�F�[�_�[�̓��̓p�����[�^
typedef VS_OUT GS_IN;


struct GS_OUT
{
	float4	Position	: SV_POSITION;
	float2	TexCoord	: TEXCOORD0;
	float4	Diffuse		: COLOR0;
};


cbuffer WorldBuffer : register(b0) {
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
		Out.Position = In[i].Position;
		Out.TexCoord = In[i].TexCoord;
		Out.Diffuse = In[i].Diffuse;

		TriStream.Append(Out);
	}
	TriStream.RestartStrip();
}