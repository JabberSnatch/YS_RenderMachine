#ifndef __YS_SHADER_HPP__
#define __YS_SHADER_HPP__

#include "glew-2.0.0/include/GL/glew.h"
#include <unordered_map>
#include <string>


namespace ys_render_machine {


class ShaderStage
{
public:
	ShaderStage() = delete;
	ShaderStage(GLuint _gl_shader_stage);
	~ShaderStage();

	void	CompileFile(const std::string& _path);
	void	CompileSource(const std::string& _source);

	GLuint	gl_name() const { return m_gl_name; }
	GLuint	gl_shader_stage() const { return m_gl_shader_stage; }

private:
	GLuint			m_gl_name = 0;
	GLuint			m_gl_shader_stage = 0;
	bool			m_compiled = false;
	std::string		m_source = "";
};


class Shader
{
public:
	// Maps each ShaderStage to an OpenGL pipeline stage (vertex, fragment, ..)
	using t_ShaderStageMap = std::unordered_map<GLuint, const ShaderStage*>;

	Shader();
	~Shader();

	void		AddShaderStage(const ShaderStage* _stage);
	void		Link();
	void		Enable();
	static void	Disable();

private:
	GLuint				m_gl_name = 0;
	bool				m_linked = false;
	t_ShaderStageMap	m_stages;
};


}


#endif // __YS_SHADER_HPP__
