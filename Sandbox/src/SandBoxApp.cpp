#include <DotEngine.h>

#include "imgui.h"
#include "imgui_internal.h"

class ExampleLayer : public DotEngine::Layer {
public:
	ExampleLayer() : Layer("Example") {}

	void OnUpdate() override
	{
		if (m_DockingEnabled)
		{
			if (DotEngine::Input::IsKeyPressed(DOTENGINE_KEY_TAB))
				DOTENGINE_TRACE("Tab key is pressed (poll)!");
		}
	}

	void OnImGuiRender() override {
		if (m_DockingEnabled)
		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;

			if (ImGui::Begin("Test", &m_DockingEnabled, window_flags))
			{
				ImGui::Text("Hello World");
			}
			ImGui::End();
		}
	}

	void OnEvent(DotEngine::Event& event) override {
		if (m_DockingEnabled)
		{
			if (event.GetEventType() == DotEngine::EventType::KeyPressed) {
				const auto& e = dynamic_cast<DotEngine::KeyPressedEvent &>(event);
				DOTENGINE_TRACE("{0}", static_cast<char>(e.GetKeyCode()));
			}
		}
	}

	void OpenWindow()
	{
		m_DockingEnabled = true;
	}

private:
	bool m_DockingEnabled = true;
};

class DockSpaceLayer : public DotEngine::Layer {
public:
	DockSpaceLayer() : Layer("DockSpace") {}

	void OnImGuiRender() override {
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		window_flags |= ImGuiWindowFlags_NoBackground;

		dockspace_flags |= ImGuiDockNodeFlags_PassthruCentralNode;

		dockspace_flags |= ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("DotEngine DockSpace", nullptr, window_flags);

		ImGui::PopStyleVar();

		ImGui::PopStyleVar(2);

		if (const ImGuiIO& io = ImGui::GetIO(); io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			const ImGuiID dockspace_id = ImGui::GetID("DotEngineDockSpace");

			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::MenuItem("Test Window"))
			{
				const auto& app = DotEngine::Application::Get();

				if (const auto example_layer = dynamic_cast<ExampleLayer*>(app.GetLayer("Example")))
				{
					example_layer->OpenWindow();
				}
			}

	        if (ImGui::BeginMenu("Options"))
	        {
	            // Disabling fullscreen would allow the window to be moved to the front of other windows,
	            // which we can't undo at the moment without finer window depth/z control.

	            if (ImGui::MenuItem("Flag: NoSplit",                "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))                 { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
	            if (ImGui::MenuItem("Flag: NoResize",               "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))                { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
	            if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
	            if (ImGui::MenuItem("Flag: AutoHideTabBar",         "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))          { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
	            if (ImGui::MenuItem("Flag: PassthruCentralNode",    "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0))     { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
	            ImGui::Separator();

	            if (ImGui::MenuItem("Close")){}

	            ImGui::EndMenu();
	        }
			//This function is to create a tooltip to help and give tips about the option menu
	        // HelpMarker(
	        //     "When docking is enabled, you can ALWAYS dock MOST window into another! Try it now!" "\n"
	        //     "- Drag from window title bar or their tab to dock/undock." "\n"
	        //     "- Drag from window menu button (upper-left button) to undock an entire node (all windows)." "\n"
	        //     "- Hold SHIFT to disable docking (if io.ConfigDockingWithShift == false, default)" "\n"
	        //     "- Hold SHIFT to enable docking (if io.ConfigDockingWithShift == true)" "\n"
	        //     "This demo app has nothing to do with enabling docking!" "\n\n"
	        //     "This demo app only demonstrate the use of ImGui::DockSpace() which allows you to manually create a docking node _within_ another window." "\n\n"
	        //     "Read comments in ShowExampleAppDockSpace() for more details.");

	        ImGui::EndMenuBar();
	    }

		ImGui::End();
	}
};

class SandBox : public DotEngine::Application {
public:
	SandBox() {
		PushLayer(new DockSpaceLayer());
		PushLayer(new ExampleLayer());
	}
	~SandBox() override = default;

private:

};

DotEngine::Application* DotEngine::createApplication() {

	return new SandBox(); 

}