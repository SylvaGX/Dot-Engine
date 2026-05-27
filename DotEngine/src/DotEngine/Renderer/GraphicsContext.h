#pragma once

namespace DotEngine {

	enum class GraphicsAPI {
		OpenGL,
		Metal
	};

	class GraphicsContext {
	public:
		virtual ~GraphicsContext() = default;
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
		virtual GraphicsAPI GetAPI() const = 0;
		virtual void* GetDevice() const = 0;
	};

}