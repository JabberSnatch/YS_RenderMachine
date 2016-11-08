#include "_vec4.hpp"

#include <memory>

namespace ys_render_machine {


vec4::vec4()
{
	memset(E, 0, sizeof(x) * 4);
	w = 1.f;
}


vec4::vec4(float _x, float _y, float _z, float _w)
	: x(_x), y(_y), z(_z), w(_w)
{}


float
vec4::Dot(const vec4& _other) const
{
	return x * _other.x + y * _other.y + z * _other.z + w * _other.w;
}


vec4
vec4::operator * (const float& _scalar)
{
	return vec4(x * _scalar, y * _scalar, z * _scalar, w * _scalar);
}


vec4
vec4::operator + (const vec4& _other)
{
	return vec4(x + _other.x, y + _other.y, z + _other.z, w + _other.w);
}


}
