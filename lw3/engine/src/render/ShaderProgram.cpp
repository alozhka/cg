#include "ShaderProgram.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

ShaderProgram::~ShaderProgram()
{
	if (m_program)
		glDeleteProgram(m_program);
}

static std::string readFile(const std::string& path)
{
	std::ifstream f(path);
	if (!f)
		return {};
	std::stringstream buf;
	buf << f.rdbuf();
	return buf.str();
}

bool ShaderProgram::LoadFromFile(const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vs = readFile(vertexPath);
	std::string fs = readFile(fragmentPath);
	if (vs.empty() || fs.empty())
		return false;
	return LoadFromSource(vs, fs);
}

bool ShaderProgram::LoadFromSource(const std::string& vertexSource, const std::string& fragmentSource)
{
	GLuint vert = glCreateShader(GL_VERTEX_SHADER);
	GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
	if (!vert || !frag)
		return false;

	const char* vsPtr = vertexSource.c_str();
	const char* fsPtr = fragmentSource.c_str();
	glShaderSource(vert, 1, &vsPtr, nullptr);
	glShaderSource(frag, 1, &fsPtr, nullptr);

	if (!compileShader(vert, vsPtr, "vertex") || !compileShader(frag, fsPtr, "fragment"))
	{
		glDeleteShader(vert);
		glDeleteShader(frag);
		return false;
	}

	m_program = glCreateProgram();
	if (!m_program)
	{
		glDeleteShader(vert);
		glDeleteShader(frag);
		return false;
	}
	glAttachShader(m_program, vert);
	glAttachShader(m_program, frag);
	glDeleteShader(vert);
	glDeleteShader(frag);

	if (!linkProgram())
	{
		glDeleteProgram(m_program);
		m_program = 0;
		return false;
	}
	return true;
}

bool ShaderProgram::compileShader(GLuint shader, const char* source, const char* label)
{
	glCompileShader(shader);
	GLint ok = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
	if (!ok)
	{
		GLint len = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
		std::string log(len, '\0');
		glGetShaderInfoLog(shader, len, &len, log.data());
		std::cerr << "Shader " << label << " compile error:\n" << log << std::endl;
		return false;
	}
	return true;
}

bool ShaderProgram::linkProgram()
{
	glLinkProgram(m_program);
	GLint ok = 0;
	glGetProgramiv(m_program, GL_LINK_STATUS, &ok);
	if (!ok)
	{
		GLint len = 0;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &len);
		std::string log(len, '\0');
		glGetProgramInfoLog(m_program, len, &len, log.data());
		std::cerr << "Program link error:\n" << log << std::endl;
		return false;
	}
	return true;
}

void ShaderProgram::Use() const
{
	glUseProgram(m_program);
}

void ShaderProgram::SetMat3(const char* name, const float* value) const
{
	GLint loc = glGetUniformLocation(m_program, name);
	if (loc >= 0)
		glUniformMatrix3fv(loc, 1, GL_FALSE, value);
}

void ShaderProgram::SetVec4(const char* name, float x, float y, float z, float w) const
{
	GLint loc = glGetUniformLocation(m_program, name);
	if (loc >= 0)
	{
		GLfloat v[4] = { x, y, z, w };
		glUniform4fv(loc, 1, v);
	}
}

void ShaderProgram::SetVec4(const char* name, const float* value) const
{
	GLint loc = glGetUniformLocation(m_program, name);
	if (loc >= 0)
		glUniform4fv(loc, 1, value);
}
