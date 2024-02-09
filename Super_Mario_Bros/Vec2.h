#pragma once

class Vec2
{
public:
	float x;
	float y;

	Vec2 operator + (Vec2& rhs) const
	{
		return Vec2(x + rhs.x, y + rhs.y);
	}

	Vec2 operator - (Vec2& rhs) const
	{
		return Vec2(x - rhs.x, y - rhs.y);
	}

	Vec2 operator - (const Vec2& rhs) const
	{
		return Vec2(x - rhs.x, y - rhs.y);
	}

	Vec2 operator * (Vec2& rhs) const
	{
		return Vec2(x * rhs.x, y * rhs.y);
	}

	Vec2 operator * (const Vec2& rhs) const
	{
		return Vec2(x * rhs.x, y * rhs.y);
	}

	Vec2 operator / (Vec2& rhs) const
	{
		return Vec2(x / rhs.x, y / rhs.y);
	}

	void operator += (Vec2& rhs)
	{
		x += rhs.x;
		y += rhs.y;
	}

	void operator -= (Vec2& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
	}

	void operator *= (Vec2& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
	}

	void operator /= (Vec2& rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
	}
};