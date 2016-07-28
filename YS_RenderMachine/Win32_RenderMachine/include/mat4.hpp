#ifndef __YS_MAT4_HPP__
#define __YS_MAT4_HPP__

#include "vec4.hpp"


namespace ys_render_machine {


// TODO: Possible improvements
// - Profiling on matrix product.

// Lightweight 4x4 matrix.
// Default value is identity.
// By the way this is column major.
struct mat4
{
	mat4();
	mat4(const mat4& _other) = default;
	mat4(mat4&& _other) = default;
	~mat4() = default;

	void	Transpose();

	mat4&	operator = (const mat4& _other) = default;
	mat4&	operator = (mat4&& _other) = default;

	mat4	operator * (const mat4& _other);

	union
	{
		vec4	col[4];
		float	data[16];
	};
};


}

#endif // __YS_MAT4_HPP__
