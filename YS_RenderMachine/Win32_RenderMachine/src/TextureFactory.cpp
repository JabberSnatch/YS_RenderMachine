#include "TextureFactory.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"

namespace ys_render_machine {


Texture*
TextureFactory::LoadIntoScene(Scene& _scene, const std::string& _file)
{
	Texture* texture = nullptr;

	int w, h, d;
	stbi_info(_file.c_str(), &w, &h, &d);
	auto channels = STBI_rgb;
	if (d == 4) channels = STBI_rgb_alpha;

	auto* data = stbi_load(_file.c_str(), &w, &h, &d, channels);

	if (data != nullptr)
	{
		auto internal_format = (d == 4) ? GL_SRGB_ALPHA : GL_SRGB;
		auto format = (d == 4) ? GL_RGBA : GL_RGB;

		texture = new Texture();
		glGenTextures(1, &texture->m_gl_name);
		glBindTexture(GL_TEXTURE_2D, texture->m_gl_name);
		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, w, h, 0, format, 
					 GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(data);

		_scene.textures().push_back(texture);
	}

	return texture;
}


} // namespace ys_render_machine
