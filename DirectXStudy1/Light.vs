cbuffer MatrixBuffer
{
	matrix world;
	matrix view;
	matrix proj;
};

struct VInput
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PInput
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

PInput VS(VInput i)
{
	PInput o;

	i.pos.w = 1.0f;

	o.pos = mul(i.pos, world);
	o.pos = mul(o.pos, view);
	o.pos = mul(o.pos, proj);

	o.tex = i.tex;

	o.normal = mul(i.normal, (float3x3)world);

	o.normal = normalize(o.normal);

	return o;
}