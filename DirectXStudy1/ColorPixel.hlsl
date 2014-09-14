struct PInputType
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

float4 main(PInputType i) : SV_TARGET
{
	return i.color;
}