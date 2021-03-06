struct VS_INPUT {
	float4 pos : POSITION;
	float4 pos2 : POSITION1;
	float3 color: COLOR;
	float3 color2: COLOR1;
};

struct VS_OUTPUT {
	float4 pos: SV_POSITION;
	float3 color: COLOR;
	float3 color2: COLOR1;
};

cbuffer constant: register(b0) {
	//unsigned int time;
	float time;
}

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT) 0;
	//output.pos = input.pos;
	//output.color = input.color;

	/*if (output.pos.y > 0 && output.pos.y < 1) {
		output.pos.x += 0.25f;
	}

	if (output.pos.y > -1 && output.pos.y < 0 && output.pos.x > 0 && output.pos.x < 1) {
		output.pos.y += 0.25f;
	}*/

	//output.pos = lerp(input.pos, input.pos2, (sin(time) + 1.0f) / 2.0f);
	output.pos = lerp(input.pos, input.pos2, (sin(time)));
	output.color = input.color;
	output.color2 = input.color2;
	return output;
}