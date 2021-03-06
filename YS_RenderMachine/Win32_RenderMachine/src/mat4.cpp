#include "_mat4.hpp"

#include <memory>

#define PI 3.14159265359f

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
#if 0
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
#else
	result.col[0] = (col[0] * _other.col[0].x) + 
					(col[1] * _other.col[0].y) + 
					(col[2] * _other.col[0].z) +
					(col[3] * _other.col[0].w);
	result.col[1] = (col[0] * _other.col[1].x) +
					(col[1] * _other.col[1].y) +
					(col[2] * _other.col[1].z) +
					(col[3] * _other.col[1].w);
	result.col[2] = (col[0] * _other.col[2].x) +
					(col[1] * _other.col[2].y) +
					(col[2] * _other.col[2].z) +
					(col[3] * _other.col[2].w);
	result.col[3] = (col[0] * _other.col[3].x) +
					(col[1] * _other.col[3].y) +
					(col[2] * _other.col[3].z) +
					(col[3] * _other.col[3].w);
#endif

	return result;
}


mat4
mat4::Translation(const vec4& _vector)
{
	mat4	result;
	result.col[3] = _vector;
	return result;
}


mat4
mat4::Scale(float _factor)
{
	mat4	result;
	result.col[0].x = _factor;
	result.col[1].y = _factor;
	result.col[2].z = _factor;
	return result;
}


mat4
mat4::Frustum(float _near, float _far, float _width, float _height)
{
	mat4	result;

	float	depth = _far - _near;
	float	w = _near / (_width / 2.0f);
	float	h = _near / (_height / 2.0f);
	float	q = -(_far + _near) / depth;
	float	qn = -2.f * (_far * _near) / depth;
	
	result.col[0].x = w;
	result.col[1].y = h;
	result.col[2].z = q;
	result.col[2].w = -1.0f;
	result.col[3].z = qn;
	result.col[3].w = 0.0f;

	return result;
}


mat4
mat4::Perspective(float _near, float _far, float _fov, float _ratio)
{
#if 0
	float height = 2 * _near * (float)tan(_fov * (PI / 360.f));
	float width = height * _ratio;
	
	return Frustum(_near, _far, width, height);

#else
	// KNOWN TO WORK (YOLORenderSQUAD)
	mat4	result;

	float xymax = _near * (float)tan(_fov * (PI / 360.f));
	float width = 2 * xymax;
	float depth = _far - _near;
	float q = -(_far + _near) / depth;
	float qn = -2.f * _far * _near / depth;
	float w = (2.f * _near / width) / _ratio;
	float h = 2.f * _near / width;
	
	result.col[0].x = w;
	result.col[1].y = h;
	result.col[2].z = q;
	result.col[2].w = -1.f;
	result.col[3].z = qn;
	result.col[3].w = 0.f;
	
	return result;
#endif
}



}
