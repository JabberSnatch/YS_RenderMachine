#include "Shader.hpp"

#include <fstream>
#include "Logger.hpp"


namespace ys_render_machine {


ShaderStage::ShaderStage(GLuint _gl_shader_stage)
	:m_gl_shader_stage(_gl_shader_stage)
{
	m_gl_name = glCreateShader(m_gl_shader_stage);
}


ShaderStage::~ShaderStage()
{

}


void
ShaderStage::CompileFile(const std::string& _path)
{
	std::fstream	file(_path, std::ios_base::in);
	std::string		source((std::istreambuf_iterator<char>(file)),
							std::istreambuf_iterator<char>());
	file.close();

	CompileSource(source);
}


void
ShaderStage::CompileSource(const std::string& _source)
{
	const char* sources[1] = { _source.c_str() };
	glShaderSource(m_gl_name, 1, sources, nullptr);
	glCompileShader(m_gl_name);
	
	GLint success;
	glGetShaderiv(m_gl_name, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE)
	{
		char	buffer[1024];
		GLsizei	actual_size;
		glGetShaderInfoLog(m_gl_name, 1024, &actual_size, buffer);

		Logger::Log(std::string(buffer, actual_size), Logger::kChannelRenderer);
	}
	else 
	{
		m_compiled = true;
		m_source = _source;
	}
}


Shader::~Shader()
{

}


}
