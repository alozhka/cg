#pragma once
#include <glm/fwd.hpp>
#include <graphics/windows/GraphicsApplication.hpp>

class MemoryTrainerApp final : public GraphicsApplication
{
public:
	MemoryTrainerApp(int width, int height, const std::string& title)
		: GraphicsApplication(width, height, title)
	{
	}

	void OnDraw(const glm::mat4& perspective) override
	{
	}
};
