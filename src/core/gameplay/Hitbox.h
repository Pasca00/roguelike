#pragma once

class Hitbox {
public:
	float x;
	float y;
	float w;
	float h;

	Hitbox(float x = 0, float y = 0, float w = 0, float h = 0)
		: x(x), y(y), w(w), h(h) { }
};