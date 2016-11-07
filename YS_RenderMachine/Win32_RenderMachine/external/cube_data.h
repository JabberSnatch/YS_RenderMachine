#ifndef __YS_CUBE_DATA_HPP__
#define __YS_CUBE_DATA_HPP__

#include <cstdint>


namespace ys_primitives
{

static float		ys_cube_vertex[] =
{
	-.5f, -.5f, -.5f,
	-.5f, -.5f, .5f,
	-.5f, .5f, -.5f,
	-.5f, .5f, .5f,

	.5f, -.5f, -.5f,
	.5f, -.5f, .5f,
	.5f, .5f, -.5f,
	.5f, .5f, .5f
};

static uint32_t		ys_cube_indices[] =
{
	0, 6, 4,	0, 2, 6,
	0, 4, 2,	0, 1, 3,
	2, 7, 6,	2, 3, 7,
	4, 6, 7,	4, 7, 5,
	0, 4, 5,	0, 5, 1,
	1, 5, 7,	1, 7, 3
};

}


#endif // __YS_CUBE_DATA_HPP__
