#pragma once
#include <graphics/Mesh.hpp>
#include <graphics/windows/3d/GraphicsApplication.hpp>

class PolyhedronApp final : public GraphicsApplication
{
public:
	explicit PolyhedronApp(int width, int height, const std::string& title)
		: GraphicsApplication(width, height, title)
	{
	}

protected:
	void OnDraw(const Mat4& projection) override
	{
		Mat4 view = Mat4::LookAt();
	}

private:
	Mesh m_rect;
};