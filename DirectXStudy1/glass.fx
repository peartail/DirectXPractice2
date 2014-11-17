cbuffer MatrixBuffer : register(b0)
{
	matrix world;
	matrix view;
	matrix proj;
};

cbuffer GlassBuffer : register(b1)
{
	float refractionScale;
	float3 padding;
};

SamplerState sampleType;

Texture2D ctex : register(t0);
Texture2D ntex : register(t1);
Texture2D rtex : register(t2);


struct VInputT
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;
};

struct PInputT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
	float4 refractPos : TEXCOORD1;
};

PInputT VS(VInputT i)
{
	PInputT o;
	matrix vpw;

	i.pos.w = 1.0f;

	o.pos = mul(i.pos, world);
	o.pos = mul(o.pos, view);
	o.pos = mul(o.pos, proj);

	o.tex = i.tex;

	vpw = mul(view, proj);
	vpw = mul(world, vpw);

	o.refractPos = mul(i.pos, vpw);

	return o;
}

float4 PS(PInputT i) : SV_TARGET
{
	float2 rtexcoord;
	float4 nmap;
	float3 norm;
	float4 refracColor;
	float4 texColor;
	float4 color;

	rtexcoord.x = i.refractPos.x / i.refractPos.w / 2.0f + 0.5f;
	rtexcoord.y = -i.refractPos.y / i.refractPos.w / 2.0f + 0.5f;

	nmap = ntex.Sample(sampleType, i.tex);

	norm = (nmap.xyz * 2.0f) - 1.f;

	rtexcoord = rtexcoord + (norm.xy * refractionScale);

	refracColor = rtex.Sample(sampleType, rtexcoord);

	texColor = ctex.Sample(sampleType, i.tex);

	color = lerp(refracColor, texColor, 0.5);
	
	//color = float4(1, 1, 1, 1);

	return color;
}