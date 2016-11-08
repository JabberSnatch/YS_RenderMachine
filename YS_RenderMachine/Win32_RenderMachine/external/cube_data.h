#ifndef __YS_CUBE_DATA_HPP__
#define __YS_CUBE_DATA_HPP__

#include <cstdint>


namespace ys_primitives
{

static float		ys_cube_vertex[] =
{
	-.5f, -.5f, -.5f, // 0
	-.5f, -.5f, .5f,
	-.5f, .5f, -.5f,
	-.5f, .5f, .5f,

	.5f, -.5f, -.5f, // 4
	.5f, -.5f, .5f,
	.5f, .5f, -.5f,
	.5f, .5f, .5f // 7
};

static uint32_t		ys_cube_indices[] =
{
	0, 4, 6,	6, 2, 0,
	1, 5, 7,	7, 3, 1,
	3, 2, 0,	0, 1, 3,
	7, 6, 4,	4, 5, 7,
	0, 4, 5,	5, 1, 0,
	2, 6, 7,	7, 3, 2
};

}


#endif // __YS_CUBE_DATA_HPP__
