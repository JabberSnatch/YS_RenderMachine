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
	glDeleteShader(m_gl_name);
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


Shader::Shader()
{
	m_gl_name = glCreateProgram();
}


Shader::~Shader()
{
	for (t_ShaderStageMap::iterator ite = m_stages.begin();
		 ite != m_stages.end(); ++ite)
	{
		glDetachShader(m_gl_name, ite->second->gl_name());
	}

	glDeleteProgram(m_gl_name);
}


void
Shader::AddShaderStage(const ShaderStage* _stage)
{
	GLuint	gl_shader_stage = _stage->gl_shader_stage();
	t_ShaderStageMap::iterator ite = m_stages.find(gl_shader_stage);
	
	if (ite != m_stages.end())
	{
		glDetachShader(m_gl_name, ite->second->gl_name());
		m_stages.erase(ite);
		
		m_linked = false;
	}
	
	m_stages.emplace(gl_shader_stage, _stage);
	glAttachShader(m_gl_name, _stage->gl_name());
}


void
Shader::Link()
{
	if (m_linked)
		return;

	glLinkProgram(m_gl_name);

	GLint success;
	glGetProgramiv(m_gl_name, GL_LINK_STATUS, &success);
	if (success != GL_TRUE)
	{
		char	buffer[1024];
		GLsizei	actual_size;
		glGetProgramInfoLog(m_gl_name, 1024, &actual_size, buffer);

		Logger::Log(std::string(buffer, actual_size), Logger::kChannelRenderer);
	}
	else
		m_linked = true;
}


void
Shader::Enable()
{
	if (!m_linked)
		Link();

	glUseProgram(m_gl_name);
}


void
Shader::Disable()
{
	glUseProgram(0);
}


}
