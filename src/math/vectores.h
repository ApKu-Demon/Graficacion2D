#pragma once

typedef union _vec2
{
	float data[2];
	struct
	{
		float x;
		float y;
	}unpack;
}Vec2;

typedef union _vec3
{
	float data[3];
	struct
	{
		float x;
		float y;
		float z;
	}unpack;
}Vec3;

float distanciav2(Vec2 p1, Vec2 p2);

extern Vec2 vec2;

