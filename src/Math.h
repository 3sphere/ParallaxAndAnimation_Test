#pragma once

const float Pi = 3.1415926535f;

class Vector2
{
public:
	Vector2(float x_ = 0.0f, float y_ = 0.0f) : x(x_), y(y_) {}
	float x, y;
};