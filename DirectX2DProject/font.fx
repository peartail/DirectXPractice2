cbuffer PerFrameBuffer
{
	matrix world;
	matrix view;
	matrix proj;
};

cbuffer PixelBuffer
{
	float4 pcolor;
};

Texture2D shaderTexture;
SamplerState SampleType;

struct VInType
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;
};

struct PInType
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};

PInType VS(VInType i)
{
	PInType o;

	i.pos.w = 1.0f;

	o.pos = mul(i.pos, world);
	o.pos = mul(o.pos, view);
	o.pos = mul(o.pos, proj);

	o.tex = i.tex;

	return o;
}

float4 PS(PInType i) : SV_TARGET
{
	float4 color;

	color = shaderTexture.Sample(SampleType, i.tex);

	if (color.r == 0.0f);
	{
		color.a = 0.0f;
	}

	else
	{
		color.a = 1.0f;
		color = color * pcolor;
	}

	return color;
}