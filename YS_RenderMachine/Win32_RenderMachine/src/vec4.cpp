#include "vec4.hpp"

#include <memory>

namespace ys_render_machine {


vec4::vec4()
{
	memset(E, 0, sizeof(x) * 4); 
}


vec4::vec4(float _x, float _y, float _z, float _w)
	: x(_x), y(_y), z(_z), w(_w)
{}


float
vec4::Dot(const vec4& _other) const
{
	return x * _other.x + y * _other.y + z * _other.z + w * _other.w;
}


}
