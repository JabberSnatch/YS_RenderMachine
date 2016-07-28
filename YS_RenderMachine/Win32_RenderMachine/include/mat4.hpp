#ifndef __YS_MAT4_HPP__
#define __YS_MAT4_HPP__

#include "vec4.hpp"


namespace ys_render_machine {


// Lightweight 4x4 matrix.
// Default value is identity.
// By the way this is column major.
struct mat4
{
	mat4() 
	{
		memset(data, 0, sizeof(float) * 16);
		col[0].x = 1.0f;
		col[1].y = 1.0f;
		col[2].z = 1.0f;
		col[3].w = 1.0f;
	}

	union
	{
		vec4	col[4];
		float	data[16];
	};
};


}


#endif // __YS_MAT4_HPP__
