#pragma once

#include "Core/Core.h"
#include "Core/Timestep.h"
#include "Core/Window.h"
#include "Core/Events/Event.h"
#include "Core/LayerStack.h"
#include "Core/Events/ApplicationEvent.h"

#include "ImGui/ImGuiLayer.h"
namespace Janus
{

	// Application is a signleton object which maintains the window and game loop
	class Application
	{

	public:
		Application(const std::string& name);
		virtual ~Application();

		void Run();

		void OnEvent(Event &e);

		void PushLayer(Layer *layer);

		void PushOverlay(Layer *layer);

		void Close();
		// Exposes the window object for input polling
		inline Window &GetWindow() { return *m_Window; }
		inline static Application &Get() { return *s_Instance; }
		ImGuiLayer *GetImGuiLayer() { return m_ImGuiLayer; }
		void RenderImGui();
		std::string Application::OpenFile(const char* filter) const;

	private:
		bool OnWindowClose(WindowCloseEvent &e);
		bool OnWindowResize(WindowResizeEvent &e);
		// Pointer to the applications window object. An application can only have one window
		std::unique_ptr<Window> m_Window;
		ImGuiLayer *m_ImGuiLayer;
		LayerStack m_LayerStack;
		bool m_Running = true;
		bool m_Minimized = false;
		float m_LastFrameTime = 0.0f;

	private:
		// Pointer to the apps application object
		static Application *s_Instance;
	};

	// To be defined in client
	// Client defines a Janus Application object to be returned by this function
	Application *CreateApplication();
}
