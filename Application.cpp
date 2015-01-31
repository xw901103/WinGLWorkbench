#include "Application.h"
#ifdef _MSC_VER
#include <Windows.h>
#endif

Application* Application::instance = 0;

Application* Application::getInstance()	
{
	return Application::instance;
}

Application::Application() : running(false)
{
	Application::instance = this;
}

#ifdef _MSC_VER
Application::Application(HINSTANCE hInitInstance) : running(false), hInstance(hInitInstance)
{
	Application::instance = this;
}
#endif

Application::~Application()
{
	Application::instance = 0;
}

int Application::execute()
{
	int result = 0;

	this->running = true;

	while (this->running)
	{
#ifdef _MSC_VER
		MSG msg;
		//while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) == TRUE)
		//{
			if (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			} else {
				this->running = false;
			}
		//}
#endif
	}

	return result;
}

void Application::quit() {
	this->running = false;
}
