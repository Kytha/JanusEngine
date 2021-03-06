#include "jnpch.h"

#include "Core/Events/ApplicationEvent.h"
#include "Core/Events/MouseEvent.h"
#include "Core/Events/KeyEvent.h"

#include "Platform/Windows/WindowsWindow.h"
#include "Core/stb_image/stb_image.h"
namespace Janus
{
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char *description)
	{
		JN_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window *Window::Create(const WindowProps &props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps &props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
	}

	void WindowsWindow::Init(const WindowProps &props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		JN_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			JN_ASSERT(success, "WINDOW_ERROR: Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}
		
		// Create glfw window
		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		

		GLFWimage images[1]; 
		images[0].pixels = stbi_load("assets/icons/janus-logo.png", &images[0].width, &images[0].height, 0, 4); //rgba channels 
		glfwSetWindowIcon(m_Window, 1, images); 
		stbi_image_free(images[0].pixels);
		//m_Context = new OpenGLContext(m_Window);
		//m_Context->Init();
		//^

		glfwMakeContextCurrent(m_Window);

		// Get Hardware specific implementation of OpenGL
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		JN_ASSERT(status, "WINDOW_ERROR: Failed to initialize Glad!");

		JN_CORE_INFO("OpenGL Info:");
		JN_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		JN_CORE_INFO("	Renderer: {0}", glGetString(GL_RENDERER));
		JN_CORE_INFO("	Version: {0}", glGetString(GL_VERSION));

		glfwSetWindowUserPointer(m_Window, &m_Data);

		SetVSync(false);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow *window, int width, int height)
								  {
									  WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
									  data.Width = width;
									  data.Height = height;

									  WindowResizeEvent event(width, height);
									  data.EventCallback(event);
								  });
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *window)
								   {
									   WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
									   WindowCloseEvent event;
									   data.EventCallback(event);
								   });
		glfwSetKeyCallback(m_Window, [](GLFWwindow *window, int key, int scancode, int action, int mods)
						   {
							   WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
							   switch (action)
							   {
							   case GLFW_PRESS:
							   {
								   KeyPressedEvent event(key, 0);
								   data.EventCallback(event);
								   break;
							   }
							   case GLFW_REPEAT:
							   {
								   KeyPressedEvent event(key, 1);
								   data.EventCallback(event);
								   break;
							   }
							   case GLFW_RELEASE:
							   {
								   KeyReleasedEvent event(key);
								   data.EventCallback(event);
								   break;
							   }
							   }
						   });

		glfwSetCharCallback(m_Window, [](GLFWwindow *window, unsigned int keycode)
							{
								WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
								KeyTypedEvent event(keycode);
								data.EventCallback(event);
							});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow *window, int button, int action, int mods)
								   {
									   WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);

									   switch (action)
									   {
									   case GLFW_PRESS:
									   {
										   MouseButtonPressedEvent event(button);
										   data.EventCallback(event);
										   break;
									   }

									   case GLFW_RELEASE:
									   {
										   MouseButtonReleasedEvent event(button);
										   data.EventCallback(event);
										   break;
									   }
									   }
								   });
		glfwSetScrollCallback(m_Window, [](GLFWwindow *window, double xOffset, double yOffset)
							  {
								  WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
								  MouseScrolledEvent event((float)xOffset, (float)yOffset);
								  data.EventCallback(event);
							  });

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow *window, double xPos, double yPose)
								 {
									 WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
									 MouseMovedEvent event((float)xPos, (float)yPose);
									 data.EventCallback(event);
								 });
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
}