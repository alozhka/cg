#pragma once
#include <Mat3.hpp>

#include <windows/GuiApplication.hpp>

class PolyhedronApp final : public GuiApplication
{
public:
	explicit PolyhedronApp(int width, int height, const std::string& title)
		: GuiApplication(width, height, title)
	{
	}

protected:
	void OnDraw(const Mat3& projection) override
	{
	}
};