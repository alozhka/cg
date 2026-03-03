#pragma once

#include <glad/glad.h>
#include <string>

class ShaderProgram
{
public:
	ShaderProgram() = default;
	~ShaderProgram();

	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator=(const ShaderProgram&) = delete;

	bool LoadFromFile(const std::string& vertexPath, const std::string& fragmentPath);
	bool LoadFromSource(const std::string& vertexSource, const std::string& fragmentSource);

	void Use() const;
	void SetMat3(const char* name, const float* value) const;
	void SetVec4(const char* name, float x, float y, float z, float w) const;
	void SetVec4(const char* name, const float* value) const;

	GLuint GetId() const { return m_program; }
	bool IsValid() const { return m_program != 0; }

private:
	bool compileShader(GLuint shader, const char* source, const char* label);
	bool linkProgram();

	GLuint m_program = 0;
};
