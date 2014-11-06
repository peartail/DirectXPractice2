cbuffer MatrixBuffer
{
	matrix world;
	matrix view;
	matrix proj;
};

Texture2D shaderTexture[2];
SamplerState SampleType;

struct VInputT
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;
};

struct PInputT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};

PInputT VS(VInputT i)
{
	PInputT o;

	i.pos.w = 1.0f;

	o.pos = mul(i.pos, world);
	o.pos = mul(o.pos, view);
	o.pos = mul(o.pos, proj);

	o.tex = i.tex;

	return o;
}

float4 PS(PInputT i) : SV_TARGET
{
	float4 color1;
	float4 color2;
	float4 blendcolor;

	color1 = shaderTexture[0].Sample(SampleType, i.tex);
	color2 = shaderTexture[1].Sample(SampleType, i.tex);

	blendcolor = color2 * color1 * 2.0;
	//blendcolor = color2; 

	blendcolor = saturate(blendcolor);

	return blendcolor;
}