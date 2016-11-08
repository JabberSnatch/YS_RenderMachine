#ifndef __YS_VEC4_HPP__
#define __YS_VEC4_HPP__


namespace ys_render_machine {

// TODO: Possible improvements
// - Compare performances using either simd intrinsics or standard operators.

// Lightweight 4-component vector.
// Default value is 0.f for each component.
struct vec4
{
	vec4(); 
	vec4(float _x, float _y, float _z, float _w = 1.0f);

	vec4(const vec4& _other) = default;
	vec4(vec4&& _other) = default;

	float	Dot(const vec4& _other) const;

	vec4	operator * (const float& _scalar);
	vec4	operator + (const vec4& _other);

	vec4&	operator = (const vec4& _other) = default;
	vec4&	operator = (vec4&& _other) = default;

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
