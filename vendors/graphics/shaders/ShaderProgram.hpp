#pragma once
#include <array>
#include <format>
#include <fstream>
#include <glad/glad.h>
#include <string>

class ShaderProgram
{
public:
	ShaderProgram()
		: m_program(0)
	{
	}

	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator=(const ShaderProgram&) = delete;

	~ShaderProgram()
	{
		if (m_program)
		{
			glDeleteProgram(m_program);
		}
	}

	void Use()
	{
		glUseProgram(m_program);
	}

	void LoadFromFile(const std::string& vertexFilePath, const std::string& fragmentFilePath)
	{
		GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexFilePath);
		GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentFilePath);

		m_program = glCreateProgram();

		glAttachShader(m_program, vertexShader);
		glAttachShader(m_program, fragmentShader);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		glLinkProgram(m_program);
		if (HasLinkErrors(m_program))
		{
			glDeleteProgram(m_program);
			m_program = 0;
		}
	}

	void SetUniformMat3(const std::string& str, const std::array<float, 9>& data)
	{
		GLint location = glGetUniformLocation(m_program, str.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, data.data());
	}

	void SetUniformVec4(const std::string& str, const std::array<float, 4>& data)
	{
		GLint location = glGetUniformLocation(m_program, str.c_str());
		glUniform4f(location, data[0], data[1], data[2], data[3]);
	}

private:
	static GLuint CompileShader(GLenum shaderType, const std::string& filename)
	{
		GLuint shader = glCreateShader(shaderType);
		std::string sourceText = ReadFileText(filename);
		const GLchar* shaderSource = sourceText.c_str();

		glShaderSource(shader, 1, &shaderSource, nullptr);
		glCompileShader(shader);

		if (HasCompilationErrors(shader))
		{
			std::string error = GetShaderCompilationErrors(shader);
			glDeleteShader(shader);
			std::string shaderTypeStr = shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment";
			throw std::runtime_error("Error compiling " + shaderTypeStr + " shader: " + error);
		}

		return shader;
	}

	static std::string ReadFileText(const std::string& filename)
	{
		std::ifstream in(filename);
		if (!in.is_open())
		{
			throw std::runtime_error("Could not open file " + filename);
		}

		std::string source{
			std::istreambuf_iterator<char>(in),
			std::istreambuf_iterator<char>()
		};
		return source;
	}

	static GLuint LinkProgram(GLuint& vertexShader, GLuint& fragmentShader)
	{
		GLuint program = glCreateProgram();

		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		glLinkProgram(program);
		if (HasLinkErrors(program))
		{
			glDeleteProgram(program);
			std::string error = GetProgramLinkErrors(program);
			throw std::runtime_error("Error linking " + error);
		}

		return program;
	}

	static bool HasCompilationErrors(GLuint shaderId)
	{
		GLint isCompiled = 0;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);

		return !isCompiled;
	}

	static bool HasLinkErrors(GLuint programId)
	{
		GLint isLinked = 0;
		glGetProgramiv(programId, GL_LINK_STATUS, &isLinked);

		return !isLinked;
	}

	static std::string GetShaderCompilationErrors(GLuint shaderId)
	{
		GLint logLength = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
		std::string logInfo(logLength, '\0');
		glGetShaderInfoLog(shaderId, logLength, &logLength, logInfo.data());
		return logInfo;
	}

	static std::string GetProgramLinkErrors(GLuint program)
	{
		GLint logLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		std::string logInfo(logLength, '\0');
		glGetProgramInfoLog(program, logLength, &logLength, logInfo.data());
		return logInfo;
	}

	GLuint m_program;
};