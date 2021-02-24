struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 shadowPos : TEXCOORD1;
};

float4 main(PS_IN PIN) : SV_Target
{
	float4 color = float4(1,0,0,1);
	// ���W�ϊ���w�ɂ̓j�A�N���b�v����t�@�[�N���b�v�܂ł̋������i�[�����B
	// z�l���̂܂܏������ނ�1�ȏ�̒l���Ƃ��Ă��܂��̂����O�����̂ŁA
	// �S�̂̒���(w)�Ŋ�����0�`1�͈̔͂ɐ��K�������f�[�^����������
	color.r = PIN.shadowPos.z / PIN.shadowPos.w;
	return color;
}