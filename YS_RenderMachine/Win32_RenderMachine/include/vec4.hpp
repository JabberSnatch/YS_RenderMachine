#ifndef __YS_VEC4_HPP__
#define __YS_VEC4_HPP__


namespace ys_render_machine {


// Lightweight 4-component vector.
// Default value is 0.f for each component.
struct vec4
{
	vec4() { memset(E, 0, sizeof(x) * 4); }
	vec4(float _x, float _y, float _z, float _w = 1.0f)
		: x(_x), y(_y), z(_z), w(_w)
	{}

	vec4(const vec4& other) { memcpy(E, other.E, sizeof(x) * 4); }
	vec4(vec4&& other) { memcpy(E, other.E, sizeof(x) * 4); }

	vec4&	operator = (const vec4& other)
	{
		memcpy(E, other.E, sizeof(x) * 4);
		return *this;
	}
	vec4&	operator = (vec4&& other)
	{
		memcpy(E, other.E, sizeof(x) * 4);
		return *this;
	}

	union
	{
		float E[4];
		struct
		{
			float x, y, z, w;
		};
	};
};


}


#endif // __YS_VEC4_HPP__
