#include "EngineApplication.h"
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace
{
std::string readFile(const std::string& path)
{
	std::ifstream f(path);
	if (!f)
		return {};
	std::stringstream buf;
	buf << f.rdbuf();
	return buf.str();
}
}

EngineApplication::EngineApplication(int width, int height, const std::string& title)
	: GuiApplication(width, height, title)
{
	m_engine.SetScale(1.2);
	m_lastTime = glfwGetTime();

	// Try loading shaders from file (when run from project root or with assets next to executable)
	std::string vs = readFile("assets/shaders/vertex.glsl");
	std::string fs = readFile("assets/shaders/fragment.glsl");
	if (vs.empty() || fs.empty())
	{
		vs = R"(#version 330 core
layout (location = 0) in vec2 aPos;
uniform mat3 uMVP;
void main() { vec3 pos = uMVP * vec3(aPos, 1.0); gl_Position = vec4(pos.x, pos.y, 0.0, 1.0); }
)";
		fs = R"(#version 330 core
out vec4 FragColor;
uniform vec4 uColor;
void main() { FragColor = uColor; }
)";
	}
	if (!m_shader.LoadFromSource(vs, fs))
		throw std::runtime_error("Failed to load shaders");
}

void EngineApplication::OnDraw()
{
	glClearColor(0.7f, 0.7f, 0.7f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	double dt = GetDeltaTime();
	m_engine.Update(dt);
	m_engine.Draw(m_shader, GetProjectionMatrix());
}

double EngineApplication::GetDeltaTime()
{
	double t = glfwGetTime();
	double dt = t - m_lastTime;
	m_lastTime = t;
	return dt;
}
