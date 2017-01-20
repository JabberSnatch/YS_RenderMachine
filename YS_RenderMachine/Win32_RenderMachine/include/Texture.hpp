#ifndef __YS_TEXTURE_HPP__
#define __YS_TEXTURE_HPP__

#include <string>
#include "glew-2.0.0/include/GL/glew.h"


namespace ys_render_machine {


// Do not instanciate your own Texture. Go through TextureFactory instead.
struct Texture
{
	~Texture()
	{
		glDeleteTextures(1, &m_gl_name);
	}

	GLuint		m_gl_name;
};


} // namespace ys_render_machine


#endif // __YS_TEXTURE_HPP__
