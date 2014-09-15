cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VInputType
{
	float4 pos : POSITION;
	float4 color : COLOR;
};

struct PInputType
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};


PInputType ColorVertexShader(VInputType i)
{
	PInputType o;

	i.pos.w = 1.0f;

	o.pos = mul(i.pos, worldMatrix);
	o.pos = mul(o.pos, viewMatrix);
	o.pos = mul(o.pos, projectionMatrix);

	o.color = i.color;

	return o;
}