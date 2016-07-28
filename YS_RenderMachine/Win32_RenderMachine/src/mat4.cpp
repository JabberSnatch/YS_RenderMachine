#include "mat4.hpp"

#include <memory>

namespace ys_render_machine {


mat4::mat4()
{
	memset(data, 0, sizeof(float) * 16);
	col[0].x = 1.0f;
	col[1].y = 1.0f;
	col[2].z = 1.0f;
	col[3].w = 1.0f;
}


void
mat4::Transpose()
{
	vec4 rows[4];
	rows[0] = vec4(col[0].x, col[1].x, col[2].x, col[3].x);
	rows[1] = vec4(col[0].y, col[1].y, col[2].y, col[3].y);
	rows[2] = vec4(col[0].z, col[1].z, col[2].z, col[3].z);
	rows[3] = vec4(col[0].w, col[1].w, col[2].w, col[3].w);

	memcpy(col, rows, sizeof(float) * 16);
}


mat4
mat4::operator * (const mat4& _other)
{
	mat4	result;
	mat4	transposed = *this;
	transposed.Transpose();

	for (unsigned int row = 0; row < 4; ++row)
	{
		for (unsigned int column = 0; column < 4; ++column)
		{
			result.data[row + column * 4] = 
				transposed.col[row].Dot(_other.col[column]);
		}
	}

	return result;
}


}
