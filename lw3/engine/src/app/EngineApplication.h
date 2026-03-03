#pragma once
#include "../engine/Engine.hpp"
#include "../shared/CompositeObject.hpp"
#include "GuiApplication.h"
#include "../render/ShaderProgram.hpp"

class EngineApplication : public GuiApplication
{
public:
	EngineApplication(int width, int height, const std::string& title);
	~EngineApplication() override = default;

protected:
	void OnDraw() override;

private:
	double GetDeltaTime();

	ShaderProgram m_shader;
	Engine m_engine;
	double m_lastTime = 0;
};
