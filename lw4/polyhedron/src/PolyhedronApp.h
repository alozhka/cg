#pragma once
#include <graphics/windows/GraphicsApplication.hpp>

class PolyhedronApp final : public GraphicsApplication
{
public:
	explicit PolyhedronApp(int width, int height, const std::string& title)
		: GraphicsApplication(width, height, title)
	{
	}

protected:
	void OnDraw(const Mat3& projection) override
	{
	}
};