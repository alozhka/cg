#pragma once
#include "sea_battle/model/SeaBattleGame.hpp"
#include "sea_battle/view/SeaBattleGameView.hpp"

#include <graphics/DrawableModel.hpp>
#include <graphics/camera/FirstPersonCamera.hpp>
#include <graphics/light/DirectLight.hpp>
#include <graphics/shaders/ShaderProgram.hpp>
#include <graphics/textures/TextureCache.hpp>
#include <graphics/windows/GraphicsApplication.hpp>
#include <graphics/windows/TimeProvider.hpp>

class SeaBattleApp final : public GraphicsApplication
{
public:
	SeaBattleApp(int width, int height, const std::string& title)
		: GraphicsApplication(width, height, title)
		, m_gameViewModel(m_game)
		, m_gameView(m_gameViewModel, m_textureCache)
	{
		m_shader.LoadFromFile("assets/phong.vert", "assets/phong.frag");
		glEnable(GL_DEPTH_TEST);
		CaptureMouseInput();

		m_camera.SetPosition({ 0, 12, 25 });
		m_camera.AddYaw(glm::radians(-90.0f));
		m_camera.AddPitch(glm::radians(-25.0f));

		m_light.SetDirection({ -0.4f, -1.0f, -0.2f });
		m_light.SetAmbientIntensity(0.5f, 0.5f, 0.5f);
		m_light.SetDiffuseIntensity(1.0f, 1.0f, 1.0f);
		m_light.SetSpecularIntensity(0.4f, 0.4f, 0.4f);
	}

protected:
	void OnDraw(const glm::mat4& perspective) override
	{
		glClearColor(0.1, 0.11, 0.13, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Update();
		const glm::mat4 viewProjection = perspective * m_camera.GetViewMatrix();

		m_shader.Use();
		m_shader.SetUniformMat4("uViewProjection", viewProjection);
		m_shader.SetUniformVec3("uCameraPos", m_camera.GetPosition());
		m_shader.SetUniformInt("uPointLightCount", 0);
		m_light.Apply(m_shader);

		m_gameView.Draw(m_shader);
	}

private:
	void Update()
	{
		float dt = m_time.GetDeltaTime();
		m_game->Update(dt);
	}

	TextureCache m_textureCache;
	ShaderProgram m_shader;
	FirstPersonCamera m_camera;
	DirectLight m_light;
	TimeProvider m_time;

	SeaBattleGamePtr m_game = std::make_shared<SeaBattleGame>();
	SeaBattleGameViewModel m_gameViewModel;
	SeaBattleGameView m_gameView;
};