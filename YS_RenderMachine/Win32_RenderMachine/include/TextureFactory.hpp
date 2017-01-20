#ifndef __YS_TEXTURE_FACTORY_HPP__
#define __YS_TEXTURE_FACTORY_HPP__

#include "Scene.hpp"

namespace ys_render_machine {


class TextureFactory
{
public:
	static Texture* LoadIntoScene(Scene& _scene, const std::string& _file);
	
};


} // namespace ys_render_machine


#endif // __YS_TEXTURE_FACTORY_HPP__
