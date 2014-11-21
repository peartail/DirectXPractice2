cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

Texture2D shaderTex;
SamplerState sampleType;

struct VInput
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;
	float3 instPos : TEXCOORD1;
};

struct PInput
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};

PInput VS(VInput i)
{
	PInput o;

	i.pos.w = 1.0f;

	i.pos.x += i.instPos.x;
	i.pos.y += i.instPos.y;
	i.pos.z += i.instPos.z;

	o.pos = mul(i.pos, worldMatrix);
	o.pos = mul(o.pos, viewMatrix);
	o.pos = mul(o.pos, projectionMatrix);

	o.tex = i.tex;

	return o;
}

float4 PS(PInput i) : SV_TARGET
{
	float4 texcolor;

	texcolor = shaderTex.Sample(sampleType, i.tex);

	return texcolor;
}
