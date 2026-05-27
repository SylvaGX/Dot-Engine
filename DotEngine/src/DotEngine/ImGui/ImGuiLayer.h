#pragma once

#include "DotEngine/Layer.h"

#include "DotEngine/Events/ApplicationEvent.h"
#include "DotEngine/Events/KeyEvent.h"
#include "DotEngine/Events/MouseEvent.h"

#ifdef DOTENGINE_PLATFORM_MACOS
namespace MTL
{
	class CommandBuffer;
	class RenderCommandEncoder;
}

namespace CA
{
	class MetalDrawable;
}
#endif

namespace DotEngine {

	class DOTENGINE_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer() override;

		void OnAttach() override;
		void OnDetach() override;
		void OnImGuiRender() override;

		void Begin();
		void End();
	private:
#ifdef DOTENGINE_PLATFORM_MACOS
		MTL::RenderCommandEncoder* m_RenderCommandEncoder;
		MTL::CommandBuffer* m_CommandBuffer;
		CA::MetalDrawable* m_Drawable;
#endif
		float m_Time = 0.0f;
	};
}
