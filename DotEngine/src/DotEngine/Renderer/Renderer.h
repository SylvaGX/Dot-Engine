#pragma once

namespace DotEngine {

	enum class RendererAPI{
		None = 0,
		OpenGL = 1,
		Metal = 2
	};

	class Renderer {
	public:
		static RendererAPI GetAPI() { return s_RendererAPI; }
	private:
		static RendererAPI s_RendererAPI;
	};

}